#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

char* append(char* string, char c) {
  size_t size = strlen(string);
  char* string_size = malloc(size + 2); //? idk why the +2
  strcpy(string_size, string);
  string_size[size] = c;
  string_size[size + 1] = '\0';
  return string_size;
}

int main(int argc, char** argv) {
    printf("%c\n", argv[1][0]);
    if (argv[1][0] == '"')
    {
      printf("dsa");
      
    }
}