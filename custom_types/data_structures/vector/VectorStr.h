#ifndef VECTOR_STR
#define VECTOR_STR

#include "../strings/dynamic_str/String.h"
#include "../allocators/Allocator.h"

typedef struct {
    String* array;
    int size;
    int capacity;
} VectorStr;

void construct_vector_str(VectorStr* vector, const int capacity) {
    vector->capacity = capacity;
    vector->size = 0;
    vector->array = (String*)allocate(capacity * sizeof(String));
}

void add_in_vector_str_by_moving(VectorStr* vector, String str) {
    vector->array[vector->size++] = str;

    if (vector->size == vector->capacity) {
        vector->capacity *= 2;
        String* array = (String*)allocate(vector->capacity * sizeof(String));
        for (int i = 0; i < vector->size; ++i) {
            array[i] = vector->array[i];
        }
        free(vector->array);
        vector->array = array;
    }
}

String get_str_at(const VectorStr* vector, int index) {
    if (index >= vector->size) {
        return string_from("");
    }

    return vector->array[index];
}

void free_vector_str(VectorStr* vector) {

    for (int i = 0; i < vector->size; ++i) {
        free_string(&vector->array[i]);
    }

    vector->capacity = 0;
    vector->size = 0;
    free(vector->array);
}

void vector_str_foreach(const VectorStr* vector, void(*action)(String*)) {
    for (int i = 0; i < vector->size; ++i) {
        action(&vector->array[i]);
    }
}

#endif