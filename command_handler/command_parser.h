#ifndef COMMAND_PARSER
#define COMMAND_PARSER

#include "commands.h"
#include "../basic_os_info/important_symbols.h"
#include "../terminal/Console.h"
#include "../custom_types/data_structures/strings/os_string.h"


void parse_and_execute_command(Console* console);

void catch_one_arg(Console* console, char* arg_buffer, int size);

void NO_ARGS_execute_command(Console* console, Command* command);
void (*NO_ARGS_define_command(const char* name))(Console*);

void ONE_ARG_execute_command(Console* console, Command* command);
void (*ONE_ARG_define_command(const char* name))(Console*, const char*);


void parse_and_execute_command(Console* console) {

    if (console->video_ptr[-4] == PROMPT_SYMBOL) {
        return;
    }
    
    Command command;
    command.number_of_args = 0;

    while (*console->video_ptr != PROMPT_SYMBOL) {
        console->video_ptr -= 2;
        if (*console->video_ptr == '-') {
            command.number_of_args += 1;
        }
    }

    char* line_begin = console->video_ptr;

    console->video_ptr += 4;

    char buffer[32];
    clean_char_buffer(buffer, 32);

    for (int i = 0; i < 32 && *console->video_ptr != ' '; ++i) {
        buffer[i] = *console->video_ptr;
        console->video_ptr += 2;
    }

    command.name = buffer;

    char arg_buffer[32];
    clean_char_buffer(arg_buffer, 32);

    if (command.number_of_args > 0) {
        catch_one_arg(console, arg_buffer, 32);
    }

    command.arg0 = arg_buffer;

    switch(command.number_of_args) {
        case 0:
            return NO_ARGS_execute_command(console, &command);
        case 1:
            return ONE_ARG_execute_command(console, &command);
        default:
            return;
    }
}

void catch_one_arg(Console* console, char* arg_buffer, int size) {
    while(*(console->video_ptr += 2) != '-');

    for (int i = 0; i < size && *console->video_ptr != ' ' && *console->video_ptr != '\n'; ++i) {
        arg_buffer[i] = *(console->video_ptr += 2);
    }
}

void NO_ARGS_execute_command(Console* console, Command* command) {
    void(*command_func)(Console*) = NO_ARGS_define_command(command->name);

    if (command_func != 0) {
        command_func(console);
    }
}

void (*NO_ARGS_define_command(const char* name))(Console*) {

    if (assert_strings_equal("clear", name)) {
        return clear;
    }

    if (assert_strings_equal("echo", name)) {
        return echo;
    }

    if (assert_strings_equal("creators", name)) {
        return creators;
    }

    return 0;

}

void ONE_ARG_execute_command(Console* console, Command* command) {
    void(*command_func)(Console*, const char*) = ONE_ARG_define_command(command->name);

    if (command_func != 0) {
        command_func(console, command->arg0);
    }
}

void (*ONE_ARG_define_command(const char* name))(Console*, const char*) {
    if (assert_strings_equal("ram", name)) {
        return ram;
    }

    return 0;
}

#endif