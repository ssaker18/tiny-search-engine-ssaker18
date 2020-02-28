#!/bin/sh
# Testing script for querier.c
# By: Sebastian Saker
# Date: Feb 18, 2020
#
# usage: bash -v memtesting.sh

#####################################
### These tests should fail ###

# 1 argument
valgrind -s --leak-check=yes --show-leak-kinds=all ./querier


# 2 arguments
valgrind -s --leak-check=yes --show-leak-kinds=all ./querier ../pagedir/letters-depth-3

# 3 arguments, false directory/file
valgrind -s --leak-check=yes --show-leak-kinds=all ./querier ../pagedir/letterswdawdawd ../pagedir/fakeFilename


######################################
### These tests should pass ####

valgrind -s --leak-check=yes --show-leak-kinds=all ./querier ../pagedir/letters-depth-3 ../pagedir/letters-index-3


