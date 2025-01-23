# Teex - Terminal Text Editor

**Teex** is a terminal-based text editor written in C using the ncurses library. This project is a work in progress, with features and bug fixes being added in my free time.

## Features

- Show help(`Ctrl+H`)
- File explorer(`Ctrl+F`)
- Basic text editing (inserting, deleting, and moving text)
- Arrow key navigation (up, down, left, right)
- File saving (`Ctrl+S`)
- Tracking of recent actions and undo them with (`Ctrl+Z`) 
- Tracking of recently deleted chars with ctrl + z to restore it with(`Ctrl+Y`)
- Open and edit existing files by providing the file name as a command-line argument
- Quit using the `Esc` key

## Keybindings

- **Arrow Keys**: Move the cursor up, down, left, and right.
- **Backspace**: Delete the character before the cursor, with handling for merging lines and deleting empty lines.
- **Enter**: Create new line and splits the current line.
- **Esc**: Exit the editor.
- **Ctrl+S**: Save the current file.
- **Ctrl+Z**: Undo recent action.
- **Ctrl+Y**: Restore recently deleted chars.
- **Ctrl+H**: Show help window.
- **Ctrl+F**: Show file explorer window for current dir.

## How to Use


1. Compile the program:

   ```bash
     make
   ```

2. Run the editor with a file name:

   ```bash
   ./teex filename.txt
   ```

   If the file doesn't exist, it will be created. If it exists, you can edit and save it.


## Contributing & License
This project is licensed under the MIT License.
Feel free to open issues or submit pull requests to help improve the project. Any feedback is appreciated!

### Files Explained
- include 
    - teex.h ==> header file
      
- src
    - editor_utils.c ==> functions used by the editor(print, create widnow, etc)
    - editor.c       ==> this is the editor
    - file_utils.c   ==> file handling like create,close,open, etc
      
- main.c ==> The entry point of the program, making sure there is file or not.
- makefile ==> the build file for compiling the program.
