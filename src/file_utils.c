#include "../include/teex.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>



//This file's functions is used to handel file open,close,,save
//file name will be isla.txt if no name is given (check main.c)


void save_file(const char *filename){
    FILE *file = fopen(filename,"w");
    if (!file){
        printw("Couldnt save the file");
        return;
    }
    for (int i = 0;i <= line_count; i++){
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
