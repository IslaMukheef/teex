# Teex - Terminal Text Editor

**Teex** is a terminal-based text editor written in C using the ncurses library. This project is a work in progress, with features and bug fixes being added in my free time.

## Features

- Basic text editing (inserting, deleting, and moving text)
- Arrow key navigation (up, down, left, right)
- File saving (`Ctrl+S`)
- Open and edit existing files by providing the file name as a command-line argument
- Quit using the `Esc` key

## Keybindings

- **Arrow Keys**: Move the cursor up, down, left, and right.
- **Backspace**: Delete the character before the cursor, with handling for merging lines and deleting empty lines.
- **Enter**: Create new line and splits the current line.
- **Esc**: Exit the editor.
- **Ctrl+S**: Save the current file.

## How to Use


1. Compile the program:

   ```bash
   gcc teex.c -o teex -lncurses
   ```

2. Run the editor with a file name:

   ```bash
   ./teex filename.txt
   ```

   If the file doesn't exist, it will be created. If it exists, you can edit and save it.


## Contributing

Feel free to open issues or submit pull requests to help improve the project. Any feedback is appreciated!
