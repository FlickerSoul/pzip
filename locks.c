#include "locks.h"

pthread_mutex_t task_queue_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t task_queue_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t task_queue_filled = PTHREAD_COND_INITIALIZER;


pthread_mutex_t write_queue_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t write_queue_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t write_queue_filled = PTHREAD_COND_INITIALIZER;

pthread_mutex_t global_write_queue_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t global_write_queue_cond = PTHREAD_COND_INITIALIZER;
