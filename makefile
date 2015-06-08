
SOURCEFILES=$(SRCDIR)de4.c $(SRCDIR)state.c $(SRCDIR)prop.c $(SRCDIR)entity.c $(SRCDIR)dump.c

INCLUDEDIR=./include/
test_de4: $(INCLUDEFILES)
	gcc -o $@ main.c src/de4.c src/state.c src/prop.c src/entity.c src/dump.c -I$(INCLUDEDIR) -g -std=c99 -Wall
