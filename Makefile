CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = keylogger
SOURCES = main.c keyboard.c time_utils.c
HEADERS = keylogger.h
OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	sudo ./$(TARGET)

.PHONY: clean run
