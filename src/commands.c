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
#include "../include/global.h"


int cd(char **args) {
  int num_of_args = count_args(args);
  const char *home_dir = getenv("HOME");

  if (home_dir == NULL) {
    home_dir = getenv("USERPROFILE"); // Fallback for Windows
  }

  //? go to home dir
  if (num_of_args == 1) {
    if (chdir(home_dir) != 0) {
      fprintf(stderr, "ash: Error going to home directory\n");
      perror("ash");
    }
  }
  //? if there is only one argument after `cd` then just change to that directory
  else if (num_of_args == 2) {
    if (chdir(args[1]) != 0) {
      printf("arg: %s\n", args[1]);
      fprintf(stderr, "ash: Error going to directory\n");
      perror("ash");
    }
  } 
  else {
    //? the directory may be multi word
    char* dir = "";
    int count = 1;
    //! in this code the dir argument can start with a " and end with a ' which shouldnt be allowed
    //! the check should ensure that if it starts with a " it ends with it and same for '
    //! this can be done with 2 separate if statements for " and ' respectively
    //TODO check is switch case is better
    if (args[1][0] == '"')
    {
      printf("dsa");
      while (args[1][count] != '"')
      {
        printf("char : %c", args[1][count]);
        count++;
      }
    }
    // else if (args[1][0] == '\'' )
    // {
    //   while (args[1][count] != '\'')
    //   {
    //     //! code doesnt work
    //     // dir = append(dir, args[1][count]);
    //     printf("char : %c", args[1][count]);
    //     count++;
    //   }
      
    // }
    else {
      fprintf(stderr, "ash: Too many arguments\nUsage: cd <directory>\n");
      return CONTINUE;
    }

    if (chdir(dir) != 0) {
      printf("arg: %s\n", dir);
      fprintf(stderr, "ash: Error going to directory\n");
      perror("ash");
    }
  }
  return CONTINUE;
}

int EXIT(char **args) { return BREAK; }

int ls(char **args) {
  DIR *d;
  struct stat statbuf;
  struct dirent *dir;
  char *path;
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
        char *text = dir->d_name;

        // Checks if it is a directory and if the dir/file isnt hidden.
        // TODO use the '-a' flag to override the hiding of the files
        if (type == 'D' && text[0] != '.') {
          // TODO design colour scheme
          println_colored_text(dir->d_name, BG_PINK, "", BOLD);
        } else if (type == 'F' && text[0] != '.') {
          println_colored_text(dir->d_name, BG_RED, "", "");
        }
      }
    }
    closedir(d);
  } else {
    fprintf(stderr, "Error listing directory contents\n");
  }
  return CONTINUE;
}

// TODO Add way to give path to the file: touch dir1/dir2/test.txt
int touch(char **args) {
  FILE *file_ptr;
  int number_of_args = count_args(args);

  if (args[1] != NULL) {
    for (int i = 1; i < number_of_args; i++) //~ Handles all arguments
    {
      //~ Check if file exists
      // TODO if the flag is -r then override the file
      if (access(args[i], F_OK) != -1) {
        printf("File '%s' already exists, "
               "cannot create a new one.\n",
               args[i]);
        return CONTINUE;
      } else {
        file_ptr = fopen(args[i], "w");
      }
      if (file_ptr == NULL) {
        perror("Error creating file");
        return CONTINUE;
      }
      fclose(file_ptr);
    }
  } else {
    fprintf(stderr, "Error. No file name entered\nUsage: touch "
                    "<filename>\n");
    return CONTINUE;
  }

  return CONTINUE;
}

// TODO Needs to take more than just 1 word for the content
//! Overwrites the current file
int echo(char **args) {
  FILE *file_ptr;

  //~ Check if only 3 arguments
  int number_of_arguments = count_args(args);
  if (number_of_arguments < 3) {
    fprintf(stderr, "Error. Too few arguments\nUsage: echo "
                    "\"Content\" filename\n");
  } else if (number_of_arguments > 3) {
    fprintf(stderr, "Error. Too many arguments\nUsage: echo "
                    "\"Content\" filename\n");
  }

  char *content = args[1];
  char *filename = args[2];
  if (content != NULL && filename != NULL) {
    file_ptr = fopen(filename, "w");
    if (file_ptr == NULL) {
      perror("Error creating file");
      return CONTINUE;
    }
    fprintf(file_ptr, "%s", content);
    fclose(file_ptr);
  } else {
    fprintf(stderr, "Error. No content or file name "
                    "entered\nUsage: echo "
                    "\"Content\" filename\n");
    return CONTINUE;
  }
  return CONTINUE;
}

//! Doesnt work when filename is entered
// TODO should take multiple files
int cat(char **args) {
  FILE *file_ptr;

  int num_of_args = count_args(args);
  if (num_of_args < 2) {
    fprintf(stderr, "Error. Too few arguments\nUsage: cat <filename>...\n");
    return CONTINUE;
  }

  char contents;
  for (int i = 1; i < num_of_args; i++) {
    char *filename = args[i];
    file_ptr = fopen(filename, "r");
    if (file_ptr == NULL) {
      printf("Cannot open file \n");
      return CONTINUE;
    }
    contents = fgetc(file_ptr);
    while (contents != EOF) {
        //TODO Print file name above text. Should be done with the verbose flag
        // printf("%s\n", args[i]);
        printf("%c", contents);
        contents = fgetc(file_ptr);
    }
    printf("\n");
  }

  fclose(file_ptr);
  return CONTINUE;
}
int man(char **args) {}
// TODO Make better
int mkd(char **args) {
  int num_of_args = count_args(args);

  if (num_of_args < 2) {
    fprintf(stderr, "Too few arguments\nUsage: mkd <path>\n");
    return CONTINUE;
  } else if (num_of_args > 2) {
    fprintf(stderr, "Too many arguments\nUsage: mkd <path>\n");
    return CONTINUE;
  }

  char *filepath = args[1];
  const char delimiter[] = "/";
  char *token;
  char cwd[100];
  int count = 0;

  if (strchr(filepath, '/') == NULL) { // If the forward slash isnt in the file
                                       // path string just make the directory
    mkdir(filepath, 0777); // TODO Look more into the mode. 0777 sets full
                           // read/write/execute permissions for all users.
    return CONTINUE;
  } else {
    token = strtok(filepath, delimiter);

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
      perror("ash: mkd - Error getting current working directory");
    }

    //~ Go over all the sub-directories (tokens) and cd into them and make the
    //dirs
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
    return CONTINUE;
  }
}

int mv(char **args) {}
// TODO Make sure if the copy is made in the same dir that it has a new name
int cp(char **args) {
  int num_of_args = count_args(args);

  if (num_of_args < 3) {
    fprintf(stderr, "ash: Too few arguments\nUsage: cp <filename> <directory>");
    return CONTINUE;
  } else if (num_of_args > 3) {
    fprintf(stderr,
            "ash: Too many arguments\nUsage: cp <filename> <directory>");
    return CONTINUE;
  }

  char *filename = args[1];
  char *directory = args[2];
  //~ buffer to store contents of file
  char buffer[4096];

  FILE *file_ptr;
  file_ptr = fopen(filename, "r");
  fgets(buffer, 4096, file_ptr);

  cd(&directory);
  fopen("filename.txt", "w");
}
int rm(char **args) {}

int test(char** args) {
  printf("%c\n", args[1][0]);
    char* dir = "";
    int count = 1;
    if (args[1][0] == '"')
    {
      while (args[1][count] != '"')
      {
        //? Empty space not registering
        if (args[1][count] == "")
        {
          dir = append(dir, '#');
        }
        
        dir = append(dir, args[1][count]);
        count++;
      }
      //? second quotation mark doesnt get added in loop
      dir = append(dir, '"');
    }
    else {
      fprintf(stderr, "enter something\n");
      return CONTINUE;
    }
    printf("%s", dir);
    // return BREAK;
    return CONTINUE;
}

// enum RETURN {
//   BREAK,
//   CONTINUE,
// };

// enum FD {
//   BREAK,
//   CON,
// };