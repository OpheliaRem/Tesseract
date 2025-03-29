#ifndef COMMAND_CONFIGURER
#define COMMAND_CONFIGURER

#include "../custom_types/data_structures/hash_map/HashMapStrStr.h"

HashMapStrStr map_of_terminal_commands;

void configure_commands();
String clear_command_info();
String echo_command_info();
String creators_command_info();
String ram_command_info();

void configure_commands() {

    construct_hash_map_str_str(&map_of_terminal_commands);

    add_in_hash_map_str_str(
        &map_of_terminal_commands,
        string_from("clear"), clear_command_info()
    );
    add_in_hash_map_str_str(
        &map_of_terminal_commands,
        string_from("echo"), echo_command_info()
    );
    add_in_hash_map_str_str(
        &map_of_terminal_commands,
        string_from("creators"), creators_command_info()
    );
    add_in_hash_map_str_str(
        &map_of_terminal_commands,
        string_from("ram"), ram_command_info()
    );
}

String clear_command_info() {
    return string_from("clear command clears the current terminal making it look brand new");
}

String echo_command_info() {
    return string_from("echo command prints the text inputted after the first space symbol");
}

String creators_command_info() {
    String str = concatenate_char_seq(
        "creators command prints the names of developers contributed",
        " to this OS for you to enjoy it!"
    );
    return str;
}

String ram_command_info() {
    return string_from("");
}

#endif