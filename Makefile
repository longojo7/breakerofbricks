# Makefile for Brick Breaker

CC=gcc
CFLAGS=-Wall -g
 
all: break.c
	$(CC) $(CFLAGS) -o break break.c -lncurses
 
clean:
	rm break
	rm -r break.dSYM
 
