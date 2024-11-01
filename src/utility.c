#include "../include/utility.h"
#include "../include/global.h"
// #define RGB_COLOR(r, g, b) "\033[38;2;" #r ";" #g ";" #b "m" // Text Colour
// #define BG_RGB_COLOR(r, g, b) "\033[48;2;" #r ";" #g ";" #b "m" // Background
// Colour #define BOLD "\033[1m" #define UNDERLINE "\033[4m" #define RESET
// "\033[0m"

// Example predefined colors
//~ Uses Catpuccin Latte colours
const char *RED = RGB_COLOR(210, 15, 57);
const char *GREEN = RGB_COLOR(64, 160, 43);
const char *BLUE = RGB_COLOR(30, 102, 245);
const char *YELLOW = RGB_COLOR(223, 142, 29);
const char *PINK = RGB_COLOR(234, 118, 203);

const char *BG_RED = BG_RGB_COLOR(210, 15, 57);
const char *BG_GREEN = BG_RGB_COLOR(64, 160, 43);
const char *BG_BLUE = BG_RGB_COLOR(30, 102, 245);
const char *BG_YELLOW = BG_RGB_COLOR(223, 142, 29);
const char *BG_PINK = BG_RGB_COLOR(234, 118, 203);

/*
Utility Functions
*/
void print_colored_text(const char *text, const char *color,
                        const char *bg_colour, const char *style) {
  printf("%s%s%s%s%s", color, bg_colour, style, text, RESET);
}

void println_colored_text(const char *text, const char *bg_colour,
                          const char *color, const char *style) {
  printf("%s%s%s%s%s\n", color, bg_colour, style, text, RESET);
}

int count_args(char **args) {
  int count = 0;
  while (args[count] != NULL) {
    count++;
  }
  return count;
}

//? change naming as it sounds like a bool
int are_number_of_args_correct(char **args, int wanted_number_of_args,
                               char *command_usage) {
  int num_of_args = count_args(args);
  if (num_of_args < wanted_number_of_args) {
    fprintf(stderr, "Error. Too few arguments\n");
    fprintf(stderr, "%s", command_usage);
    return CONTINUE;
  } else if (num_of_args > wanted_number_of_args) {
    fprintf(stderr, "Error. Too many arguments\n");
    fprintf(stderr, "%s", command_usage);
    return CONTINUE;
  }
  return 0;
}

char* append(char* string, char c) {
  size_t size = strlen(string);
  char* string_size = malloc(size + 2); //? idk why the +2
  strcpy(string_size, string);
  string_size[size] = c;
  string_size[size + 1] = '\0';
  return string_size;
}

char *get_home_dir() {
  const char *home_dir;
#ifdef _WIN32
  home_dir = getenv("USERPROFILE");
#else
  home_dir = getenv("HOME");
#endif
  if (home_dir == NULL) {
    fprintf(stderr, "ash: Error finding home directory\n");
    return NULL;
  }
  return home_dir;
}