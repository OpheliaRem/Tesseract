#ifndef TERMINAL_OUTPUT
#define TERMINAL_OUTPUT

#include "Console.h"

#define WIDTH 80
#define HEIGHT 25
#define NUMBER_OF_BYTES_REPRESENTING_CHAR 2
const unsigned int STRING_LENGTH = WIDTH * NUMBER_OF_BYTES_REPRESENTING_CHAR;

extern void outb(unsigned short port, unsigned char value);

void clear_terminal(Console* console);
void print(const char* message, Console* console);
void print_byte(char byte, Console* console);
void insert_new_line(Console* console);
void println(const char* message, Console* console);
void clear_previous_byte(Console* console);
char* get_video_memory_ptr();
void move_cursor(Console* console);
void move_cursor_forward(Console* console);
void move_cursor_back(Console* console);
void move_cursor_to_start_postion(Console* console);
void move_cursor_next_line(Console* console);
void move_cursor_previos_line(Console* console);

void clear_terminal(Console* console) {
    
    unsigned int j = 0;

    char* video_memory_ptr = get_video_memory_ptr();

    while (j < WIDTH * HEIGHT * NUMBER_OF_BYTES_REPRESENTING_CHAR) {
        video_memory_ptr[j] = '\0';
        video_memory_ptr[j + 1] = 0x07;
        j += 2;
    }

    move_cursor_to_start_postion(console);
}

void print(const char* message, Console* console) {
    unsigned int j = 0;

    while (message[j] != '\0') {
        print_byte(message[j], console);    
        ++j;
    }
}

void print_byte(char byte, Console* console) {
    console->video_ptr[0] = byte;
    console->video_ptr[1] = *(console->current_color);
    console->video_ptr += 2;

    move_cursor_forward(console);
}

void insert_new_line(Console* console) {
    int used_space_of_line = (console->video_ptr- get_video_memory_ptr()) % STRING_LENGTH;
    int endline_remainder = STRING_LENGTH - used_space_of_line;
    console->video_ptr += endline_remainder;

    move_cursor_next_line(console);
}

void println(const char* message, Console* console) {
    print(message, console);
    insert_new_line(console);
}

void clear_previous_byte(Console* console) {
    if (console->video_ptr == get_video_memory_ptr()) {
        return;
    }

    console->video_ptr[-1] = console->user_color_of_text;
    console->video_ptr[-2] = 0;

    console->video_ptr -= 2;

    move_cursor_back(console);
}

char* get_video_memory_ptr() {
    return (char*)0xb8000;
}

void move_cursor(Console* console) {

    int x = console->cursor.x;
    int y = console->cursor.y;

    unsigned short position = y * WIDTH + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
}

void move_cursor_forward(Console* console) {
    console->cursor.x += 1;
    move_cursor(console);
}

void move_cursor_back(Console* console) {
    console->cursor.x -= 1;
    move_cursor(console);
}

void move_cursor_to_start_postion(Console* console) {
    console->cursor.x = 0;
    console->cursor.y = 0;

    move_cursor(console);
}

void move_cursor_next_line(Console* console) {
    console->cursor.x = 0;
    console->cursor.y += 1;

    move_cursor(console);
}

void move_cursor_previos_line(Console* console) {
    console->cursor.x = WIDTH;
    console->cursor.y -= 1;

    move_cursor(console);
}

#endif