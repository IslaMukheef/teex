CC = gcc
CFLAGS = -Wall -g
INCLUDE = -Iinclude
SRC = main.c src/file_utils.c src/editor_utils.c src/editor.c
TARGET = teex

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INCLUDE) $(SRC) -o $(TARGET) -lncurses

clean:
	rm -f $(TARGET)
