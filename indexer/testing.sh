#!/bin/sh
# Testing script for indexer.c and indextester.c
# By: Sebastian Saker
# Date: Feb 18, 2020
#
# usage: bash -v testing.sh

#####################################
### These tests should fail ###
# 1 argument
./indexer

./indextester

# 2 arguments
./indexer fakedir

./indextester fakefile

# 3 arguments, false directory/file
./indexer fakedir fakefile

./indextester oldfakefile newfakefile

######################################
### These tests should pass ####

./indexer ../pagedir/letters-depth-0 test.index

./indextest ../pagedir/letters-index-0 newindextest.index
