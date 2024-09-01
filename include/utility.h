#ifndef UTILITY_H
#define UTILITY_H

#include <complex.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef enum {
    SUCESS = 1,
    FAILURE = 0,
} Status;



#define RGB_COLOR(r, g, b) "\033[38;2;" #r ";" #g ";" #b "m" // Text Colour
#define BG_RGB_COLOR(r, g, b) "\033[48;2;" #r ";" #g ";" #b "m" // Background Colour
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"
#define RESET "\033[0m"

// Example predefined colors
//~ Uses Catpuccin Latte colours
extern const char* RED;
extern const char* GREEN;
extern const char* BLUE;
extern const char* YELLOW;
extern const char* PINK;

extern const char* BG_RED;
extern const char* BG_GREEN;
extern const char* BG_BLUE;
extern const char* BG_YELLOW;
extern const char* BG_PINK;

void print_colored_text(const char* text, const char* color, const char* bg_colour, const char* style);
void println_colored_text(const char* text, const char* bg_colour, const char* color, const char* style);
int count_args(char** args);
int are_number_of_args_correct(char **args, int wanted_number_of_args, char* command_usage);
char* get_home_dir();
#endif