#include "terminal/terminal_output.h"
#include "interrupt_handler/keyboard_map.h"
#include "terminal/colors.h"
#include "interrupt_handler/idt.h"
#include "interrupt_handler/input_info_parser.h"
#include "terminal/UserInteractionMode.h"
#include "terminal/Console.h"
#include "convert/convert.h"
#include "allocators/Allocator.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

Console console;

void keyboard_handler_main() {
    unsigned char status;
    char keycode;

    write_port(0x20, 0x20);

    status = read_port(KEYBOARD_STATUS_PORT);

    if (status & 0x01) {
        keycode = read_port(KEYBOARD_DATA_PORT);
        if (keycode < 0) {  
            return;
        }

        define_interrupt(keycode, &console);
    }
}

void kmain(void) {

    configure_heap();

    console.video_ptr = get_video_memory_ptr();
    console.kernel_color_of_text = COLOR_GREEN;
    console.user_color_of_text = COLOR_LIGHT_GRAY;
    console.user_interaction_mode = UIM_GREETING;
    console.current_color = &(console.kernel_color_of_text);
    console.cursor.x = 0;
    console.cursor.y = 0;

    clear_terminal(&console);

    println("Welcome to Tesseract: free and open-source operating system", &console);
    println("Please press 'Enter' to open terminal mode", &console);

    idt_init();
    keyboard_init();

    while(1);
}