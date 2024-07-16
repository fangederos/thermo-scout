CC = gcc
CFLAGS = -Wall

all: ts

ts: ts.o logger.o sensor.o
	$(CC) $(CFLAGS) -o ts ts.o logger.o sensor.o -lncurses -lsensors

ts.o: ts.c logger.h sensor.h
	$(CC) $(CFLAGS) -c ts.c

logger.o: logger.c logger.h
	$(CC) $(CFLAGS) -c logger.c

sensor.o: sensor.c sensor.h logger.h
	$(CC) $(CFLAGS) -c sensor.c

clean:
	rm -f *.o ts
