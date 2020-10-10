//
// Created by Larry Zeng on 10/4/20.
//

#include "task_queue.h"
#include "constants.h"


task_queue_t* create_task_queue(unsigned int process_queue_size) {
    task_queue_t* task_queue = malloc(sizeof(task_queue_t));
    task_queue->tasks = malloc(process_queue_size * sizeof(task_node_t));

    return process_queue
}

task_queue_t* quick_create_task_queue() {
    return create_task_queue(DEFAULT_TASK_QUEUE_SIZE)
}

void destroy_task_node(task_node_t* tn) {
    free(tn->chunk);
    free(tn)
}

void destroy_task_queue(task_queue_t* tq) {
    free(tq);
}
