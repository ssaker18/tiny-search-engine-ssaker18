/* 
 * indexer.c - program for CS50 indexer module
 *
 * usage: ./indexer pageDirectory indexFilename
 * 
 * In validateArgs and main: code from activity fetchweb-solution.c used for input validations
 * Also borrowed from function save_page from fetchweb-solution.c
 * getNextWord function was inspired by webpage.c's function
 * Borrowed functions and parts of main from iterator/save-solution.c activity
 * Sebastian Saker
 * CS50, February 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //for checking if a directory is writable and exists
#include <stdbool.h>
#include <ctype.h>
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../common/word.h"
//#include "../libcs50/set.h"
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/file.h"
#include "../libcs50/webpage.h"
#include "../libcs50/memory.h"

#define HT_SIZE 500 //macro to define the number of slots in our hashtable

/**************** global types ****************/

/**************** local function prototypes ****************/
int validateArgs(const int argc, char *argv[]);
/**************** main ****************/
int main(const int argc, char *argv[])
{

    hashtable_t *inverted_index; //Contains words as keys, countersets as the respective item for each word
    int slots = HT_SIZE;
    char *dir = argv[1];   // pageDirectory
    char *fname = argv[2]; // indexFilename

    if (validateArgs(argc, argv) != 0) //perform arguement validations
    {
        return 1; //an arguement validation has failed
    }

    inverted_index = hashtable_new(slots); //create our hashtable, remember to call delete on it
    if (inverted_index == NULL)
    {
        fprintf(stderr, "hashtable_new failed\n");
        return 2;
    }

    index_build(inverted_index, dir); // build our inverted index

    index_save(inverted_index, fname); // save our inverted index in the designated file

    // clean up
    hashtable_delete(inverted_index, ct_delete);
    return 0;
}

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/
/* that is, visible outside this file */
/* see directory libcs50 for comments about global types */

/**************** global functions ****************/
/* that is, visible outside this file */
/* see directory libcs50 for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */

/**************** validateArgs ****************/
//function will validate the arguements for indexer.c
//returns non-zero integer if a validation fails
int validateArgs(const int argc, char *argv[])
{
    char *program = argv[0]; // program name
    // check input arguments: # of arguments
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s pageDirectory indexFilename\n", program); //Indexer MUST always have 3 arguements(excluding the program name)
        return 1;
    }
    char *dir = argv[1];   // pageDirectory
    char *fname = argv[2]; // indexFilename

    // check input arguments: pageDirectory is a directory that exists and was created by the crawler
    if (!isCrawlerDirectory(dir)) // if pageDirectory does not exist, is not readable or was not created by crawler
    {
        fprintf(stderr, "%s must be a directory that exists and is readable and was created by crawler\n", dir);
        return 2;
    }

    // check input arguments: indexFilename
    FILE *fp = fopen(fname, "w"); //I might need to check 'errno' for this!
    if (fp == NULL)
    {
        fprintf(stderr, "%s must be a valid filepath name\n", fname);
        return 3;
    }
    fclose(fp); //close the index file
    return 0;   //return 0 if all validation tests are successful
}

