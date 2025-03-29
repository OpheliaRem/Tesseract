#ifndef OS_STRING
#define OS_STRING

#include "../../simple_types/boolean.h"

boolean assert_strings_equal(const char* a, const char* b);
void clean_char_buffer(char* buffer, int size);
int strlen(char* str);

boolean assert_strings_equal(const char* a, const char* b) {
    for (int i = 0; a[i] != '\0' && b[i] != '\0'; ++i) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}

void clean_char_buffer(char* buffer, int size) {
    for (int i = 0; i < size; ++i) {
        buffer[i] = '\0';
    }
}

int strlen(char* str) {
    int len = 0;
    for (int i = 0; str[i] != '\0'; ++i) {
        len++;
    }
    return len;
}

#endif