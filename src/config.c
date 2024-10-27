#include "../include/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utility.h>
#include "../include/global.h"
#define SEPARATOR '='
#define MAX_LINES 1024       // Probably a lot
#define MAX_LINE_LENGTH 1024 // Probably too much


const char CONFIG_FILENAME[] = "/.ash_config";
const char DEFAULT_CONFIG_FILE_CONTENTS[] =
    "# ash config file\n"
    "# In this file, we define the default KEYS and their VALUES you can use "
    "to customise ash\n"
    "# All the comments are single lines cuz i cba to inlcude multi-line in "
    "the parser :)\n"
    "# I will include descriptions and explanations of the KEYS above their "
    "definition\n"
    "\n"
    "# Change the Arrow colour\n"
    "# \t Following accepted (case does matter): [Red, Orange, Yellow, Brown, "
    "Green, Blue, Purple, Pink, Black, White] \n"
    "# \t To be implemented:\n"
    "# \t\t RGBA(R,G,B,A)\n"
    "# \t\t Redefining constants i.e. Red = RGBA(255,0,0,255)\n"
    "ArrowColour = \"Pink\"\n"
    "\n"
    "FilePath = \"Short\"\n"
    "\n"
    "ColourPalette = [\"Red\", \"Yellow\", \"Purple\"]\n";

char *get_config_file_path() {
  char *config_file =
      malloc(strlen(get_home_dir()) + strlen(CONFIG_FILENAME) + 1);

  if (config_file == NULL) {
    fprintf(stderr, "ash: Error allocating memory for config file");
    return NULL;
  }
  strcpy(config_file, get_home_dir());
  strcat(config_file, CONFIG_FILENAME);
}

// check if file exists. if yes break else create then break
char *check_for_config_file() {
  const char *home_dir = get_home_dir();

  char *config_file = get_config_file_path();

  //~ Open the file. If it doesnt exist create it
  FILE *config_file_ptr = fopen(config_file, "r");
  if (!config_file_ptr) {
    printf("\nCannot find config file. Creating it ...\n");
    config_file_ptr = fopen(config_file, "w");
    fprintf(config_file_ptr, DEFAULT_CONFIG_FILE_CONTENTS);
    fclose(config_file_ptr);
  }

  return config_file;
}

struct Format {
  char *KEY;
  char *VALUE;
};

char **read_file_contents(const char *filepath) {
  FILE *file_ptr;
  char line_content[MAX_LINE_LENGTH];
  char **file_content = malloc(MAX_LINES * sizeof(char *));
  int line_count = 0;

  if (file_content == NULL) {
    fprintf(stderr, "ash: Memory allocation failed\n");
    exit(1);
  }

  file_ptr = fopen(filepath, "r");
  if (file_ptr == NULL) {
    fprintf(stderr, "ash: Config file can't be opened\n");
    free(file_content);
    exit(1);
  }

  while (fgets(line_content, MAX_LINE_LENGTH, file_ptr) &&
         line_count < MAX_LINES) {
    // strdup = string duplicate
    file_content[line_count] = strdup(line_content);
    if (file_content[line_count] == NULL) {
      fprintf(stderr, "ash: Memory allocation failed for line %d\n",
              line_count);
      // Free previously allocated memory
      for (int i = 0; i < line_count; i++) {
        free(file_content[i]);
      }
      free(file_content);
      fclose(file_ptr);
      exit(1);
    }
    line_count++;
  }

  // Null-terminate the array
  file_content[line_count] = NULL;

  fclose(file_ptr);
  return file_content;
}

void print_file(char **file) {
  for (int i = 0; file[i] != NULL; i++) {
    printf("[%s]", file[i]);
  }
}

void free_file(char **file) {
  for (int i = 0; file[i] != NULL; i++) {
    free(file[i]);
  }
  free(file);
}
