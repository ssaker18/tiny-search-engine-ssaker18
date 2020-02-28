#!/bin/sh
# Testing script for querier.c
# By: Sebastian Saker
# Date: Feb 18, 2020
#
# usage: bash -v testing.sh

#####################################
### These tests should fail ###

# 1 argument
./querier


# 2 arguments
./querier ../pagedir/letters-depth-3

# 3 arguments, false directory/file
./querier ../pagedir/letterswdawdawd ../pagedir/fakeFilename


######################################
### These tests should pass ####

./querier ../pagedir/letters-depth-3 ../pagedir/letters-index-3


