//
// Created by Larry Zeng on 10/4/20.
//

#include "task_queue.h"
#include "constants.h"


task_queue_t* gloabl_task_queue = NULL;


task_queue_t* create_task_queue(unsigned int process_queue_size) {
    task_queue_t* task_queue = malloc(sizeof(task_queue_t));
    assert(task_queue != NULL);

    task_queue->size = process_queue_size;
    task_queue->tasks = malloc(process_queue_size * sizeof(task_node_t*));
    assert(task_queue->tasks != NULL);

    task_queue->count = 0;
    task_queue->fill_ptr = 0;
    task_queue->use_ptr = 0;
    task_queue->end = 0;
    return task_queue;
}

task_queue_t* quick_create_task_queue() {
    return create_task_queue(DEFAULT_TASK_QUEUE_SIZE);
}

void put_task(task_node_t* task_node) {
    gloabl_task_queue->tasks[gloabl_task_queue->fill_ptr] = task_node;
    gloabl_task_queue->fill_ptr = (gloabl_task_queue->fill_ptr+1) % gloabl_task_queue->size;
    gloabl_task_queue->count++;
}

task_node_t* get_task() {
    task_node_t* task_node = gloabl_task_queue->tasks[gloabl_task_queue->use_ptr];
    gloabl_task_queue->use_ptr = (gloabl_task_queue->use_ptr + 1) % gloabl_task_queue->size;
    gloabl_task_queue->count--;

    return task_node;
}

void destroy_task_node(task_node_t** tn) {
    free((*tn)->file_name);
    free(*tn);
    *tn = NULL;
}

void destroy_task_queue(task_queue_t** tq_ptr) {
    task_queue_t* task_queue = *tq_ptr;
    free(task_queue->tasks);
    free(task_queue);
    *tq_ptr = NULL;
}

task_node_t* create_task_node(char* file_name, unsigned long long file_position, unsigned long long write_data_queue_position) {
    task_node_t* task_node = malloc(sizeof(task_node_t));
    assert(task_node != NULL);
    task_node->file_name = file_name;
    task_node->file_position = file_position;
    task_node->write_data_queue_position = write_data_queue_position;

    return task_node;
}

write_data_t* create_write_data(void* main_data, char first, int first_count, char last, int last_count) {
    write_data_t* write_data = malloc(sizeof(write_data_t));
    assert(write_data != NULL);
    write_data->main_data = main_data;
    write_data->first_char = first;
    write_data->first_count = first_count;
    write_data->last_char = last;
    write_data->last_count = last_count;

    return write_data;
}

void destroy_write_data(write_data_t** write_data_ptr) {
    write_data_t* write_data = *write_data_ptr;
    free(write_data->main_data);
    free(write_data);
    *write_data_ptr = NULL;
}

void put_data(write_data_t* data, unsigned long long position) {
    global_write_queue->write_data_queue[position] = data;
}

write_data_t* get_data() {
    write_data_t* data = global_write_queue->write_data_queue[global_write_queue->current_work_position++];
    return data;
}

write_queue_t* create_write_queue(unsigned long long queue_size) {
    write_queue_t* write_queue = malloc(sizeof(write_queue_t));
    assert(write_queue != NULL);
    
    write_queue->queue_size = queue_size;

    write_queue->write_data_queue = malloc(queue_size * sizeof(write_data_t*));
    assert(write_queue->write_data_queue != NULL);
    for (int i = 0; i < queue_size; i++) {
        write_queue->write_data_queue[i] = NULL;
    }
    
    write_queue->current_work_position = 1;

    return write_queue;
}

void destroy_write_queue(write_queue_t** wq_ptr) {
    write_queue_t* write_queue = *wq_ptr;
    free(write_queue->write_data_queue);
    free(write_queue);
    *wq_ptr = NULL;
}
