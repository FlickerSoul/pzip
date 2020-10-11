//
// Created by Larry Zeng on 10/4/20.
//

#ifndef PZIP_PZIP_UTILS_H
#define PZIP_PZIP_UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "constants.h"
#include "file_worker.h"
#include "compression_worker.h"


unsigned int THREAD_NUM;

void get_thread_num();

#endif //PZIP_PZIP_UTILS_H
