#!/bin/sh
# Testing script for crawler.c
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
./crawler

# 2 arguments
./crawler $seedURL

# 3 arguments
./crawler $seedURL data

# 4 arguments + externalURL
./crawler $externalURL data 2

######################################
### These tests should pass ####

# at depth 0
./crawler $seedURL data 0

### etc.
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html data 0

./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html data 1

./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html data 2

./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html data 3

./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html data 4

./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html data 5

./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html data 0


#./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ wikidata 0

#./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ wikidata 1

#./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ wikidata 2
