CC = gcc
CFLAGS = -Wall

all: ts

ts: ts.c logger.c
	$(CC) $(CFLAGS) -o ts ts.c logger.c -lsensors

clean:
	rm -f ts *.log
