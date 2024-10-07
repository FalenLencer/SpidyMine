CC = gcc
CFLAGS = -Wall -I. 
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

.PHONY: all clean

all: game

game: Main.c SpidyLib.o
	$(CC) -o game Main.c SpidyLib.o $(LIBS) $(CFLAGS)

SpidyLib.o: SpidyLib.c SpidyLib.h
	$(CC) -c SpidyLib.c $(CFLAGS)

clean:
	rm -f *.o game
