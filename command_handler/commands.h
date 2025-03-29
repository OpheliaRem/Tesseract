#ifndef COMMANDS
#define COMMANDS

#include "../terminal/terminal_output.h"
#include "../terminal/Console.h"
#include "../creators.h"
#include "command_parser.h"
#include "../allocators/Allocator.h"
#include "../os_string.h"
#include "../convert.h"

typedef struct {
    char* name;
    unsigned char number_of_args;
    char* arg0;
} Command;


void clear(Console*);
void echo(Console*);
void creators(Console*);
void ram(Console*, const char*);
void ram_write(Console*);
void ram_read(Console*);
void ram_clear(Console*);

char* read_dynamic_string_from_console(Console*);
int count_bytes(char*);

void clear(Console* console) {
    clear_terminal(console);
    console->video_ptr = get_video_memory_ptr();
}

void echo(Console* console) {

    char* line_ptr = console->video_ptr;
    insert_new_line(console);

    char buffer[256];
    for (int i = 0; i < 256; ++i) {
        buffer[i] = '\0';
    }

    line_ptr += 2;
    for (int i = 0; i < 256 && *line_ptr != '\0'; ++i) {
        buffer[i] = *line_ptr;
        line_ptr += 2;
    }

    console->current_color = &(console->kernel_color_of_text);
    println(buffer, console);
    console->current_color = &(console->user_color_of_text);
}

void creators(Console* console) {
    insert_new_line(console);

    char color = COLOR_MAGENTA;
    console->current_color = &color;
    println(creator, console);
    console->current_color = &(console->user_color_of_text);
}

void ram(Console* console, const char* arg) {

    if (assert_strings_equal("write", arg)) {
        return ram_write(console);
    }

    if (assert_strings_equal("read", arg)) {
        return ram_read(console);
    }

    if (assert_strings_equal("clear", arg)) {
        return ram_clear(console);
    }
}

void ram_write(Console* console) {
    console->video_ptr += 2;

    int number_of_bytes = count_bytes(console->video_ptr) / 2;

    char* buffer = (char*)allocate(number_of_bytes + 1);
    for (int i = 0; i < number_of_bytes; ++i) {
        buffer[i] = *(console->video_ptr);
        console->video_ptr += 2;
    }
    buffer[number_of_bytes] = '\0';

    char number_of_bytes_STR[128];
    int_to_string(number_of_bytes, number_of_bytes_STR, 128);

    char address_str[32];
    int_to_string((int)buffer, address_str, 32);

    insert_new_line(console);
    print(number_of_bytes_STR, console);
    print(" bytes '", console);
    print(buffer, console);
    print("' were written to the address: ", console);
    println(address_str, console);
}

void ram_read(Console* console) {
    console->video_ptr += 2;

    char* address_buffer = read_dynamic_string_from_console(console);

    ubyte* address = (ubyte*)string_to_int(address_buffer);

    insert_new_line(console);
    println(address, console);

    free(address_buffer);
}

void ram_clear(Console* console) {
    console->video_ptr += 2;

    char* address_buffer = read_dynamic_string_from_console(console);

    ubyte* address = (ubyte*)string_to_int(address_buffer);

    free(address);

    insert_new_line(console);
    println("The memory was successfully freed", console);

    free(address_buffer);
}

char* read_dynamic_string_from_console(Console* console) {
    int number_of_bytes = count_bytes(console->video_ptr) / 2;

    char* buffer = (char*)allocate(number_of_bytes + 1);
    for (int i = 0; i < number_of_bytes; ++i) {
        buffer[i] = *(console->video_ptr);
        console->video_ptr += 2;
    }
    buffer[number_of_bytes] = '\0';
    return buffer;
}

int count_bytes(char* ptr) {
    int number_of_bytes = 0;
    while(*(ptr++) != '\0') {
        ++number_of_bytes;
    }
    return number_of_bytes;
}

#endif