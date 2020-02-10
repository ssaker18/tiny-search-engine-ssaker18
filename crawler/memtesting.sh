#!/bin/sh
# Memory Testing script for crawler.c
# Author: Temi Prioleau
# Updated by: Sebastian Saker
# Date: Feb 9, 2020
#
# usage: bash -v testing.sh

# Define variables
seedURL=http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
externalURL=www.google.com
#####################################
### These tests should fail ###

# 1 argument
valgrind --leak-check=yes ./crawler

# 2 arguments
valgrind --leak-check=yes ./crawler $seedURL

# 3 arguments
valgrind --leak-check=yes ./crawler $seedURL data

# 4 arguments + externalURL
valgrind --leak-check=yes ./crawler $externalURL data 2

######################################
### These tests should pass ####

# at depth 0
valgrind -s --leak-check=yes ./crawler $seedURL data 0

### etc.
valgrind -s --leak-check=yes ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html data 0

valgrind -s --leak-check=yes ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html data1 1

valgrind -s --leak-check=yes ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html data2 2

valgrind -s --leak-check=yes ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html data3 3

valgrind -s --leak-check=yes ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html data4 4

valgrind -s --leak-check=yes ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html data5 5

#valgrind -s --leak-check=yes ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html data 0


#valgrind --leak-check=yes ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ wikidata 0

#valgrind --leak-check=yes ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ wikidata 1

#valgrind --leak-check=yes ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ wikidata 2
