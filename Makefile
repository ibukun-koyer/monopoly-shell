CC=gcc
CFLAGS=-lncurses

all: operations_monopoly.c
	$(CC) operations_monopoly.c $(CFLAGS)
clean: 
	rm -rf *o a.out
