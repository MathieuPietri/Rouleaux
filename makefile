CC=gcc
OPT=-Wall -std=c17
LIB=-lglpk

all: test

test: test.c test.h
	$(CC) $< $(OPT) $(LIB)