#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *check_for_config_file();
char **read_file_contents(const char *filepath);
void print_file(char **file);
void free_file(char **file);
char *get_config_file_path();

#endif
