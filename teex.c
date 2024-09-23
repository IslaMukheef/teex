#include <ncurses.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define MAX_LINES 1000
int ch;
int row = 0, col = 0;                   
char lines[MAX_LINES][MAX_LINE_LENGTH];  
int line_count = 0;  
// Function to adjust the cursor's column position if it's beyond the current line's length
void adjust_cursor_col(char lines[MAX_LINES][MAX_LINE_LENGTH], int *row, int *col) {
    if (*col > (int)strlen(lines[*row])) {
        *col = strlen(lines[*row]); // Move the cursor to the last char if the line is shorter than the current col position
    }
}
void save_file(const char *filename){
    FILE *file = fopen(filename,"w");
    if (!file){
        printw("Couldnt save the file");
        return;
    }
    for (int i = 0;i < line_count; i++){
        fprintf(file, "%s\n", lines[i]);
    }
    fclose(file);
}

void read_file(const char * filename){
    FILE *file =fopen(filename,"r");
    if (!file) return;

    char buffer[MAX_LINE_LENGTH];
    while(fgets(buffer,MAX_LINE_LENGTH, file) && line_count < MAX_LINES){
        strncpy(lines[line_count],buffer, MAX_LINE_LENGTH -1);
        lines[line_count][strlen(buffer)-1] = '\0'; // remove new lines char
        line_count++;
    }
    fclose(file);
    
}
void editor(const char *filename) {
    initscr();            // Initialize the screen for the editor
    raw();                // Disable line buffering
    keypad(stdscr, TRUE); // Enable function keys and arrow keys
    noecho();             // Disable character echoing

                       

    refresh(); 

    while (1) {
        clear();

       
        for (int i = 0; i <= line_count; i++) {
            mvprintw(i, 0, "%s", lines[i]); 
        }

        
        move(row, col);
        refresh();

        ch = getch(); 
        switch (ch) {
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
                if (col > 0) {
                    memmove(&lines[row][col - 1], &lines[row][col], strlen(&lines[row][col]) + 1);
                    col = (row == 0 && col == 0) ? 0 : col -1; // Move cursor back after deletion
                } 
                else if(col ==0 && strlen(lines[row]) == 0 ){ //deletes empty lines
                    if (row > 0) {
                        for (int i = row; i < line_count; i++) {
                            strncpy(lines[i], lines[i+1], MAX_LINE_LENGTH); // moves lines up
                        }
                        col = strlen(lines[row - 1]); 
                        row--;
                        line_count--;
                    }
                }
                else if (col == 0 && row > 0){
                        strcat(lines[row-1], lines[row]); // merge current line with the one upper
                        for (int i = row; i < line_count; i++) {
                            strncpy(lines[i], lines[i+1], MAX_LINE_LENGTH); // moves lines up
                        }
                        col = strlen(lines[row - 1]);
                        row--;
                        line_count--;
                    }
                clear(); // Clear the screen after deleting a character
                
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

                    line_count++;  
                    row++;         
                    col = 0;       
                }
                break;

            case 27: // Escape key to exit
                endwin();
                return;
            case 19:
                save_file(filename);
                break;
            default: // Handle regular character input
                if (col < MAX_LINE_LENGTH - 1 && row < MAX_LINES) {
                    memmove(&lines[row][col + 1], &lines[row][col], strlen(&lines[row][col]) + 1); // Shift characters to the right
                    lines[row][col] = ch; // Insert the character at the current position
                    col++; // Move cursor to the right after insertion
                }
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    char filename[256];
    if (argc < 2) {
        strcpy(filename, "isla.txt");
    } else {
        strcpy(filename, argv[1]);
        read_file(filename);
    }
    editor(filename); // Start the text editor
    return 0;
}
