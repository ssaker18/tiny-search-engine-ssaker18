# Makefile for 'crawler' module
#
# By Sebastian Saker, February2020


CC= gcc
CFLAGS= -Wall -pedantic -std=c11 -ggdb -I../libcs50 -L../libcs50
PROG = crawler
OBJS = crawler.o
LIBS = ../libcs50/libcs50.a

MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

#object files depend on include files
crawler.o: ../libcs50/bag.h ../libcs50/set.h ../libcs50/file.h ../libcs50/hashtable.h ../libcs50/webpage.h ../libcs50/memory.h 

.PHONY: clean test valgrind

clean:
	rm -f $(PROG)
	rm -f *~ *.o
	rm -rf *.dSYM
	rm -f stocks
	rm -rf data*
	rm -f *core*

test:
	bash -v testing.sh &> test.out

valgrind:
	bash -v memtesting.sh &> memtest.out

