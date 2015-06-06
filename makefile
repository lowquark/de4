
all:
	gcc -o test_de4 main.c src/de4.c src/event.c src/state.c -g -std=c99 -Wall
