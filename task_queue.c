//
// Created by Larry Zeng on 10/4/20.
//

#include "task_queue.h"
#include "constants.h"


task_queue_t* create_task_queue(unsigned int process_queue_size) {
    task_queue_t* task_queue = malloc(sizeof(task_queue_t));
    task_queue->tasks = malloc(process_queue_size * sizeof(task_node_t));

    return task_queue;
}

task_queue_t* quick_create_task_queue() {
    return create_task_queue(DEFAULT_TASK_QUEUE_SIZE);
}

void destroy_task_node(task_node_t** tn) {
    free((*tn)->chunk);
    free(*tn);
    *tn = NULL;
}

void destroy_task_queue(task_queue_t** tq) {
    free(*tq);
    *tq = NULL;
}

task_node_t* create_task_node(char* chunk, int write_data_queue_position) {
    task_node_t* task_node = malloc(sizeof(task_node_t));
    task_node->chunk = chunk;
    task_node->write_data_queue_position = write_data_queue_position;

    return task_node;
}
