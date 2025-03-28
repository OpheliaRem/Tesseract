#ifndef INPUT_INFO_PARSER
#define INPUT_INFO_PARSER

#include "../terminal/terminal_output.h"
#include "keyboard_map.h"
#include "../UserInteractionMode.h"
#include "../terminal/colors.h"
#include "../important_symbols.h"
#include "../command_handler/command_parser.h"

void define_interrupt(char, Console*);
void backspace(Console*);
void print_prompt(Console*);
void print_new_prompt(Console*);
void greeting_mode_define_interrupt(char, Console*);
void terminal_mode_define_interrupt(char, Console*);

void define_interrupt(char key, Console* console) {
    switch (console->user_interaction_mode)
    {
    case UIM_GREETING:
        return greeting_mode_define_interrupt(key, console);
    case UIM_COMMANDLINE:
        return terminal_mode_define_interrupt(key, console);
    default:
        return;
    }
}

void greeting_mode_define_interrupt(char key, Console* console) {
    if (keyboard_map[key] != '\n') {
        return;
    }

    clear_terminal(console);
    console->user_interaction_mode = UIM_COMMANDLINE;
    console->video_ptr = get_video_memory_ptr();
    print_prompt(console);
}

void terminal_mode_define_interrupt(char key, Console* console) {
    switch (keyboard_map[key])
    {
    case '\b':
        return backspace(console);
    
    case '\n':
        parse_and_execute_command(console);
        return print_new_prompt(console);

    default:
        return print_byte(keyboard_map[key], console);
    }
}

void print_new_prompt(Console* console) {
    if (console->video_ptr != get_video_memory_ptr()) {
        insert_new_line(console);
    }
    print_prompt(console);
}

void backspace(Console* console) {
    if (console->video_ptr[-4] == PROMPT_SYMBOL) {
        return;
    }

    clear_previous_byte(console);
}

void print_prompt(Console* console) {
    console->current_color = &(console->kernel_color_of_text);
    print("user:", console);
    console->current_color = &(console->user_color_of_text);
    print_byte(PROMPT_SYMBOL, console);
    print(" ", console);
}

#endif