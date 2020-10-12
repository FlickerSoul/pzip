//
// Created by Larry Zeng on 10/10/20.
//

#ifndef PZIP_COMPRESSION_WORKER_H
#define PZIP_COMPRESSION_WORKER_H

#include <stdio.h>
#include "locks.h"
#include "task_queue.h"
#include "constants.h"

write_data_t* compress_data(FILE* file, unsigned long long qp);
void* compression_worker(void* args);

#endif //PZIP_COMPRESSION_WORKER_H
