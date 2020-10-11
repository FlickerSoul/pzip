//
// Created by Larry Zeng on 10/10/20.
//

#ifndef PZIP_LOCKS_H
#define PZIP_LOCKS_H

#include <pthread.h>


extern pthread_mutex_t task_queue_lock;
extern pthread_cond_t task_queue_empty;
extern pthread_cond_t task_queue_filled;


extern pthread_mutex_t write_queue_lock;
extern pthread_cond_t write_queue_empty;
extern pthread_cond_t write_queue_filled;


extern pthread_mutex_t global_write_queue_lock;
extern pthread_cond_t global_write_queue_cond;

#endif //PZIP_LOCKS_H
