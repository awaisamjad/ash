#include <complex.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/commands.h"
#include "../include/utility.h"

int cd(char** args)
{
    int num_of_args = count_args(args);
    if (num_of_args < 2) {
        fprintf(stderr, "ash: Too few arguments\nUsage: cd <directory>\n");
        return 1;
    }
    if (args[1] == NULL) {
        fprintf(stderr, "ash: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("ash");
        }
    }
    return 1;
}

int EXIT(char** args) { return 0; }

int ls(char** args)
{
    DIR* d;
    struct stat statbuf;
    struct dirent* dir;
    char* path;
    char fullPath[1024]; // TODO Does this need 1024?

    //~ If the argument exists 'ls' to that path else 'ls' the current
    // path
    // TODO If multiple arguments are given, multiple 'ls' results are
    // given for each argument
    if (args[1] != NULL) {
        path = args[1];
    } else {
        path = ".";
    }

    d = opendir(path);

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            snprintf(fullPath, sizeof(fullPath), "%s/%s", path, dir->d_name);
            if (stat(fullPath, &statbuf) == 0) {

                // Check if it's a directory or a file
                char type = S_ISDIR(statbuf.st_mode) ? 'D' : 'F';
                if (type == 'D') {
                    // TODO design colour scheme
                    println_colored_text(dir->d_name, BG_PINK, "", BOLD);
                } else {
                    println_colored_text(dir->d_name, BG_RED, "", "");
                }
            }
        }
        closedir(d);
    } else {
        fprintf(stderr, "Error listing directory contents\n");
    }
    return 1;
}
// TODO Add way to give path to the file: touch dir1/dir2/test.txt
int touch(char** args)
{
    FILE* file_ptr;
    int number_of_args = count_args(args);

    if (args[1] != NULL) {
        for (int i = 1; i < number_of_args; i++) //~ Handles all arguments
        {
            //~ Check if file exists
            if (access(args[i], F_OK) != -1) {
                printf("File '%s' already exists, "
                       "cannot create a new one.\n",
                    args[i]);
                return 1;
            } else {
                file_ptr = fopen(args[i], "w");
            }
            if (file_ptr == NULL) {
                perror("Error creating file");
                return 1;
            }
            fclose(file_ptr);
        }
    } else {
        fprintf(stderr, "Error. No file name entered\nUsage: touch "
                        "<filename>\n");
        return 1;
    }

    return 1;
}
// TODO Needs to take more than just 1 word for the content
//! Overwrites the current file
int echo(char** args)
{
    FILE* file_ptr;

    //~ Check if only 3 arguments
    int number_of_arguments = count_args(args);
    if (number_of_arguments < 3) {
        fprintf(stderr, "Error. Too few arguments\nUsage: echo "
                        "\"Content\" filename\n");
    } else if (number_of_arguments > 3) {
        fprintf(stderr, "Error. Too many arguments\nUsage: echo "
                        "\"Content\" filename\n");
    }

    char* content = args[1];
    char* filename = args[2];
    if (content != NULL && filename != NULL) {
        file_ptr = fopen(filename, "w");
        if (file_ptr == NULL) {
            perror("Error creating file");
            return 1;
        }
        fprintf(file_ptr, "%s", content);
        fclose(file_ptr);
    } else {
        fprintf(stderr, "Error. No content or file name "
                        "entered\nUsage: echo "
                        "\"Content\" filename\n");
        return 1;
    }
    return 1;
}

int mv(char** args) { }
int cp(char** args) { }
int rm(char** args) { }
int cat(char** args)
{
    FILE* file_ptr;

    int num_of_args = count_args(args);
    are_number_of_args_correct(args, 2, "Usage: cat <filename>");

    char filename[100], contents; // TODO Is 100 bytes enough?
    scanf("%s", filename);

    // Open file
    file_ptr = fopen(filename, "r");
    if (file_ptr == NULL) {
        printf("Cannot open file \n");
        exit(0);
    }
    contents = fgetc(file_ptr);
    while (contents != EOF) {
        printf("%c", contents);
        contents = fgetc(file_ptr);
    }

    fclose(file_ptr);
    return 0;
}
int man(char** args) { }
// TODO Make better
int mkd(char** args)
{
    int num_of_args = count_args(args);

    if (num_of_args < 2) {
        fprintf(stderr, "Too few arguments\nUsage: mkd <path>\n");
        return 1;
    } else if (num_of_args > 2) {
        fprintf(stderr, "Too many arguments\nUsage: mkd <path>\n");
        return 1;
    }

    char* filepath = args[1];
    const char delimiter[] = "/";
    char* token;
    char cwd[100];
    int count = 0;

    if (strchr(filepath, '/') == NULL) { // If the forward slash isnt in the file path string just make the directory
        mkdir(filepath, 0777); // TODO Look more into the mode. 0777 sets full read/write/execute permissions for all users.
        return 1;
    } else {
        token = strtok(filepath, delimiter);

        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("ash: mkd - Error getting current working directory");
        }

        //~ Go over all the sub-directories (tokens) and cd into them and make the dirs
        while (token != NULL) {
            mkdir(token, 0777);
            chdir(token);
            token = strtok(NULL, delimiter);
            count++;
        }

        //~ Change back to starting directory
        if (chdir(cwd) != 0) {
            fprintf(stderr, "ash: mkd - Error changing directory");
        }
        return 1;
    }
}

int command_history(){
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

    char* command_history_filename = "/.ash_command_history";
    char command_history_file[1024];
    strcpy(command_history_file, home_dir);
    strcat(command_history_file, command_history_filename);
    printf("%s", command_history_file);
    //~ Open the file. If it doesnt exist create it
    FILE *command_history_file_ptr = fopen(command_history_file, "r");
    if (!command_history_file_ptr) {
        printf("\nCannot find command history file. Creating it ...\n");
        command_history_file_ptr = fopen(command_history_file, "w");
        fclose(command_history_file_ptr);
    }


}

int config_file(){
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
    printf("%s", config_file);
    //~ Open the file. If it doesnt exist create it
    FILE *config_file_ptr = fopen(config_file, "r");
    if (!config_file_ptr) {
        printf("\nCannot find config file. Creating it ...\n");
        config_file_ptr = fopen(config_file, "w");
        fclose(config_file_ptr);
    }


}