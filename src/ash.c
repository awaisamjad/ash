#include <complex.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/command_history.h"
#include "../include/commands.h"
#include "../include/config.h"
#include "../include/flags.h"
#include "../include/utility.h"


// Command Table
COMMAND list_of_commands[] = {
    {"cd", {{NULL, NULL}}}, 
    {"ls", {{"-a", "--all"}, {"-l", "--long"}, {NULL, NULL}}},
    {"rm", {{"-rf", NULL}, {NULL, NULL}}},
    {NULL, {{NULL, NULL}}} //? Represents the end of the list
};

/*
  Function Declarations for builtin shell commands:
 */
int cd(char **args);
int help(char **args);
int EXIT(char **args);
int ls(char **args);
int touch(char **args);
int echo(char **args);
int mv(char **args);
int cp(char **args);
int rm(char **args);
int cat(char **args);
int man(char **args);
int mkd(char **args);
int test(char **args);

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_commands[] = {
    "cd", "help", "EXIT", "ls",  "touch", "mv",
    "cp", "rm",   "cat",  "man", "echo",  "mkd",
    "test"
};

int (*builtin_functions[])(char **) = {
    cd, help, EXIT, ls, touch, mv, cp, rm, cat, man, echo, mkd, test, 
};

int number_of_builtin_commands() {
  return sizeof(builtin_commands) / sizeof(char *);
}

/*
Main Functions
*/

int help(char **args) {
  int i;
  printf("Awais Amjads Shell (ash)\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < number_of_builtin_commands(); i++) {
    println_colored_text(builtin_commands[i], GREEN, "", UNDERLINE);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}

#define BUFSIZE 1024
char *read_line(void) {
  int bufsize = BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int character;

  if (!buffer) {
    fprintf(stderr, "ALLOCATION ERROR \n");
    exit(0);
  }

  while (1) {
    character = getchar();
    if (character == EOF || character == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = character;
    }
    position++;

    if (position >= bufsize) {
      bufsize += BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "ALLOCATION ERROR \n");
        exit(0);
      }
    }
  }
}

#define TOKEN_BUFSIZE 64
#define TOKEN_DELIM " \t\r\n\a"
char **split_line(char *line) {
  int bufsize = TOKEN_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char *));
  char *token;

  if (!tokens) {
    fprintf(stderr, "ALLOCATION ERROR\n");
    exit(0);
  }

  token = strtok(line, TOKEN_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += TOKEN_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char *));
      if (!tokens) {
        fprintf(stderr, "ALLOCATION ERROR\n");
        exit(0);
      }
    }

    token = strtok(NULL, TOKEN_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

int launch(char **args) {
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("Awais Amjads Shell");
    }
    exit(0);
  } else if (pid < 0) {
    // Error forking
    perror("Awais Amjads Shell");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int execute(char **args) {
  if (args[0] == NULL) {
    // No command entered.
    return 1;
  }

  for (int i = 0; i < number_of_builtin_commands(); i++) {
    if (strcmp(args[0], builtin_commands[i]) == 0) {
      // Found a match, execute the corresponding
      // function.
      //TODO Number of commands param should be dynamically calculated
      display_flags_from_command_name(list_of_commands, args[0]);
      return (*builtin_functions[i])(args);
    }
  }

  // If we reach here, no built-in command matched.
  print_colored_text("ash: Command Not Found\n", RED, "", BOLD);
  return 1; // You might want to call launch(args) here if you have
            // external command execution implemented.
}

void loop() {
  char *line;
  char **arguments;
  int is_finished;
  // TODO Give a proper length to the array
  char cwd[100];

  do {
    //~ Get the current directory
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      print_colored_text(cwd, GREEN, "", BOLD);
    } else {
      perror("Error getting current working directory");
    }
    print_colored_text(" => ", PINK, "", "");

    // main_command_history();

    line = read_line();
    arguments = split_line(line);
    is_finished = execute(arguments);

    free(line);
    free(arguments);
  } while (is_finished);
}

/***
Checks to see if the required files exist. If not they will be created
*/
void check_for_required_files() {
  (void)check_for_command_history_file();
  (void)check_for_config_file();
}

int main(int argc, char **argv) {
  // (void)check_for_required_files();
  // char** config_file = read_file_contents(get_config_file_path());
  // print_file(config_file);
  // free_file(config_file);
  
  loop();
  
  // main_command_history();
  return 0;
}
