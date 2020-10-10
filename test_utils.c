#include "test_utils.h"

bool compare_string(const char* str1, const char* str2) {
    if (str1 == NULL || str2 == NULL || strcmp(str1, str2)) {
        printf("s1: %s, s2: %s\n", str1, str2);
        return 0;
    }
    return true;
}
