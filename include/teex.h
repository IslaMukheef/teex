#ifndef TEEX_H
#define TEEX_H

#include <ncurses.h>

#define MAX_LINE_LENGTH 256
#define MAX_LINES 1000
#define Queue_SIZE 50

extern int ch;  // for chars
extern int row, col;  
extern char lines[MAX_LINES][MAX_LINE_LENGTH]; // all the lines lays here!
extern int line_count;

typedef struct{
    char node_char[Queue_SIZE];
    int x_row[Queue_SIZE];
    int y_col[Queue_SIZE];
    int front;
    int rear;
    int current_size;
}Queue;

// Function declarations
void helpFunc(); // information of the help window
void destroy_win(WINDOW * local_win); // destory the help window(for now)
WINDOW *create_window(int height, int width, int starty, int startx); // called by the helpfunc to create the help window
void printlines(); // handle most of the print on the screen (except the one in the helpfunc)
void adjust_cursor_col(char lines[MAX_LINES][MAX_LINE_LENGTH], int *row, int *col); // help keeping the cursor correct
void initTracking(Queue *stack);// init the tracking of the undo fucntion
void push(Queue *stack,char new_char, int new_x, int new_y); // track last changes(Queue_SIZE)
void pop(Queue *stack); // undo using the ctrl +z 
void delete_char(int caller_type); // handle deleting chars in the editor

void save_file(const char *filename); 
void read_file(const char *filename);
void editor(const char *filename);

#endif // TEEX_H
