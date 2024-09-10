#include "../include/flags.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>

// void command_iter(COMMAND commands[], int num_commands)
// {
//     for (int i = 0; i < num_commands; i++) {
//         COMMAND command = commands[i];
//         printf("Command: %s\n", command.name);

//         // Dynamically calculate the number of flags
//         int num_flags = 0;
//         while (command.flags[num_flags] != NULL) {
//             num_flags++;
//         }

//         printf("Number of flags: %d\n", num_flags);
//         printf("Flags: ");
//         for (int j = 0; j < num_flags; j++) {
//             printf("%s ", command.flags[j]);
//         }
//         printf("\n\n");
//     }
// }

// void display_flags_from_command_name(COMMAND commands[], char *name,
//                                      int num_of_commands) {
//   for (int i = 0; i < num_of_commands; i++) {
//     COMMAND command = commands[i];

//     //? Calculates the number of flags that a command has
//     int num_flags = 0;
//     while (command.flag_pairs[num_flags].short_ != NULL) {
//       num_flags++;
//     }
//     //? Checks if the command given matches that in the command table and if true, prints the flags
//     if (strcmp(command.name, name) == 0) {
//       if (num_flags == 0) {
//         printf("No Flags\n");
//       } else {
//         for (int j = 0; j < num_flags; j++) {
//           printf("%s / %s \n", command.flag_pairs[j].short_, command.flag_pairs[j].long_);
//         }
//       }
//     }
//   }
// }

void display_flags_from_command_name(COMMAND commands[], char *name) {
  int command_index = 0;
  
  while (commands[command_index].name != NULL) {
    COMMAND command = commands[command_index];
    command_index++;

    //? Calculates the number of flags that a command has
    int num_flags = 0;
    while (command.flag_pairs[num_flags].short_ != NULL) {
      num_flags++;
    }
    
    //? Checks if the command given matches that in the command table and if true, prints the flags
    if (strcmp(command.name, name) == 0) {
      if (num_flags == 0) {
        printf("No Flags\n");
      } else {
        for (int j = 0; j < num_flags; j++) {
          printf("%s / %s \n", command.flag_pairs[j].short_, command.flag_pairs[j].long_);
        }
      }
    }
  }
}

// int flags_main(int argc, char** argv)
// {
//     COMMAND list_of_commands[] = {
//         { "cd", { NULL } }, // If no flags, explicitly set the first element
//         to NULL { "ls", { "-a", "-v", NULL } }, // Ensure subsequent slots
//         are NULL to indicate end of valid flags { "rm", { "-rf", NULL } } //
//         Same as above
//     };

//     int num_commands = sizeof(list_of_commands) / sizeof(COMMAND);
//     // command_iter(list_of_commands, num_commands);
//     display_flags_from_command_name(list_of_commands, argv[1], num_commands);
// }