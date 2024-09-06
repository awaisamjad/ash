#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CONFIG_FILEPATH ".ash_config"
#define SEPARATOR '='
#define MAX_LINES 1024 // Probably a lot
#define MAX_LINE_LENGTH 1024 // Probably too much

struct Format {
  char *KEY;
  char *VALUE;
};

char **read_file_contents(const char *filepath) {
    FILE *file_ptr;
    char line_content[MAX_LINE_LENGTH];
    char **file_content = malloc(MAX_LINES * sizeof(char*));
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

    while (fgets(line_content, MAX_LINE_LENGTH, file_ptr) && line_count < MAX_LINES) {
        // strdup = string duplicate
        file_content[line_count] = strdup(line_content);
        if (file_content[line_count] == NULL) {
            fprintf(stderr, "ash: Memory allocation failed for line %d\n", line_count);
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

void print_file(char** file) {
    for (int i = 0; file[i] != NULL; i++) {
        printf("[%s]", file[i]);
    }
}

int main() {
    char **file_content = read_file_contents(CONFIG_FILEPATH);
    print_file(file_content);

    // Free allocated memory
    for (int i = 0; file_content[i] != NULL; i++) {
        free(file_content[i]);
    }
    free(file_content);

    return 0;
}
