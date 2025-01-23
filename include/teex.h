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
extern bool enable_undo_redo; // make sure that is on if you want to track
extern int load_new_file;  // flag set to alert the editor to kill everthing and move on with the new file from explore
extern char file_to_load[256];

typedef struct {
    char node_char[Queue_SIZE];
    int x_row[Queue_SIZE];
    int y_col[Queue_SIZE];
    int front;
    int rear;
    int current_size;
}CircularQueue; // change the name to something else so it can be more understanding !

// Function declarations
void helpFunc(); // information of the help window
void destroy_win(WINDOW * local_win); // destory the help window(for now)
WINDOW *create_window(int height, int width, int starty, int startx); // called by the helpfunc to create the help window
void printlines(); // handle most of the print on the screen (except the one in the helpfunc)
void adjust_cursor_col(char lines[MAX_LINES][MAX_LINE_LENGTH], int *row, int *col); // help keeping the cursor correct
void initTracking(CircularQueue *stack);// init the tracking of the undo fucntion
void push(CircularQueue *stack,char new_char, int new_x, int new_y); // track last changes(Queue_SIZE)
void pop(CircularQueue *stack, CircularQueue *reSack); // undo using the ctrl +z 
void redo(CircularQueue *reStack, CircularQueue *stack);// redo using ctrl + y
void delete_char(int caller_type); // handle deleting chars in the editor
void file_explorer(); // create window with the files of current dir
void file_explorer_hightlight(WINDOW *local_win, int num, char *fileslist[]);

void save_file(const char *filename); 
void read_file(const char *filename);
void editor(const char *filename);

#endif // TEEX_H
