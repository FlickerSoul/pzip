//
// Created by Larry Zeng on 10/10/20.
//

#ifndef PZIP_FILE_WORKER_H
#define PZIP_FILE_WORKER_H

#include "constants.h"
#include "task_queue.h"
#include "locks.h"
#include <stdio.h>

void* file_reader(void* args);


#endif //PZIP_FILE_WORKER_H
