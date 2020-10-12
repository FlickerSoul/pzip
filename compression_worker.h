//
// Created by Larry Zeng on 10/10/20.
//

#ifndef PZIP_COMPRESSION_WORKER_H
#define PZIP_COMPRESSION_WORKER_H

#include <stdio.h>
#include "locks.h"
#include "task_queue.h"
#include "constants.h"
#include <unistd.h>
#include <fcntl.h>

write_data_t* compress_data(int fd);
void* compression_worker(void* args);

#endif //PZIP_COMPRESSION_WORKER_H
