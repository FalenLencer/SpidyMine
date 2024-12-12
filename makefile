CC = gcc
CFLAGS = -Wall -I. 
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

.PHONY: all clean

all: game

game: Main.c SpidyLib.o CalculLib.o
	$(CC) -o game Main.c SpidyLib.o CalculLib.o $(LIBS) $(CFLAGS)

SpidyLib.o: SpidyLib.c SpidyLib.h
	$(CC) -c SpidyLib.c $(CFLAGS)

CalculLib.o: CalculLib.c CalculLib.h
	$(CC) -c CalculLib.c $(CFLAGS)

clean:
	rm -f *.o game
