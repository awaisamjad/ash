#include "../include/config.h"
#include <stdio.h>

int check_for_config_file(){
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

    char* config_filename = "/.ash_config";
    char config_file[1024];

    strcpy(config_file, home_dir);
    strcat(config_file, config_filename);

    //~ Open the file. If it doesnt exist create it
    FILE *config_file_ptr = fopen(config_file, "r");
    if (!config_file_ptr) {
        printf("\nCannot find config file. Creating it ...\n");
        config_file_ptr = fopen(config_file, "w");
        fprintf(config_file_ptr, "# ash config file");
        fclose(config_file_ptr);
    }

}