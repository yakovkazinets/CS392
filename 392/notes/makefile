C      = g++
C_FILE = $(wildcard *.c)
TARGET   = $(patsubst %.c,%,$(C_FILE))
CFLAGS = -g -Wall -Werror -pedantic-errors -fmessage-length=0

all:
	$(C) $(CFLAGS) $(C_FILE) -o $(TARGET)
clean:
	rm -f $(TARGET) $(TARGET).exe
