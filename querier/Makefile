# Makefile for 'querier' module
#
# By Sebastian Saker, February2020


CC= gcc
CFLAGS= -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
#CFLAGS= -Wall -pedantic -std=c11 -ggdb -I../libcs50 -L../libcs50 -I../common -L../common
PROG = querier
OBJS = querier.o
#LIBS = ../libcs50/libcs50.a ../common/common.a
LIBS = ../common/common.a ../libcs50/libcs50.a

MAKE = make

all: querier

querier: querier.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

#object files depend on include files
querier.o: ../common/pagedir.h ../common/index.h ../common/word.h ../libcs50/counters.h ../libcs50/file.h ../libcs50/hashtable.h ../libcs50/webpage.h ../libcs50/memory.h
#querier.o: ../common/common.a ../libcs50/libcs50.a


.PHONY: clean test valgrind

clean:
	rm -f $(PROG)
	rm -f *~ *.o
	rm -rf *.dSYM
	rm -f stocks
	rm -f *core*
	rm -f *.out

test:
	bash -v testing.sh

valgrind:
	bash -v memtesting.sh
