#!/bin/sh
# Testing script for indexer.c and indextester.c
# By: Sebastian Saker
# Date: Feb 18, 2020
#
# usage: bash -v testing.sh

#####################################
### These tests should fail ###

# 1 argument
valgrind --leak-check=yes ./indexer

valgrind --leak-check=yes ./indextester

# 2 arguments
valgrind --leak-check=yes ./indexer fakedir

valgrind --leak-check=yes ./indextester fakefile

# 3 arguments, false directory/file
valgrind --leak-check=yes ./indexer fakedir fakefile

valgrind --leak-check=yes ./indextester oldfakefile newfakefile

######################################
### These tests should pass ####

valgrind --leak-check=yes ./indexer ../pagedir/letters-depth-0 test.index

valgrind --leak-check=yes ./indextest ../pagedir/letters-index-0 newindextest.index
