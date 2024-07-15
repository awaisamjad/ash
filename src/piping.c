#include <stdio.h>
#include <string.h>

#define MAX_ROWS 1024
#define MAX_COLS 1024

int main(int argc, char *argv[]) {
    int row = 0, col = 0;
    char separator = ';';
    char *array[MAX_ROWS][MAX_COLS] = { NULL };

    for (int i = 1; i < argc; i++) { // Starting from 1 to skip the program name
        if (argv[i][0] == separator && argv[i][1] == '\0') {
            row++;
            col = 0;
            if (row >= MAX_ROWS) {
                fprintf(stderr, "Exceeded maximum rows\n");
                return 1;
            }
        } else {
            array[row][col] = argv[i];
            col++;
            if (col >= MAX_COLS) {
                fprintf(stderr, "Exceeded maximum columns in row %d\n", row);
                return 1;
            }
        }
    }

    // Printing the 2D array for demonstration purposes
    for (int r = 0; r <= row; r++) {
        for (int c = 0; array[r][c] != NULL; c++) {
            printf("%s ", array[r][c]);
        }
        printf("\n");
    }

    return 0;
}
