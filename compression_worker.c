//
// Created by Larry Zeng on 10/10/20.
//

#include "compression_worker.h"

write_data_t* compress_data(FILE* file) {
    int char_counter = CHUNK_SIZE;

    void* chunk = NULL;
    int data_chunk_num = 0;

    int first_char, last_char = 0;
    uint32_t first_count, last_count = 0;

    // first char
    int c, last = first_char = fgetc(file);
    int c_count = first_count = 0;

    char_counter -= 1;

    assert(last != EOF);

    for (first_count = 1; (c = fgetc(file)) == last; first_count++, char_counter--) {
        // empty
    }

    last = c;

    if (last != EOF) {
        chunk = malloc(WRITE_CHUNK_SIZE * CHUNK_SIZE);
        void* temp = chunk;

        while (last != EOF && char_counter != 0) {
            for (c_count = 1; (c = fgetc(file)) == last && char_counter != 0; c_count++, char_counter--) {
                // empty
            }

            *(uint32_t*)temp = c_count;
            temp += UINT32_SIZE;
            *(char*)temp = last;
            temp += 1;

            last = c;

            data_chunk_num += 1;
        }

        last_char = *(char*)(temp-CHAR_SIZE);
        last_count = *(uint32_t*)(temp-WRITE_CHUNK_SIZE);
        data_chunk_num -= 1;

        if (data_chunk_num == 0) {
            free(chunk);
            chunk = NULL;
        }
    }

    write_data_t* data = create_write_data(chunk, first_char, first_count, last_char, last_count, data_chunk_num);

    return data;
}


void* compression_worker(void* args) {
    char* cached_file_name = NULL;
    FILE* cached_file_ptr = NULL;
    
    task_node_t* task_node;

    while (1) {
        // printf("compression worker inited\n");

        pthread_mutex_lock(&task_queue_lock);
        // printf("compression got task lock\n");

        while (global_task_queue->count == 0) {
            if (global_task_queue -> end) {
                // printf("end compression\n");
                fclose(cached_file_ptr);
                return NULL;
            }

            // printf("compression worker wait\n");
            pthread_cond_wait(&task_queue_filled, &task_queue_lock);

            // printf("wake up compression\n");
        }

        task_node = get_task();

        pthread_cond_signal(&task_queue_empty);
        pthread_mutex_unlock(&task_queue_lock);

        // printf("compressiong read task\n");

        if (cached_file_name != task_node->file_name) {
            if (cached_file_name != NULL) {
                fclose(cached_file_ptr);
            }
            cached_file_ptr = fopen(task_node->file_name, "r");
            cached_file_name = task_node->file_name;
        }

        // printf("compression created file des\n");

        long offset = task_node->file_position * CHUNK_SIZE;
        fseek(cached_file_ptr, offset, SEEK_SET);

        // printf("compression file seeked\n");

        write_data_t* data = compress_data(cached_file_ptr);

        // printf("got data for %s file at position %llu at write array %llu \n", cached_file_name, task_node->file_position, task_node->write_data_queue_position);

        pthread_mutex_lock(&write_queue_lock);
        // printf("acquired lock for writing data\n");
        put_data(data, task_node->write_data_queue_position);
        // printf("write queue info after writing at position %llu\n", task_node->write_data_queue_position);
        pthread_cond_signal(&write_queue_filled);
        // printf("signaled added data\n");
        pthread_mutex_unlock(&write_queue_lock);

        destroy_task_node(&task_node);

    }

    fclose(cached_file_ptr);
    return NULL;
}

