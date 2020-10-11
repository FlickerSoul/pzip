//
// Created by Larry Zeng on 10/10/20.
//

#include "compression_worker.h"
#include "stdio.h"


void* compression_worker(void* args) {
    while (1) {
        task_node_t* task_node;

        pthread_mutex_lock(&task_queue_lock);
        while (gloabl_task_queue->count == 0) {
            if (gloabl_task_queue->end) {
                return NULL;
            }
            pthread_cond_wait(&task_queue_filled, &task_queue_lock);
        }

        task_node = get_task();

        pthread_cond_signal(&task_queue_empty);
        pthread_mutex_unlock(&task_queue_lock);

        FILE* file = fopen(task_node->file_name, "r");

        long offset = task_node->file_position * CHUNK_SIZE;
        fseek(file, offset, SEEK_SET);

        void* chunk = NULL;
        int data_chunk_num = 0;

        int first_char, last_char = 0;
        uint32_t first_count, last_count = 0;

        // first char
        int c, last = first_char = fgetc(file);
        int c_count = first_count = 0;

        assert(last != EOF);

        for (first_count = 1; (c = fgetc(file)) == last; first_count++) {
            // empty
        }

        last = c;

        if (last != EOF) {
            chunk = malloc(WRITE_CHUNK_SIZE * CHUNK_SIZE);
            void* temp = chunk;

            while (last != EOF) {
                for (c_count = 1; (c = fgetc(file)) == last; c_count++) {
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

        write_data_t* data = create_write_data(chunk, first_char, first_count, last_char, last_count);
        pthread_mutex_lock(&write_queue_lock);
        put_data(data, task_node->write_data_queue_position);
        pthread_cond_signal(&write_queue_filled);
        pthread_mutex_unlock(&write_queue_lock);

        destroy_task_node(&task_node);
        fclose(file);

        return NULL;
    }
}

