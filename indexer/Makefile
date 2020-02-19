# Makefile for 'indexer' module
#
# By Sebastian Saker, February2020


CC= gcc
CFLAGS= -Wall -pedantic -std=c11 -ggdb -I../libcs50 -L../libcs50 -I../common -L../common
PROG = indexer indextester
OBJS = indexer.o indextester.o
LIBS = ../libcs50/libcs50.a ../common/common.a

MAKE = make

all: indexer indextester

indexer: indexer.o ../libcs50/libcs50.a ../common/common.a
	$(CC) $(CFLAGS) $^ -o $@

indextester: indextester.o ../libcs50/libcs50.a ../common/common.a
	$(CC) $(CFLAGS) $^ -o $@

#object files depend on include files
indexer.o: ../common/pagedir.h ../common/index.h ../common/word.h ../libcs50/counters.h ../libcs50/file.h ../libcs50/hashtable.h ../libcs50/webpage.h ../libcs50/memory.h
indextester.o: ../common/index.h ../libcs50/counters.h ../libcs50/file.h ../libcs50/hashtable.h ../libcs50/webpage.h ../libcs50/memory.h

.PHONY: clean test valgrind

clean:
	rm -f $(PROG)
	rm -f *~ *.o
	rm -rf *.dSYM
	rm -f stocks
	rm -f *core*
	rm -f *.out
	rm -f *.index

test:
	bash -v testing.sh &> testing.out

valgrind:
	bash -v memtesting.sh &> memtesting.out

