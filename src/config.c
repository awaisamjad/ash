#include "../include/config.h"
#include <stdio.h>
#include <string.h>

char* check_for_config_file(){
    // check if file exists. if yes break else create then break
    const char* home_dir;

    #ifdef _WIN32 
        home_dir = getenv("USERPROFILE");
    #else
        home_dir = getenv("HOME");
    #endif

    if (home_dir == NULL){
        fprintf(stderr, "ash: Error finding home directory\n");
        return 1;
    }

    const char* config_filename = "/.ash_config";
    char* config_file = malloc(sizeof(home_dir) + sizeof(config_file) + 1);

    if (config_file == NULL) {
        fprintf(stderr, "ash: Error allocating memory for config file");
        return NULL;
    }
    
    strcpy(config_file, home_dir);
    strcat(config_file, config_filename);
    // strcpy(config_file_return, config_file);
    //~ Open the file. If it doesnt exist create it
    FILE *config_file_ptr = fopen(config_file, "r");
    if (!config_file_ptr) {
        printf("\nCannot find config file. Creating it ...\n");
        config_file_ptr = fopen(config_file, "w");
        fprintf(config_file_ptr, "# ash config file");
        fclose(config_file_ptr);
    }

    return config_file;
}

void read_file(char* filepath) {
    
}   

