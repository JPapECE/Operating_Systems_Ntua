
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = process

all: $(TARGET)

$(TARGET): process.c
        $(CC) $(CFLAGS) -o $(TARGET) process.c

run: $(TARGET)
        ./$(TARGET)

clean:
        rm -f $(TARGET)
