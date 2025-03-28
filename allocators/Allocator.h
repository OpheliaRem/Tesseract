#ifndef ALLOCATOR
#define ALLOCATOR

#include "TreeForAlloc.h"
#include "type_definitions.h"

#define HEAP_SIZE 131072

static AllocatorBinaryTree tree;
static ubyte heap[HEAP_SIZE];
static int heap_index = 0;

void COMPLEX_ALLOCATOR_configure_heap() {
    construct_alloc_tree(&tree, 256);
}

void* COMPLEX_ALLOCATOR_allocate(int size) {

    if (heap_index + size >= HEAP_SIZE) {
        return 0;
    }
    //TODO: finish this;
}


#endif