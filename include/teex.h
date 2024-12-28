#ifndef TEEX_H
#define TEEX_H

#include <ncurses.h>

#define MAX_LINE_LENGTH 256
#define MAX_LINES 1000

extern int ch;  // for chars
extern int row, col;  
extern char lines[MAX_LINES][MAX_LINE_LENGTH]; // all the lines lays here!
extern int line_count;

// Function declarations
void helpFunc(); // information of the help window
void destroy_win(WINDOW * local_win); // destory the help window(for now)
WINDOW *create_window(int height, int width, int starty, int startx); // called by the helpfunc to create the help window
void printlines(); // handle most of the print on the screen (except the one in the helpfunc)
void adjust_cursor_col(char lines[MAX_LINES][MAX_LINE_LENGTH], int *row, int *col); // help keeping the cursor correct
void save_file(const char *filename); 
void read_file(const char *filename);
void editor(const char *filename);

#endif // TEEX_H
