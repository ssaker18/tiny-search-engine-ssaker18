/* 
 * index.h - header file for CS50 pagedir module
 *
 * Sebastian Saker, cs50
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdbool.h>

/**************** global types ****************/

/**************** functions ****************/

/******** helper to save item in hashtable ********/
void save_item(void *arg, const char *key, void *item);

/******** helper to save items in counters ********/
void save_counts(void *arg, const int key, int count);

/******** delete counters item ********/
void index_save(void *inverted_index, char *fname);

//function for saving index to the designated indexFilename
void ct_delete(void *item);

/**************** index_load ****************/
//function shall load our inverted index and store it into
//a hashtable. fname is the old index file which we must parse through
//in order to build our inverted index.
void index_load(void *index, char *fname);

#endif // __INDEX_H
