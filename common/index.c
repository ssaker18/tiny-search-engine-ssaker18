#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //for checking if a directory is writable and exists
#include <stdbool.h>
#include "../libcs50/file.h"
#include "../libcs50/webpage.h"
#include "../libcs50/memory.h"
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/

/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h, file.h, webpage.h, memory.h, and hashtable.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */

/******** helper to save items in counters ********/
void save_counts(void *arg, const int key, int count)
{
    FILE *fp = arg;
    fprintf(fp, " %d %d", key, count);
}

/******** helper to save item in hashtable ********/
void save_item(void *arg, const char *key, void *item)
{
    FILE *fp = arg;
    counters_t *c = item;

    fprintf(fp, "%s:", key);
    counters_iterate(c, fp, save_counts);
    fprintf(fp, "\n");
}

/******** delete counters item ********/
void ct_delete(void *item)
{
    counters_t *ct = item;

    if (ct != NULL)
    {
        counters_delete(ct);
    }
}

//function for saving index to the designated indexFilename
void index_save(hashtable_t *inverted_index, char *fname)
{
    FILE *fp = fopen(fname, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "indexer: failed to open file%s\n", fname);
        return;
    }
    hashtable_iterate(inverted_index, fp, save_item);
    fclose(fp);
}

/**************** index_load ****************/
//function shall load our inverted index and store it into
//a hashtable. fname is the old index file which we must parse through
//in order to build our inverted index.
void index_load(hashtable_t *index, char *fname)
{
    FILE *fp;
    fp = fopen(fname, "r");
    char word[200];
    int ID;
    int count;
    counters_t *word_count;

    while (fscanf(fp, "%s", word) == 1) //while there is a word on the line
    {
        word_count = counters_new(); //create our counterset for our word
        while (fscanf(fp, "%d %d ", &ID, &count) == 2)
        { //get all ID's and counts
            counters_set(word_count, ID, count);
        }
        hashtable_insert(index, word, word_count); //store word and its counter set in index
    }
    fclose(fp);
    //finished loading index
}

