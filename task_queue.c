//
// Created by Larry Zeng on 10/4/20.
//

#include "task_queue.h"
#include "constants.h"


task_queue_t* gloabl_task_queue = NULL;


task_queue_t* create_task_queue(unsigned int process_queue_size) {
    task_queue_t* task_queue = malloc(sizeof(task_queue_t));
    task_queue->size = process_queue_size;
    task_queue->tasks = malloc(process_queue_size * sizeof(task_node_t*));
    task_queue->fill_ptr = 0;
    task_queue->use_ptr = 0;
    return task_queue;
}

task_queue_t* quick_create_task_queue() {
    return create_task_queue(DEFAULT_TASK_QUEUE_SIZE);
}

void put_task(task_node_t* task_node) {
    gloabl_task_queue->tasks[gloabl_task_queue->fill_ptr] = task_node;
    gloabl_task_queue->fill_ptr = (gloabl_task_queue->fill_ptr+1) % gloabl_task_queue->size;
}

task_node_t* get_task() {
    task_node_t* task_node = gloabl_task_queue->tasks[gloabl_task_queue->use_ptr];
    gloabl_task_queue->use_ptr = (gloabl_task_queue->use_ptr + 1) % gloabl_task_queue->size;

    return task_node;
}

void destroy_task_node(task_node_t** tn) {
    free((*tn)->chunk);
    free(*tn);
    *tn = NULL;
}

void destroy_task_queue(task_queue_t** tq_ptr) {
    task_queue_t* task_queue = *tq_ptr;
    free(task_queue->tasks);
    free(task_queue);
    *tq_ptr = NULL;
}

task_node_t* create_task_node(char* chunk, int write_data_queue_position) {
    task_node_t* task_node = malloc(sizeof(task_node_t));
    task_node->chunk = chunk;
    task_node->write_data_queue_position = write_data_queue_position;

    return task_node;
}

write_data_t* create_write_data(void* main_data, char first, int first_count, char last, int last_count) {
    write_data_t* write_data = malloc(sizeof(write_data_t));
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

write_queue_t* create_write_queue(unsigned int queue_size) {
    write_queue_t* write_queue = malloc(sizeof(write_queue_t));
    write_queue->queue_size = 0;
    write_queue->write_data_queue = malloc(queue_size * sizeof(write_data_t));
    write_queue->current_work_position = 1;

    return write_queue;
}

void destroy_write_queue(write_queue_t** wq_ptr) {
    write_queue_t* write_queue = *wq_ptr;
    free(write_queue->write_data_queue);
    free(write_queue);
    *wq_ptr = NULL;
}
