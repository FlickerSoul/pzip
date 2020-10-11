#include "pzip_utils.h"

int main(int argc, char** argv) {
    get_thread_num();

    if (argc < 2) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }

    void* read_worker_argv = malloc((argc - 1) * sizeof(char*) + sizeof(int));

    void* temp = read_worker_argv;

    *(int*)temp = argc - 1;
    temp += INT_SIZE;

    for (int i = 1; i < argc; i++) {
        *(char**)temp = strdup(argv[i]);
        temp += CHAR_PTR_SIZE;
    }

    global_task_queue = quick_create_task_queue();

    pthread_t read_thread;
    pthread_t compression_threads[THREAD_NUM];
    pthread_t write_thread;

    pthread_create(&read_thread, NULL, file_reader, temp);
    pthread_create(&write_thread, NULL, file_writer, NULL);
    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_create(&compression_threads[i], NULL, compression_worker, NULL);
    }

    void* dummy_return;
    pthread_join(write_thread, dummy_return);
    pthread_join(read_thread, dummy_return);
    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_join(compression_threads[i], dummy_return);
    }

    return 0;
}