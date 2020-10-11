//
// Created by Larry Zeng on 10/10/20.
//

#ifndef PZIP_COMPRESSION_WORKER_H
#define PZIP_COMPRESSION_WORKER_H

#include "locks.h"
#include "task_queue.h"
#include "constants.h"

void* compression_worker(void* args);

#endif //PZIP_COMPRESSION_WORKER_H
