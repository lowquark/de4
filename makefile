
SRCFILES=src/de4.c src/dump.c src/entity.c src/prop.c src/state.c

libde4.a: $(patsubst %.c,%.o,$(SRCFILES))
	ar rcs $@ $^

%.o: %.c
	gcc -c -o $@ $< -Wall -std=c99 -g -I./include/
