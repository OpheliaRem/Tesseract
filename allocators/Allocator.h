#ifndef ALLOCATOR
#define ALLOCATOR

#include "../custom_types/simple_types/type_definitions.h"

#define HEAP_SIZE 131072

static ubyte heap[HEAP_SIZE];
static ubyte bytemap[HEAP_SIZE];

void configure_heap();
void* allocate(int);
void free(void*);
int get_bytes_used();

void configure_heap() {
    for (int i = 0; i < HEAP_SIZE; ++i) {
        bytemap[i] = 0;
    }
}

void* allocate(int size) {
    int index = 0;
    while (index < HEAP_SIZE) {
        while(index < HEAP_SIZE && bytemap[index] == 1) {
            index++;
        }
        index++;
        int begin_index = index;
    
        while(index < HEAP_SIZE && bytemap[index] == 0) {
            if (index - begin_index >= size + 1) {
                for (int i = begin_index; i < begin_index + size; ++i) {
                    bytemap[i] = 1;
                }
                bytemap[begin_index + size] = 0;
                return heap + begin_index;
            }
            index++;
        }
    }
    return 0;
}

void free(void* ptr) {

    if (!ptr) {
        return;
    }

    int index = (ubyte*)ptr - heap;

    if (index < 0 || index >= HEAP_SIZE) {
        return;
    }

    while(bytemap[index] != 0) {
        bytemap[index] = 0;
        index++;
    }
}

int get_bytes_used() {
    int counter = 0;
    for (int i = 0; i < HEAP_SIZE; ++i) {
        if (bytemap[i] == 1) {
            ++counter;
        }
    }
    return counter;
}


#endif