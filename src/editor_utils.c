#include "../include/teex.h"
#include <ncurses.h>
#include <string.h>
#include <stdio.h>  



/* 
called when app first run!
*/
void initTracking(Queue *stack){
    stack->front = 0;
    stack->rear = 0;
    stack->current_size = 0;
}
// each new added char will be pushed to the sack
//the stack has it limit(Queue_SIZE)
void push(Queue *stack,char new_char, int new_x, int new_y){
    if(stack->current_size == Queue_SIZE){
        stack->front = (stack->front +1) % Queue_SIZE; // move it by one when the last elemetn is reached
    }
    else{
        stack->current_size++;
    }
    stack->node_char[stack->rear] = new_char; // store new_char in rear current positon
    stack->x_row[stack->rear] = new_x;
    stack->y_col[stack->rear] = new_y;
    stack->rear = (stack->rear +1) % Queue_SIZE; // move rear to next element

}

// ctrl+z function where char will be poped from the stack and will be deleted from screen as well
void pop(Queue *stack){
    if(stack->current_size ==0){
        return;
    }
    // stack->rear, but after the push() operation, rear always points to the next empty slot (not the last pushed item).
    int index = (stack->rear - 1 + Queue_SIZE) % Queue_SIZE;
    int temp_x = row;
    row = stack->x_row[index];
    int temp_y = col;
    col = stack->y_col[index]; 
    //assuming that the current col and row points to the char that will be poped
    delete_char(2);
       
    row = temp_x;
    col = temp_y;
    //mvprintw(stack->x_row[stack->rear],stack->y_col[stack->rear],"%c", stack->node_char[stack->rear]);
    stack->rear = index ;     
    stack->current_size--;
    
} // end of the undo structure !

/*
delete_char handles all the removing of chars in the editor even spaces and taps
it should be called whenever there is a need to remove something editor!
Function can be rewritten in more clear way but this will be done later on
*/
void delete_char(int caller_type){
    // 1 = backsapce caller !
    // 2 = pop called
    switch (caller_type)
    {
    case 1: // backspace called to delete a char
        if (col > 0) {
        memmove(&lines[row][col - 1], &lines[row][col], strlen(&lines[row][col]) + 1);
        col = (row == 0 && col == 0) ? 0 : col -1; // Move cursor back after deletion
        } 
        else if(col ==0 && strlen(lines[row]) == 0 ){ //deletes empty lines
            goto empty_line;
        }
        else if (col == 0 && row > 0){
            goto merge_lines;
        }
        break;


    case 2: // pop called to delete a certin char
        if(lines[row][col] != '\0'){
             memmove(&lines[row][col], &lines[row][col + 1], strlen(lines[row]) - col);
        }  
        break;
    // merge is deleting a line first char so that makes the rest moves line up to merge
    merge_lines:  // merge is a common use 
        strcat(lines[row-1], lines[row]); // merge current line with the one upper
        for (int i = row; i < line_count; i++) {
            strncpy(lines[i], lines[i+1], MAX_LINE_LENGTH); // moves lines up
        }
        col = strlen(lines[row - 1]);
        row--;
        line_count--;
        break;
    
    empty_line: // if line is empty and delete action was usd then it get deleted
        if (row > 0) {
            for (int i = row; i < line_count; i++) {
                strncpy(lines[i], lines[i+1], MAX_LINE_LENGTH); // moves lines up
            }
            col = strlen(lines[row - 1]); 
            row--;
            line_count--;
        }
        break;
    
    default:
        break;
    }
    
    clear(); // Clear the screen after deleting a character
}

//creates new windows!
//It's used to create the help window for now but will be used for new stuff later on(i think!)
WINDOW *create_window(int height, int width, int starty, int startx){
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0);
    wrefresh(local_win);
    return local_win;
}
 // the docs says we should destroy the window this way even tho i did not had any problems without using it but for safety !
void destroy_win(WINDOW * local_win){
    wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '); // docs says we should use it
    wrefresh(local_win);
    delwin(local_win);
}


//Print help window
void helpFunc() {
    int startx, starty;
    int height=LINES/2; // get height to be half of the rows
    int width =COLS/2;  // get width to be half of the rows 
    int wch;            // window char
    WINDOW *my_wind;    // struct to window

    starty = (LINES - height) / 2; // center as well 
    startx = (COLS - width) / 2;   

    my_wind = create_window(height, width, starty, startx);
    
    mvprintw(starty, (COLS - 4)/2 , "%s", "HELP"); // hepl window title!!
    wrefresh(my_wind);

                     
    char *msg[] = 
    {
        "Press any key to exit this message",
        "Ctrl + s: save the file",
        "Ctrl + h: shows this help screen",
        "Ctrl + z: Reverse last action",
        "Esc: exit Teex editor"
    };
    int msg_len = sizeof(msg) / sizeof(msg[0]);
    for(int j = 0; j< msg_len; j++){
        mvwprintw(my_wind, j+1, 1, "%s", msg[j]);
    }
    wrefresh(my_wind);
    wch = getch(); // this line is used to just keep the window on but will be replaced later
    destroy_win(my_wind);
}


// printlines 
void printlines(){
    clear();  
    int max_y = getmaxy(stdscr);

    int start_line = (row >=max_y) ? row - max_y +1 : 0; // if row is less start from 0 
    int end_line = start_line + max_y -1 ;
  
    for(int i = start_line; i <= end_line && i <= line_count; i++){
        mvprintw(i- start_line,0,"%s", lines[i]);
    }
    move(row - start_line, col);
    refresh();

}

// Function to adjust the cursor's column position if it's beyond the current line's length
void adjust_cursor_col(char lines[MAX_LINES][MAX_LINE_LENGTH], int *row, int *col) {
    if (*col > (int)strlen(lines[*row])) {
        *col = strlen(lines[*row]); // Move the cursor to the last char if the line is shorter than the current col position
    }
}


