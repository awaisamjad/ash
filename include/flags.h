#ifndef FLAGS_H
#define FLAGS_H

#define MAX_FLAGS 10

typedef struct {
    char* short_;
    char* long_;
} FLAG_PAIR;

typedef struct {
    char* name;
    FLAG_PAIR flag_pairs[MAX_FLAGS + 1]; //? +1 for the sentinel NULL value
} COMMAND;

void command_iter(COMMAND commands[], int num_commands);
void display_flags_from_command_name(COMMAND commands[], char* name, int num_of_commands);

#endif
