# Yakov Kazinets and Tae Kim
# I pledge my honor that I have abided by the Stevens Honor System.
# used the same makefile as used in previous class assignments!
CC     = gcc
C_FILE = $(wildcard *.c)
TARGET = $(patsubst %.c,%,$(C_FILE))
CFLAGS = -g -Wall -Werror -pedantic-errors

all:
	$(CC) $(CFLAGS) $(C_FILE) -o $(TARGET) -lm
clean:
	rm -f $(TARGET) $(TARGET).exe
