#ifndef CONVERT
#define CONVERT

#include "allocators/Allocator.h"
#include "type_definitions.h"
#include "os_string.h"


char map_digit_to_char(char);
void int_to_string(int, char*, int);
void inverse_string(char*, char*, int);
void inverse_string_with_bounds(char*, char*);
void retrieve_digits_from_number(int, int, char*);
int get_number_of_digits(int);
int string_to_int(char* string);

char map_digit_to_char(char digit) {
    return "0123456789ABCDEF"[digit];
}

ubyte map_char_to_digit(char symbol) {
    ubyte digits[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    if (symbol < 48 || symbol > 57) {
        return -12;
    }

    return digits[symbol - 48];
}

void int_to_string(int num, char* buffer, int buffer_size) { 
    int number_of_digits = get_number_of_digits(num);

    if (buffer_size < number_of_digits + 1) {
        return;
    }

    clean_char_buffer(buffer, buffer_size);

    char* digits = (char*)allocate(number_of_digits * sizeof(char));

    if (!digits) {
        return;
    }

    retrieve_digits_from_number(num, number_of_digits, digits);    

    for (short i = 0; i < number_of_digits; ++i) {
        buffer[i] = map_digit_to_char(digits[i]);
    }
    buffer[number_of_digits] = '\0';

    free(digits);
}

void inverse_string(char* source, char* destination, int number_of_bytes) {
    for (int i = 0; i < number_of_bytes; ++i) {
        destination[i] = source[number_of_bytes - 1 - i];
    }
}

void retrieve_digits_from_number(int num, int number_of_digits, char* digits) {
    char* inverted_digits = (char*)allocate(number_of_digits * sizeof(char));

    for (int i = 0; i < number_of_digits; ++i) {
        inverted_digits[i] = num % 10;
        num /= 10;
    }

    inverse_string(inverted_digits, digits, number_of_digits);

    free(inverted_digits);
}

int get_number_of_digits(int num) {
    if (num == 0) {
        return 1;
    }
    int number_of_digits = 0;
    while(num != 0) {
        num /= 10;
        ++number_of_digits;
    }
    return number_of_digits;
}


int string_to_int(char* string) {
    int number = 0;
    int power = 1;

    char inverted_string[32];
    clean_char_buffer(inverted_string, 32);
    inverse_string_with_bounds(string, inverted_string);
    
    for (int i = 0; inverted_string[i] != '\0'; ++i) {
        number += map_char_to_digit(inverted_string[i]) * power;
        power *= 10;
    }

    return number;
}

void inverse_string_with_bounds(char* src, char* dest) {

    int src_size = strlen(src);

    for (int i = 0; i < src_size; ++i) {
        dest[i] = src[src_size - 1 - i];
    }
}

#endif