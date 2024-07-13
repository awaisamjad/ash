#include <complex.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define RGB_COLOR(r, g, b) "\033[38;2;" #r ";" #g ";" #b "m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"
#define RESET "\033[0m"

// Example predefined colors
const char *RED = RGB_COLOR(255, 0, 0);
const char *GREEN = RGB_COLOR(0, 255, 0);
const char *BLUE = RGB_COLOR(0, 0, 255);
const char *YELLOW = RGB_COLOR(255, 255, 0);

void print_colored_text(const char *text, const char *color,
                        const char *style) {
  printf("%s%s%s%s", color, style, text, RESET);
}

void println_colored_text(const char *text, const char *color,
                          const char *style) {
  printf("%s%s%s%s\n", color, style, text, RESET);
}

/*
  Function Declarations for builtin shell commands:
 */
int cd(char **args);
int help(char **args);
int exit_(char **args);
int ls(char **args);
int touch(char **args);
int mv(char **args);
int cp(char **args);
int rm(char **args);
int cat(char **args);
int man(char **args);

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_commands[] = {"cd", "help", "exit_", "ls",  "touch",
                            "mv", "cp",   "rm",    "cat", "man"};

int (*builtin_functions[])(char **) = {cd, help, exit_, ls,  touch,
                                       mv, cp,   rm,    cat, man};

int number_of_builtin_commands() {
  return sizeof(builtin_commands) / sizeof(char *);
}

/*
  Builtin function implementations.
*/
int cd(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "ash: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("ash");
    }
  }
  return 1;
}

int exit_(char **args) { return 0; }

int ls(char **args) {
  DIR *d;
  struct stat statbuf;
  struct dirent *dir;
  char *path;
  char fullPath[1024];

  //~ If the argument exists 'ls' to that path else 'ls' the current path
  // TODO If multiple arguments are given, multiple 'ls' results are given for
  // each argument
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
          println_colored_text(dir->d_name, BLUE, BOLD);
        } else {
          println_colored_text(dir->d_name, YELLOW, "");
        }
      }
    }
    closedir(d);
  }
  return 1;
}

int touch(char **args) {}
int mv(char **args) {}
int cp(char **args) {}
int rm(char **args) {}
int cat(char **args) {}
int man(char **args) {}
int help(char **args) {
  int i;
  printf("Awais Amjads Shell (ash)\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < number_of_builtin_commands(); i++) {
    println_colored_text(builtin_commands[i], GREEN, UNDERLINE);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}
/*
Main Functions
*/

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

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
char **split_line(char *line) {
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char *));
  char *token;

  if (!tokens) {
    fprintf(stderr, "ALLOCATION ERROR\n");
    exit(0);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char *));
      if (!tokens) {
        fprintf(stderr, "ALLOCATION ERROR\n");
        exit(0);
      }
    }

    token = strtok(NULL, LSH_TOK_DELIM);
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
      // Found a match, execute the corresponding function.
      return (*builtin_functions[i])(args);
    }
  }

  // If we reach here, no built-in command matched.
  print_colored_text("ash: Command Not Found\n", RED, BOLD);
  return 1; // You might want to call launch(args) here if you have external
            // command execution implemented.
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
      // printf("%s", cwd);
      print_colored_text(cwd, GREEN, BOLD);
    } else {
      perror("Error getting current working directory");
    }
    printf("$ ");

    line = read_line();
    arguments = split_line(line);
    is_finished = execute(arguments);

    free(line);
    free(arguments);
  } while (is_finished);
}

int main(int argc, char **argv) {
  loop();

  return 0;
}
