#ifndef VECTOR_INT
#define VECTOR_INT

#include "../allocators/Allocator.h"

typedef struct {
    int* array;
    int size;
    int capacity;
} VectorInt;

void construct_vector_int(VectorInt* vector, const int capacity) {
    vector->capacity = capacity;
    vector->size = 0;
    vector->array = (int*)allocate(capacity * sizeof(int));
}

void add_in_vector_int(VectorInt* vector, int num) {
    vector->array[vector->size++] = num;

    if (vector->size == vector->capacity) {
        vector->capacity *= 2;
        int* array = (int*)allocate(vector->capacity * sizeof(int));
        for (int i = 0; i < vector->size; ++i) {
            array[i] = vector->array[i];
        }
        free(vector->array);
        vector->array = array;
    }
}

int get_num_at(const VectorInt* vector, int index) {
    if (index >= vector->size) {
        return -8430927;
    }

    return vector->array[index];
}

void free_vector_int(VectorInt* vector) {
    vector->capacity = 0;
    vector->size = 0;
    free(vector->array);
}

void vector_int_foreach(const VectorInt* vector, void(*action)(int*)) {
    for (int i = 0; i < vector->size; ++i) {
        action(&vector->array[i]);
    }
}

#endif