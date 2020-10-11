//
// Created by Larry Zeng on 10/10/20.
//

#include "file_worker.h"
#include <math.h>

void* file_reader(void* args) {
    int file_num = *(int*)args;
    args += INT_SIZE;

    char* file_names = (char*) args;
    unsigned int* chunck_size_array[file_num];

    unsigned long long write_queue_size = 0;

    for (int i = 0; i < file_num; i++) {
        FILE* file = fopen(file_names[i]);
        file_array[i] = file;

        fseek(file, 0L, SEEK_END);
        long file_size = ftell(file);

        unsigned int chunk_size = ceil((double) file_size / CHUNK_SIZE);
        write_queue_size += chunk_size;

        fseek(file, 0L, SEEK_SET);
    }

    write_queue_t* global_write_queue = create_write_queue(write_queue_size);

    unsigned long long write_queue_position_counter = 0;

    for (int i = 0; i < file_num; i++) {
        for (int j = 0; j < chunck_size_array[i]; j++) {
            pthread_mutex_lock(&task_queue_lock);
            while (gloabl_task_queue->count == gloabl_task_queue->size) {
                pthread_cond_wait(&task_queue_lock, &task_queue_lock);
            }
            task_node_t* tn = create_task_node(file_names[i], j, write_queue_position_counter++);
            put_task(tn);
            pthread_cond_signal(&task_queue_filled);
            pthread_mutex_unlock(&task_queue_lock);
        }
    }
    // indicate the works are over
    // no 996
}

