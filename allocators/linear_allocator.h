#ifndef ALLOCATOR_SIMPLE
#define ALLOCATOR_SIMPLE

#include "../type_definitions.h"

#define HEAP_SIZE 32768

static ubyte _linear_heap[HEAP_SIZE];
static int _linear_heap_index = 0;

void* allocate_linear(int size) {
    if (_linear_heap_index + size > HEAP_SIZE) {
        return 0;
    }

    void* new_address = _linear_heap + _linear_heap_index;
    _linear_heap_index += size;
    return new_address;
}

void linear_free(void* ptr) {

}

#endif