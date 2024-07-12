# Makefile for thermo-scout

# Compiler and flags
CC = gcc
CFLAGS = -lncurses -Wall -Wextra -pedantic

# Executable name
TARGET = ts

# Source files
SRC = main.c logger.c

# Object files
OBJ = $(SRC:.c=.o)

# Default target
all: $(TARGET)

# Build the target executable
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# Compile source files into object files
%.o: %.c
	$(CC) -c $< $(CFLAGS)

# Clean target to remove compiled files
clean:
	rm -f $(TARGET) $(OBJ)

# Phony targets to avoid conflicts with files of the same name
.PHONY: all clean
