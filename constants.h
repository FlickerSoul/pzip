//
// Created by Larry Zeng on 10/10/20.
//

#ifndef PZIP_CONSTANTS_H
#define PZIP_CONSTANTS_H

#include <assert.h>

unsigned int CHUNK_SIZE = 4096;
unsigned int DEFAULT_TASK_QUEUE_SIZE = 20;

const int INT_SIZE = sizeof(int);
const int UINT32_SIZE = sizeof(uint32_t);
const int CHAR_SIZE = sizeof(char);
const int WRITE_CHUNK_SIZE = UINT32_SIZE + CHAR_SIZE;

#endif //PZIP_CONSTANTS_H
