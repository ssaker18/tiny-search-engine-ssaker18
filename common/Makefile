# Makefile for common library
#   Builds and compiles all these utility functions into a library.
#   Sebastian Saker, cs50

# object files, and the target library
OBJS = pagedir.o word.o index.o
LIB = common.a


# add -DMEMTEST for memory tracking report in indexer
FLAGS = # -DMEMTEST  

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make

# Build the library by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)


# Dependencies: object files depend on header files
pagedir.o: pagedir.h word.h ../libcs50/memory.h ../libcs50/webpage.h ../libcs50/file.h ../libcs50/counters.h ../libcs50/hashtable.h
word.o: word.h
index.o: index.h ../libcs50/memory.h ../libcs50/webpage.h ../libcs50/file.h ../libcs50/counters.h ../libcs50/hashtable.h

.PHONY: clean

# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o
