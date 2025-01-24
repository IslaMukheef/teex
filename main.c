#include "../include/teex.h"
#include <ncurses.h>
#include <stdio.h>
#include <string.h>

int ch;
int row = 0, col = 0;
char lines[MAX_LINES][MAX_LINE_LENGTH];
int line_count = 0;
bool enable_undo_redo = true; // tracking will be working now 
//vars for file explorer
int load_new_file = 0; // flag is 0 when there is no new file to load
char file_to_load[256]; // the new file that we will load


int main(int argc, char *argv[]) {
    char filename[256];

    if (argc < 2) {
        strcpy(filename, "isla.txt");
    } else {
        strcpy(filename, argv[1]);
        read_file(filename);
    }

    // Main application loop
    while (1) {
        // Load the initial or new file
        if (load_new_file == 1) {
            line_count = 0;
            row = 0;
            col = 0;
            strcpy(filename, file_to_load);
            read_file(filename);
            load_new_file = 0; // Reset the flag
        }

        // Launch the editor
        editor(filename);

        // If no new file is set, exit the loop and terminate
        if (load_new_file == 0) {
            break;
        }
    }

    return 0;
}

