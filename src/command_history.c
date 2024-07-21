#include "../include/command_history.h" 
#include <stdio.h>

int check_for_command_history_file(){
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

    FILE *command_history_file_ptr = fopen(command_history_file, "r");
    if (!command_history_file_ptr) {
        printf("\nCannot find command history file. Creating it ...\n");
        command_history_file_ptr = fopen(command_history_file, "w");
        fprintf(command_history_file_ptr, "# ash command history file");
        fclose(command_history_file_ptr);
    }

    return 1;

}

// Copied from GPT
int get_char(void) {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr); // Get current terminal attributes
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr); // Set new attributes
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr); // Restore old attributes
    return ch;
}

int is_up_arrow_pressed() {
    int ch = get_char();
    if (ch == 27) { // Escape character
        ch = get_char();
        if (ch == 91) { // '[' character
            ch = get_char();
            if (ch == 65) { // 'A' character for up arrow key
                return 1;
            }
        }
    }
    return 0;
}

int is_down_arrow_pressed() {
    int ch = get_char();
    if (ch == 27) { // Escape character
        ch = get_char();
        if (ch == 91) { // '[' character
            ch = get_char();
            if (ch == 66) { // 'A' character for up arrow key
                return 1;
            }
        }
    }
    return 0;
}

int main_command_history(){
    FILE *command_history_file_ptr;
    command_history_file_ptr = fopen("s.txt", "r");
    char contents[1024];
    fgets(contents, 1024, command_history_file_ptr);
    if (is_up_arrow_pressed() == 1) {
        printf("%s", contents);
        fclose(command_history_file_ptr);
    }
    return 0;
}