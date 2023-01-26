CC=g++
CFLAGS=-c -ansi -Wall -pedantic-errors -O0

all: scapegoattree treemap

treemap: main_treemap.o
	$(CC) main_treemap.o -o treemap

scapegoattree: main_scapegoattree.o
	$(CC) main_scapegoattree.o -o scapegoattree

main_treemap.o: main_treemap.cpp
	$(CC) $(CFLAGS) main_treemap.cpp -o main_treemap.o

main_scapegoattree.o: main_scapegoattree.cpp
	$(CC) $(CFLAGS) main_scapegoattree.cpp -o main_scapegoattree.o

clean:
	rm *o
	rm scapegoattree
	rm treemap
