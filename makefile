CC=gcc
OPT=-Wall -Wpedantic -std=c11 -g
LIB=-lglpk

all: test

test: test.c test.h
	$(CC) $< $(OPT) $(LIB) -o $@