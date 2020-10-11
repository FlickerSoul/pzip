//
// Created by Larry Zeng on 10/10/20.
//

#ifndef PZIP_FILE_WORKER_H
#define PZIP_FILE_WORKER_H

#include "constants.h"
#include "task_queue.h"
#include "locks.h"
#include "file_worker_utils.h"

void* file_reader(void* args);
void output(write_data_t* data);
void* file_writer(void* args);


#endif //PZIP_FILE_WORKER_H
