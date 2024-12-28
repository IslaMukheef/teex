#include "../include/teex.h"
#include <ncurses.h>
#include <stdio.h>
#include <string.h>

int ch;
int row = 0, col = 0;
char lines[MAX_LINES][MAX_LINE_LENGTH];
int line_count = 0;

int main(int argc, char *argv[]) {
    char filename[256];
    if (argc < 2) {
        strcpy(filename, "isla.txt");
    } else {
        strcpy(filename, argv[1]);
        read_file(filename);
    }

    editor(filename);
    return 0;
}
