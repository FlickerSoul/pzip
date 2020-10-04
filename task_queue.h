//
// Created by Larry Zeng on 10/4/20.
//

#ifndef PZIP_TASK_QUEUE_H
#define PZIP_TASK_QUEUE_H

typedef struct _task_queue_node {
    unsigned int serial;
    task_queue_node previous;
} task_queue_node;

typedef struct _task_queue {
    task_queue_node last_task;
} task_queue;

#endif //PZIP_TASK_QUEUE_H
