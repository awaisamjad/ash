#ifndef COMMAND_HISTORY_H
#define COMMAND_HISTORY_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>


int check_for_command_history_file();
int main_command_history();
#endif