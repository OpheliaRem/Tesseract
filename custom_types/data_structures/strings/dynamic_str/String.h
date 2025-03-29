#ifndef DYNAMIC_STRING_IMPLEMENTATION
#define DYNAMIC_STRING_IMPLEMENTATION

#include "../os_string.h"
#include "../../../../allocators/Allocator.h"
#include "../../../simple_types/boolean.h"

typedef struct {
    char* symbol_sequence;
    int size;
} String;

String string_from(const char*);
void free_string(String*);
void dynamic_string_foreach(String*, void(*)(char*));
void char_sequence_from_string(char*, int, const String*);
String concatenate_char_seq(char*, char*);
String concatenate_and_kill_args(String*, String*);
String make_deep_copy(const String*);
boolean strings_are_equal(const String* a, const String* b);


String string_from(const char* symbol_sequence) {
    String string;
    string.size = strlen(symbol_sequence);
    string.symbol_sequence = (char*)allocate(string.size + 1);

    for (int i = 0; i < string.size; ++i) {
        string.symbol_sequence[i] = symbol_sequence[i];
    }

    string.symbol_sequence[string.size] = '\0';

    return string;
}

void free_string(String* string) {
    string->size = 0;
    free(string->symbol_sequence);
}

void dynamic_string_foreach(String* string, void(*action)(char*)) {
    for (int i = 0; i < string->size; ++i) {
        action(&string->symbol_sequence[i]);
    }
}

void char_sequence_from_string(char* buffer, int size, const String* string) {
    if (size <= string->size) {
        return;
    }

    for (int i = 0; i < string->size; ++i) {
        buffer[i] = string->symbol_sequence[i];
    }
    buffer[string->size] = '\0';
}

String concatenate_char_seq(char* a, char* b) {
    String str;

    int len_a = strlen(a);
    int len_b = strlen(b);

    str.size = len_a + len_b;
    str.symbol_sequence = (char*)allocate(str.size + 1);

    int i = 0;
    while (i < len_a) {
        str.symbol_sequence[i] = a[i];
        i++;
    }
    while(i - len_a < len_b) {
        str.symbol_sequence[i] = b[i - len_a];
        i++;
    }
    str.symbol_sequence[i] = '\0';

    return str;
}

String concatenate_and_kill_args(String* a, String* b) {
    String str = concatenate_char_seq(a->symbol_sequence, b->symbol_sequence);
    free(a);
    free(b);
    return str;
}

String make_deep_copy(const String* str) {
    return string_from(str->symbol_sequence);
}

boolean strings_are_equal(const String* a, const String* b) {
    if (a->size != b->size) {
        return FALSE;
    }

    for (int i = 0; i < a->size; ++i) {
        if (a->symbol_sequence[i] != b->symbol_sequence[i]) {
            return FALSE;
        }
    }

    return TRUE;
}

#endif