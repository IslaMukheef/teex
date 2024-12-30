#include "../include/teex.h"
#include <ncurses.h>
#include <stdio.h>
#include <string.h>


// The function that main will call which will control the whole editor
void editor(const char *filename) {
    initscr();            // Initialize the screen for the editor
    raw();                // Disable line buffering
    keypad(stdscr, TRUE); // Enable function keys and arrow keys
    noecho();             // Disable character echoing
    
    CircularQueue stack;
    CircularQueue reStack ;
    if(enable_undo_redo) // we check if we can use undo redo here if it false we set it as is
    {
        initTracking(&stack);//undo
        initTracking(&reStack);//redo 
    }
    
    refresh(); 

    while (1) {
        printlines(); // moved it to a function// 
        ch = getch(); 
        switch (ch) {
            case KEY_RESIZE:
                printlines();
                break;
      
            case KEY_LEFT:
                if (col > 0) {
                    col--; // Move left if not at the start of the line
                }
                break;

            case KEY_RIGHT:
                if (col < (int)strlen(lines[row])) {
                    col++; // Move right if not past the last character of the line
                }
                break;

            case KEY_UP:
                if (row > 0) { // Prevent moving above the first line
                    row--; // Move to the previous line
                    adjust_cursor_col(lines, &row, &col); // Adjust cursor if necessary
                }
                break;

            case KEY_DOWN:
                if (row < line_count) { // Prevent moving below the last line
                    row++; // Move to the next line
                    adjust_cursor_col(lines, &row, &col); 
                }
                break;

            case KEY_BACKSPACE: // delete character (this case gave me a lot of Segmentation fault and i hate it)
                delete_char(1);
                break;

            case '\n': // Handle Enter key (move to the next line)
                if (line_count < MAX_LINES - 1) {
                    // Shift all lines below the current one down by one
                    for (int i = line_count; i > row; i--) {
                        strncpy(lines[i + 1], lines[i], MAX_LINE_LENGTH);
                    }

                    // Move part of the current line after the cursor to the next line
                    strncpy(lines[row + 1], &lines[row][col], MAX_LINE_LENGTH - col);
                    lines[row + 1][MAX_LINE_LENGTH - 1] = '\0'; // Null-terminate the new line
                    lines[row][col] = '\0';
                    //push(&stack, '\0',row,col);
                    line_count++;  
                    row++;         
                    col = 0;       
                }
                break;
            case '\t': // tab
                if (col + 4 < MAX_LINE_LENGTH){
                    for (int i = 0; i<4; i++){
                    memmove(&lines[row][col +1], &lines[row][col], strlen(&lines[row][col]) + 1);
                    lines[row][col]= ' ';
                    if(enable_undo_redo) push(&stack, ' ',row,col); // this will fill 4 places instead of just 1. FIX LATER
                    col++;
                    }
                    
                }

                break;
            
            case 27: // Escape key to exit
                endwin();
                return;
            case 26: // ctrl +z undo function
                if(enable_undo_redo) pop(&stack, &reStack);
                break;
            case 25: // ctrl +y redo function
                redo(&reStack, &stack);
                break;
            case 8: //ctrl+h show the help box new stuff will be added to it later on
                helpFunc();
                break;
            case 19:
                save_file(filename);
                break;
            default: // Handle regular character input
                if (col < MAX_LINE_LENGTH - 1 && row < MAX_LINES) {
                    memmove(&lines[row][col + 1], &lines[row][col], strlen(&lines[row][col]) + 1); // Shift characters to the right
                    lines[row][col] = ch; // Insert the character at the current position
                    if(enable_undo_redo) push(&stack, ch,row,col);
                    col++; // Move cursor to the right after insertion
                }
                break;
        }
    }
    endwin();
}
