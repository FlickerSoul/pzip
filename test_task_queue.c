//
// Created by Larry Zeng on 10/4/20.
//

#include "task_queue.h"
#include "test_utils.h"
#include <string.h>

void test_task_node_create_and_delete() {
    char* chunk_content = "abcde";
    int queue_position = 0;
    int file_position = 0;
    char* chunk = strdup(chunk_content);
    task_node_t* task_node_ptr = create_task_node(chunk, file_position, queue_position);

    compare_string(task_node_ptr->file_name, chunk_content);
    assert(task_node_ptr->write_data_queue_position == queue_position);
    assert(task_node_ptr->file_position == file_position);

    destroy_task_node(&task_node_ptr);
    assert(task_node_ptr == NULL);
}

void test_data_composition() {
    void* test_data = malloc(sizeof(int) + sizeof(char));
    void* counter = test_data;

    *(uint32_t*)counter = 38;
    counter += sizeof(int);

    *(char*)counter = 'a';
    counter += 1;

    FILE* f = fopen("data_composition_out.o", "w");
    fwrite(test_data, sizeof(int) + sizeof(char), 1, f);
    fclose(f);
    
    free(test_data);
}

void* data_gen() {
    void* test_data = malloc(sizeof(int) + sizeof(char));
    void* counter = test_data;
    *(uint32_t*)counter = 38;
    counter += sizeof(int);
    *(char*)counter = 'a';

    return test_data;
}

void test_write_data_create_and_delete() {
    void* chunck = data_gen();
    char first = 'b';
    char last = 'c';
    int first_count = 10;
    int last_count = 20;
    write_data_t* write_data = create_write_data(chunck, first, first_count, last, last_count);
    assert(write_data->main_data == chunck);
    assert(write_data->first_char = first);
    assert(write_data->last_char = last);
    assert(write_data->first_count = first_count);
    assert(write_data->last_count = last_count);

    destroy_write_data(&write_data);
    assert(write_data == NULL);
}

void test_task_enqueue() {
    gloabl_task_queue = create_task_queue(2);

    assert(gloabl_task_queue->size == 2);

    char* chunk_content = "abcde";
    int file_position = 0;
    int queue_position = 0;
    char* chunk = strdup(chunk_content);
    task_node_t* task_node_ptr1 = create_task_node(chunk, file_position, queue_position);

    put_task(task_node_ptr1);
    assert(gloabl_task_queue->fill_ptr == 1);
    put_task(task_node_ptr1);
    assert(gloabl_task_queue->fill_ptr == 0);

    assert(get_task() == task_node_ptr1);
    assert(gloabl_task_queue->use_ptr == 1);
    assert(get_task() == task_node_ptr1);
    assert(gloabl_task_queue->use_ptr == 0);

    destroy_task_queue(&gloabl_task_queue);
}

void test_write_enqueue() {
    global_write_queue = create_write_queue(10);
    destroy_write_queue(&global_write_queue);
    assert(global_write_queue == NULL);
}

void test_parallel_enqueue() {

}

int main() {
    test_task_node_create_and_delete();
    test_data_composition();
    test_write_data_create_and_delete();
    test_task_enqueue();
    test_write_enqueue();
}
