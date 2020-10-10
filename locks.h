//
// Created by Larry Zeng on 10/10/20.
//

#ifndef PZIP_LOCKS_H
#define PZIP_LOCKS_H

#include <pthread.h>


pthread_mutex_t task_queue_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t task_queue_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t task_queue_filled = PTHREAD_COND_INITIALIZER;


pthread_mutex_t write_queue_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t write_queue_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t write_queue_filled = PTHREAD_COND_INITIALIZER;

#endif //PZIP_LOCKS_H
