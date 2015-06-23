
all: main.o ../libde4.a
	gcc -o de4_test $^ -g -std=c99 -Wall

../libde4.a:
	make -C ..

%.o: %.c
	gcc -c -o $@ $< -Wall -std=c99 -g -I../include/
