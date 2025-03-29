#ifndef CONSOLE
#define CONSOLE

#include "UserInteractionMode.h"
#include "colors.h"
#include "Cursor.h"

typedef struct {
    char* video_ptr;
    UserInteractionMode user_interaction_mode;
    char kernel_color_of_text;
    char user_color_of_text;
    char* current_color;
    Cursor cursor;
} Console;

#endif