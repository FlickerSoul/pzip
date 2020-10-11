//
// Created by Larry Zeng on 10/10/20.
//

#include "file_worker.h"
#include <math.h>

void* file_reader(void* args) {
    int file_num = *(int*)args;
    args += INT_SIZE;

    char** file_names = (char**) args;
    unsigned int chunck_size_array[file_num];

    unsigned long long write_queue_size = 0;

    for (int i = 0; i < file_num; i++) {
        FILE* file = fopen(file_names[i], "r");

        fseek(file, 0L, SEEK_END);
        long file_size = ftell(file);

        unsigned int chunk_size = ceil((double) file_size / CHUNK_SIZE);
        write_queue_size += chunk_size;

        fseek(file, 0L, SEEK_SET);
    }

    global_write_queue = create_write_queue(write_queue_size);

    unsigned long long write_queue_position_counter = 0;

    for (int i = 0; i < file_num; i++) {
        for (unsigned int j = 0; j < chunck_size_array[i]; j++) {
            pthread_mutex_lock(&task_queue_lock);
            while (gloabl_task_queue->count == gloabl_task_queue->size) {
                pthread_cond_wait(&task_queue_empty, &task_queue_lock);
            }
            task_node_t* tn = create_task_node(file_names[i], j, write_queue_position_counter++);
            put_task(tn);
            pthread_cond_signal(&task_queue_filled);
            pthread_mutex_unlock(&task_queue_lock);
        }
    }

    pthread_mutex_lock(&task_queue_lock);
    gloabl_task_queue->end = 1;
    pthread_mutex_lock(&task_queue_lock);

    return NULL;
    // indicate the works are over
    // no 996
}


void output(write_data_t* data) {
    fwrite(&data->first_count, UINT32_SIZE, 1, stdout);
    printf("%c", data->first_char);

    if (data->main_data != NULL) {
        fwrite(&data->main_data, data->data_chunk_num * WRITE_CHUNK_SIZE, 1, stdout);
    }

    if (data->last_count > 0) {
        fwrite(&data->last_count, UINT32_SIZE, 1, stdout);
        printf("%c", data->last_char);
    }
}


void* file_writer(void* args) {
    while (global_write_queue->current_work_position == global_write_queue->queue_size) {
        write_data_t* previous_data;
        write_data_t* current_data;

        unsigned long long previous_position;

        pthread_mutex_lock(&write_queue_lock);
        while (global_write_queue->write_data_queue[global_write_queue->current_work_position-1] == NULL || 
               global_write_queue->write_data_queue[global_write_queue->current_work_position] == NULL) {
            pthread_cond_wait(&write_queue_filled, &write_queue_lock);
        }

        previous_position = global_write_queue->current_work_position-1;
        previous_data = global_write_queue->write_data_queue[previous_position];
        current_data = get_data();

        pthread_cond_signal(&write_queue_empty);
        pthread_mutex_unlock(&write_queue_lock);

        if (previous_data->last_char == current_data->first_char) {
            current_data->first_count += previous_data->last_count;
            previous_data->last_count = 0;
        } 

        output(previous_data);
        destroy_write_data(&global_write_queue->write_data_queue[previous_position]);
    }

    output(global_write_queue->write_data_queue[global_write_queue->queue_size-1]);

    return NULL;
}
