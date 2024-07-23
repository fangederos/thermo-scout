# Variables
CC = gcc
CFLAGS = -Wall -pg
LDFLAGS = -lncurses -lsensors
TARGET = ts
OBJS = ts.o logger.o sensor.o

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Compilation
ts.o: ts.c logger.h sensor.h
	$(CC) $(CFLAGS) -c ts.c

logger.o: logger.c logger.h
	$(CC) $(CFLAGS) -c logger.c

sensor.o: sensor.c sensor.h logger.h
	$(CC) $(CFLAGS) -c sensor.c

# Clean up
clean:
	rm -f *.o $(TARGET) gmon.out
