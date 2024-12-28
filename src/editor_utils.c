#include "../include/teex.h"
#include <ncurses.h>
#include <string.h>


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


