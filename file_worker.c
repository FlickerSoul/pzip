//
// Created by Larry Zeng on 10/10/20.
//

#include "file_worker.h"

void* file_reader(void* args) {
    printf("start reading\n");
    int file_num = *(int*)args;
    printf("file num: %i\n", file_num);
    args += INT_SIZE;

    char** file_names = (char**) args;
    unsigned int chunck_size_array[file_num];

    unsigned long long write_queue_size = 0;

    for (int i = 0; i < file_num; i++) {
        printf("file name: %s\n", file_names[i]);
        chunck_size_array[i] = get_chunk_num(file_names[i]);
        write_queue_size += chunck_size_array[i];
    }

    pthread_mutex_lock(&global_write_queue_lock);
    global_write_queue = create_write_queue(write_queue_size);
    pthread_cond_signal(&global_write_queue_cond);
    pthread_mutex_unlock(&global_write_queue_lock);

    printf("allocated global queue sizes\n");

    unsigned long long write_queue_position_counter = 0;

    printf("start looping tasks\n");

    for (int i = 0; i < file_num; i++) {
        for (unsigned int j = 0; j < chunck_size_array[i]; j++) {
            pthread_mutex_lock(&task_queue_lock);
            printf("locked task lock\n");
            while (global_task_queue->count == global_task_queue->size) {
                pthread_cond_wait(&task_queue_empty, &task_queue_lock);
            }
            printf("start making data for %s file at position %u at write array %llu \n", file_names[i], j, write_queue_position_counter);

            task_node_t* tn = create_task_node(file_names[i], j, write_queue_position_counter++);

            printf("made data for %s file at position %u at write array %llu \n", file_names[i], j, write_queue_position_counter-1);

            put_task(tn);
            pthread_cond_signal(&task_queue_filled);
            pthread_mutex_unlock(&task_queue_lock);
        }
    }

    printf("end adding tasks\n");

    // indicate the works are over
    // no 996
    pthread_mutex_lock(&task_queue_lock);
    global_task_queue->end = 1;
    pthread_cond_signal(&task_queue_filled);
    pthread_mutex_unlock(&task_queue_lock);

    printf("end indicating end\n");

    return NULL;
}


void output(write_data_t* data) {
    fwrite(&(data->first_count), UINT32_SIZE, 1, stdout);
    printf("%c", data->first_char);

    if (data->main_data != NULL) {
        fwrite(&data->main_data, data->data_chunk_num * WRITE_CHUNK_SIZE, 1, stdout);

        if (data->last_count > 0) {
            fwrite(&data->last_count, UINT32_SIZE, 1, stdout);
            printf("%c", data->last_char);
        }
    }
}


void* file_writer(void* args) {
    printf("start writer\n");
    pthread_mutex_lock(&global_write_queue_lock);
    while (global_write_queue == NULL) {
        printf("start wait for write queue created\n");
        pthread_cond_wait(&global_write_queue_cond, &global_write_queue_lock);
    }
    pthread_mutex_unlock(&global_write_queue_lock);

    printf("write queue created: %p\n", global_write_queue);
    printf("write queue start work position: %llu, queue size: %llu\n", global_write_queue->current_work_position, global_write_queue->queue_size);

    while (global_write_queue->current_work_position < global_write_queue->queue_size) {

        printf("init writing\n");
        write_data_t* previous_data;
        write_data_t* current_data;

        unsigned long long previous_position;

        printf("start getting previous data and current data\n");

        pthread_mutex_lock(&write_queue_lock);

        while (global_write_queue->write_data_queue[global_write_queue->current_work_position-1] == NULL || 
               global_write_queue->write_data_queue[global_write_queue->current_work_position] == NULL) {

            
            printf("waiting for previous data and current data, previous %p, current %p\n", global_write_queue->write_data_queue[global_write_queue->current_work_position-1], global_write_queue->write_data_queue[global_write_queue->current_work_position]);

            pthread_cond_wait(&write_queue_filled, &write_queue_lock);
        }

        previous_position = global_write_queue->current_work_position-1;
        previous_data = global_write_queue->write_data_queue[previous_position];
        current_data = get_data();

        pthread_cond_signal(&write_queue_empty);
        pthread_mutex_unlock(&write_queue_lock);

        printf("got previous data and current data\n");

        if (previous_data->last_count > 0 && previous_data->last_char == current_data->first_char) {
            current_data->first_count += previous_data->last_count;
            previous_data->last_count = 0;
        } 

        output(previous_data);
        destroy_write_data(&global_write_queue->write_data_queue[previous_position]);
    }

    printf("start getting last job\n");
    pthread_mutex_lock(&write_queue_lock);
    printf("acquired lock for last job\n");
    while (global_write_queue->write_data_queue[global_write_queue->queue_size-1] == NULL) {
        printf("yield write lock to wait for data, last job %p\n", global_write_queue->write_data_queue[global_write_queue->queue_size-1]);
        pthread_cond_wait(&write_queue_filled, &write_queue_lock);
        printf("wake up to write last one\n");
    }
    output(global_write_queue->write_data_queue[global_write_queue->queue_size-1]);
    printf("wrote last one\n");

    pthread_mutex_unlock(&write_queue_lock);

    printf("end writing\n");

    return NULL;
}
