# Makefile for char-count.c

CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = char-count

all: $(TARGET)

$(TARGET): char-count.o
        $(CC) $(CFLAGS) -o $(TARGET) char-count.o

char-count.o: char-count.c
        $(CC) $(CFLAGS) -c char-count.c

clean:
        rm -f $(TARGET) char-count.o

.PHONY: all clean

