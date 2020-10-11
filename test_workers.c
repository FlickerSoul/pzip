//
// Created by Larry Zeng on 10/11/20.
//

#include "file_worker_utils.h"
#include "compression_worker.h"

void test_get_chunk_num() {
    unsigned int chunk_num = get_chunk_num("tests/1.in");
    assert(chunk_num == 1);
}

void test_simple_compress_data() {
    FILE* f = fopen("tests/1.in", "r");
    write_data_t* data = compress_data(f);
    fclose(f);

    assert(data->first_char == 'a');
    assert(data->first_count == 38);

    assert(data->main_data == NULL);
    assert(data->data_chunk_num == 0);
    assert(data->last_count == 0);
}

void test_complex_compress_data() {
    FILE* f = fopen("tests/4.in", "r");
    write_data_t* data = compress_data(f);
    fclose(f);

    assert(data->first_char == 'a');
    assert(data->first_count == 58);

    assert(data->main_data != NULL);
    assert(data->data_chunk_num == 8);

    void* temp = data->main_data;
    uint32_t counts[8] = {1, 31, 1, 20, 1, 31, 1, 377};
    char chars[8] = {'\n', 'b', '\n', 'c', '\n', 'd', '\n', 'e'};
    for (int i = 0; i < data->data_chunk_num; i++) {
        assert(*(uint32_t*)(temp) == counts[i]);
        temp += sizeof(uint32_t);
        assert(*(char*)(temp) == chars[i]);
        temp += sizeof(char);
    }

    assert(data->last_char == '\n');
    assert(data->last_count == 1);
}

int main() {
    test_get_chunk_num();
    test_simple_compress_data();
    test_complex_compress_data();
    return 0;
}
