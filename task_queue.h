//
// Created by Larry Zeng on 10/4/20.
//

#ifndef PZIP_TASK_QUEUE_H
#define PZIP_TASK_QUEUE_H

#include <pthread.h>

typedef struct _write_data_t {
    /*
     * write data info
     * write_data = malloc(data_chunk_size * (sizeof(uint32_t) + sizeof(char)));
     * data_chunk_num is the number valid chunks in write data
     *
     * first_char is the first character of this chuck
     * first_count is the count of the occurrence of the first
     *
     * last_char is the last character of this chuck
     * last_count is the count of the occurrence of the last_char
     *
     * when the write thread writes to the disk,
     * it first read the last char of the previous chunk,
     * and see if it matches the first char of this data chunk
     * if so, update the first char of this chuck,
     * if not, write the last char in the previous
     * then write first_char and write_data in the previous chunk
     *
     */
    void* write_data;
    int data_chunk_num;

    char first_char;
    uint32_t first_count;
    char last_char;
    uint32_t last_count;
} write_data_t;

typedef struct _write_queue_t {
    /*
     * write queue for write thread
     *
     * write_data_queue is an array of write data
     * queue_size is the length of the array, noted as n
     * current_work_position is from 0 to n
     * when position = n, the write thread
     * writes the first_char, write_data, and last_char
     * of n-1 data chunk
     */
    write_data_t* write_data_queue;
    unsigned long long current_work_position = 1;
    unsigned long long queue_size;

    char* file_name;
    FILE* write_descriptor;

    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
    pthread_cond_t filled = PTHREAD_COND_INITIALIZER;
} write_queue_t;

typedef struct _write_work_queue_t {
    /*
     * for write thread to keep track of works
     * it's unique in one process
     */
    write_queue_t* write_queues;
    unsigned int current_work_position;
    unsigned int work_position;
} write_work_queue_t;

typedef struct _process_queue_node_t {
    /*
     * task node contains info for a task
     * chunk is the read data from a file
     */
    char* chunk;
    write_queue_t write_target;
    unsigned long long write_data_queue_position;
} process_queue_node;

typedef struct _process_queue_t {
    /*
     * a consumer/producer queue that's used
     * to feed works to compression workers
     * it's unique in one process
     */
    process_queue_node* task_queue;

    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
    pthread_cond_t filled = PTHREAD_COND_INITIALIZER;
} process_queue;

process_queue* create_process_queue(int process_queue_size);
process_queue* quick_create_process_queue();
void destroy_process_queue(process_queue* pq);

write_work_queue_t* create_write_work_queue(int write_work_num, char* write_file_names);
void destroy_write_work_queue(write_work_queue_t* wwq);

write_queue_t* create_write_queue(int queue_size, char* file_name, FILE* write_descriptor);
void destroy_write_queue(write_queue_t* wq);



#endif //PZIP_TASK_QUEUE_H
