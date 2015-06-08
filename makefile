
all:
	gcc -o test_de4 main.c src/de4.c src/state.c src/prop.c src/entity.c -g -std=c99 -Wall
