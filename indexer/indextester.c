/* 
 * indextester.c - program for CS50 indexer module
 *
 * usage: ./indextest oldIndexFilename newIndexFilename
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
#include "../common/index.h"
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
int hashTableSize(char *old_fname);
/**************** main ****************/
int main(const int argc, char *argv[])
{

    hashtable_t *inverted_index; //Contains words as keys, countersets as the respective item for each word
    int slots = HT_SIZE;
    char *old_fname = argv[1]; // oldIndexFilename
    char *new_fname = argv[2]; // newIndexFilename

    if (validateArgs(argc, argv) != 0) //perform arguement validations
    {
        return 1; //an arguement validation has failed
    }
    slots = hashTableSize(old_fname);

    inverted_index = hashtable_new(slots); //create our hashtable, remember to call delete on it
    if (inverted_index == NULL)
    {
        fprintf(stderr, "hashtable_new failed\n");
        return 2;
    }

    index_load(inverted_index, old_fname); // load our inverted index from the old index file

    index_save(inverted_index, new_fname); // save our retrieved inverted index to the designated new file

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
//function will validate the arguements for indextester.c
//returns non-zero integer if a validation fails
int validateArgs(const int argc, char *argv[])
{
    char *program = argv[0]; // program name
    // check input arguments: # of arguments
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s oldIndexFilename newIndexFilename\n", program); //program MUST always have 3 arguements(excluding the program name)
        return 1;
    }
    
    char *old_fname = argv[1]; // oldIndexFilename
    char *new_fname = argv[2]; // newIndexFilename

    // check input arguments: oldIndexFilename
    FILE *fp1 = fopen(old_fname, "r"); //I might need to check 'errno' for this!
    if (fp1 == NULL)
    {
        fprintf(stderr, "%s must be a valid filepath name\n", old_fname);
        return 2;
    }
    fclose(fp1); //close the oldIndexFilename file

    FILE *fp2 = fopen(new_fname, "w"); //I might need to check 'errno' for this!
    if (fp2 == NULL)
    {
        fprintf(stderr, "%s must be a valid filepath name\n", new_fname);
        return 3;
    }
    fclose(fp2); //close the oldIndexFilename file
    return 0;    //return 0 if all validation tests are successful
}

//calculate an approximate size for our hashtable based on number of lines in the indexfile!
int hashTableSize(char *old_fname)
{
    FILE *fp = fopen(old_fname, "r");
    int toReturn = lines_in_file(fp);
    toReturn *= 2;
    fclose(fp);
    return toReturn;
}
