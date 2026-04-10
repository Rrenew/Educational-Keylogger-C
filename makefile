CC = gcc
CFLAGS = -Wall -Wextra
TARGET = keylogger

# Arquivos fonte
SOURCES = main.c input.c teclas.c utils.c

# Objetos
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

rebuild: clean all

run: all
	sudo ./$(TARGET)

run-no-sudo: all
	./$(TARGET)

.PHONY: all clean rebuild run run-no-sudo