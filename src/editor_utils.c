#include "../include/teex.h"
#include <ncurses.h>
#include <string.h>
#include <stdio.h>  



/* 
called when app first run if the enable_undo_redo was True which is the defult!
*/
void initTracking(CircularQueue *stack){
    stack->front = 0;
    stack->rear = 0;
    stack->current_size = 0;
}
// each new added char will be pushed to the sack
//the stack has it limit(Queue_SIZE)
void push(CircularQueue *stack,char new_char, int new_x, int new_y){
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
void pop(CircularQueue *stack, CircularQueue *reStack){
    if(stack->current_size ==0){
        return;
    }
    
    // stack->rear, but after the push() operation, rear always points to the next empty slot (not the last pushed item).
    int index = (stack->rear - 1 + Queue_SIZE) % Queue_SIZE;
    //push the char that will be deleted to our redo(reStack) 
    push(reStack,stack->node_char[index], stack->x_row[index], stack->y_col[index]);
    //
    int temp_x = row;
    row = stack->x_row[index];
    int temp_y = col;
    col = stack->y_col[index]; 
    delete_char(2);    
    row = temp_x;
    col = temp_y;
    adjust_cursor_col(lines, &row, &col); // make sure the cursor stays at the last char not beyond it
    stack->rear = index ;     
    stack->current_size--;
    
}
/*
Redo will work as the opiste of pop which it will pop the last char from Restack and push it to
stack again and reprint it to the screen.
Difference is: redo needs to add the char back to lines and print it back
we do not change  line_count here! 
POP do not delete empty lines or remerge the old ones for now
it only delete chars.
redo only reprint them
*/
void redo(CircularQueue *reStack, CircularQueue *stack){
    if(reStack->current_size == 0){
        return;
    }
    int index = (reStack->rear - 1 + Queue_SIZE) % Queue_SIZE; // get the index of last char
    char redo_char = reStack->node_char[index];
    int redo_row = reStack->x_row[index];
    int redo_col = reStack->y_col[index];
    push(stack, redo_char, redo_row, redo_col);// push the the char that we did rewrite to the stack so we can undo it later if want
    memmove(&lines[redo_row][redo_col + 1], &lines[redo_row][redo_col], strlen(&lines[redo_row][redo_col]) + 1); 
    lines[redo_row][redo_col] = redo_char; // adding the char back to lines
    row = redo_row;
    col = redo_col + 1;
    reStack->rear = index ;     
    reStack->current_size--;
    printlines(); // reprint the char
}
 // end of the undo structure !

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
        "Ctrl + y: restore what was deleted with ctrl+z",
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


