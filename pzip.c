#include "pzip_utils.h"

int main(int argc, char** argv) {
    get_thread_num();

    if (argc < 2) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }

    printf("start allocated reader argv\n");

    void* read_worker_argv = malloc((argc - 1) * sizeof(char*) + sizeof(int));

    void* temp = read_worker_argv;

    printf("allocated reader argv\n");

    *(int*)temp = argc - 1;
    printf("read argc: %i\n", *(int*)read_worker_argv);
    temp += INT_SIZE;

    printf("loaded file sizes\n");

    for (int i = 1; i < argc; i++) {
        *(char**)temp = strdup(argv[i]);
        printf("added file names: %s\n", *(char**)temp);
        temp += CHAR_PTR_SIZE;
    }

    printf("loaded file names\n");

    global_task_queue = quick_create_task_queue();

    printf("created task queue\n");

    pthread_t read_thread;
    pthread_t compression_threads[THREAD_NUM];
    pthread_t write_thread;

    pthread_create(&read_thread, NULL, file_reader, read_worker_argv);
    pthread_create(&write_thread, NULL, file_writer, NULL);
    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_create(&compression_threads[i], NULL, compression_worker, NULL);
    }

    printf("start join\n");
    pthread_join(write_thread, NULL);
    printf("joined write\n");
    pthread_join(read_thread, NULL);
    printf("joined read\n");
    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_join(compression_threads[i], NULL);
    }
    printf("joined compression\n");
    printf("end join\n");


    temp = read_worker_argv + INT_SIZE;
    for (int i = 0; i < THREAD_NUM; i++) {
        free(*(char**)temp);
        temp += CHAR_PTR_SIZE;
    }
    free(read_worker_argv);
    // destroy_task_queue(&global_task_queue);
    // destroy_write_queue(&global_write_queue);

    return 0;
}