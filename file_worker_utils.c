//
// Created by Larry Zeng on 10/11/20.
//

#include "file_worker_utils.h"


unsigned int get_chunk_num(char* file_name) {
    FILE* file = fopen(file_name, "r");

    fseek(file, 0L, SEEK_END);
    long file_size = ftell(file);

    unsigned int chunk_size = ceil((double) file_size / CHUNK_SIZE);

    fclose(file);

    return chunk_size;
}
