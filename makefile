
SRCDIR=src/
SOURCEFILES=main.c $(SRCDIR)de4.c $(SRCDIR)state.c $(SRCDIR)prop.c $(SRCDIR)entity.c $(SRCDIR)dump.c

INCLUDEDIR=./include/
test_de4: $(SOURCEFILES) $(INCLUDEFILES)
	gcc -o $@ -O3 $(SOURCEFILES) -I$(INCLUDEDIR) -g -std=c99 -Wall -fopenmp
