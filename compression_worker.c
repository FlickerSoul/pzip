//
// Created by Larry Zeng on 10/10/20.
//

#include "compression_worker.h"

write_data_t* compress_data(int fd) {
    
    char buffer[CHUNK_SIZE];
    int buffer_length = 0;

    buffer_length = read(fd, buffer, CHUNK_SIZE);

    if (buffer_length != CHUNK_SIZE) {
        buffer[buffer_length] = EOF;
    }

    // buffer_length either = chunk_size or points at EOF
    // that is valid char is 1 offset before buffer length
    int array_counter = 0;

    int first_char = buffer[0];
    uint32_t first_count = 0;
	// assert(first_char != EOF);

    int last_char = 0;
    uint32_t last_count = 0;

    void* chunk = NULL;
    int data_chunk_num = 0;

	
	if (first_char != EOF) {

		for (first_count = 0; buffer[array_counter] == first_char; array_counter++) {
			// fputc(buffer[array_counter], stdout);
			first_count++;
			// empty
		}

		if (buffer[array_counter] != EOF) {
			chunk = malloc(WRITE_CHUNK_SIZE * CHUNK_SIZE);
			void* temp = chunk;

			uint32_t c_count = 0;
			int c, last = buffer[array_counter];

			while (array_counter < buffer_length && last != EOF) {
				for (c_count = 0; (c = buffer[array_counter]) == last; c_count++, array_counter++) {
					// empty
				}

				*(uint32_t*)temp = c_count;
				temp += UINT32_SIZE;
				*(char*)temp = last;
				temp += CHAR_SIZE;

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
	}

    write_data_t* data = create_write_data(chunk, first_char, first_count, last_char, last_count, data_chunk_num);

    // return data;

    return data;
}


void* compression_worker(void* args) {
    char* cached_file_name = NULL;
    int fd = -1;
    
    task_node_t* task_node;

    while (1) {
        pthread_mutex_lock(&task_queue_lock);

        while (global_task_queue->count == 0) {
            if (global_task_queue -> end) {
                pthread_mutex_unlock(&task_queue_lock);
                if (fd != -1) {
                    close(fd);
                }
                return NULL;
            }
            pthread_cond_wait(&task_queue_filled, &task_queue_lock);
        }

        task_node = get_task();

        pthread_cond_signal(&task_queue_empty);
        pthread_mutex_unlock(&task_queue_lock);

        if (cached_file_name != task_node->file_name) {
            if (fd != -1) {
                close(fd);
            }
            cached_file_name = task_node->file_name;
            fd = open(cached_file_name, O_RDONLY);
        }
        assert(fd != -1);

        long offset = task_node->file_position * CHUNK_SIZE;
        assert(lseek(fd, offset, SEEK_SET) != -1);

        write_data_t* data = compress_data(fd);

        pthread_mutex_lock(&write_queue_lock);
        put_data(data, task_node->write_data_queue_position);
        pthread_cond_signal(&write_queue_filled);
        pthread_mutex_unlock(&write_queue_lock);
        destroy_task_node(&task_node);
    }

    if (fd != -1) {
        close(fd);
    };
    return NULL;
}

