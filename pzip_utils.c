#include "pzip_utils.h"


void get_thread_num() {
    char* thread_from_env = getenv("NTHREADS");
    if (thread_from_env == NULL) {
        THREAD_NUM  = 1;
    } else {
        THREAD_NUM = atoi(thread_from_env);
    }
}