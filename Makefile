PACKAGE = \"entris\"
VERSION = \"0.1\"
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
OSFLAGS = -DLINUX
else ifeq ($(UNAME), Darwin)
OSFLAGS = -DMACOSX
else ifeq ($(UNAME), Solaris)
OSFLAGS = -DSOLARIS
endif

CC = gcc
EXTRA_CFLAGS = -Wall
DEBUG = -ggdb
LIBS = -lncurses
CFLAGS = $(DEBUG) $(EXTRA_CFLAGS) $(OSFLAGS)
DEFS = -DPACKAGE=$(PACKAGE) -DVERSION=$(VERSION) -DTIOCGWINSZ

all: main.o interface.o game.o util.o game-board.o
	$(CC) $(CFLAGS) $(LIBS) main.o interface.o game-board.o game.o util.o -o entris

main.o: main.c
	$(CC) $(CFLAGS) $(DEFS) -c main.c

interface.o: interface.c interface.h
	$(CC) $(CFLAGS) $(DEFS) -c interface.c

game.o: game.c game.h
	$(CC) $(CFLAGS) $(DEFS) -c game.c

game-board.o: game-board.c game.h
	$(CC) $(CFLAGS) $(DEFS) -c game-board.c

util.o: util.c util.h
	$(CC) $(CFLAGS) $(DEFS) -c util.c

entris.o: entris.c entris.h
	$(CC) $(CFLAGS) $(DEFS) -c entris.c

clean:
	rm -f entris *.o other
