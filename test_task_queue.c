//
// Created by Larry Zeng on 10/4/20.
//

#include "task_queue.h"
#include "test_utils.h"
#include <string.h>

void test_task_node_create_and_delete() {
    char* chunk_content = "abcde";
    int position = 0;
    char* chunk = strdup(chunk_content);
    task_node_t* task_node_ptr = create_task_node(chunk, position);

    compare_string(task_node_ptr->chunk, chunk_content);
    assert(task_node_ptr->write_data_queue_position == position);

    destroy_task_node(&task_node_ptr);
}

void test_data_composition() {
    void* test_data = malloc(sizeof(int) + sizeof(char));
    void* counter = test_data;
    printf("%p\n", counter);

    *(uint32_t*)counter = 38;
    counter += sizeof(int);
    printf("%p\n", counter);

    *(char*)counter = 'a';
    counter += 1;
    printf("%p\n", counter);

    printf("%p\n", test_data);
    FILE* f = fopen("data_composition_out.o", "w");
    fwrite(test_data, sizeof(int) + sizeof(char), 1, f);
    fclose(f);

    free(test_data);
}

void test_write_data_create_and_delete() {
}

void test_enqueue() {

}

void test_parallel_enqueue() {

}

int main() {
    test_task_node_create_and_delete();
    test_data_composition();
}
