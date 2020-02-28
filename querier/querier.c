/* 
 * querier.c - program for CS50 querier module
 *
 * usage: ./querier pageDirectory indexFilename
 * 
 * In validateArgs and main: code from activity fetchweb-solution.c used for input validations
 * Also borrowed from function save_page from fetchweb-solution.c
 * getNextWord function was inspired by webpage.c's function
 * Borrowed functions and parts of main from iterator/save-solution.c activity
 * Borrowed code from set_iterate3.c activity for 'or' and 'and' queries
 * Borrowed code from inserver-split-solution.c activity for sep_words function
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
//#include "../libcs50/counters.h"
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/file.h"
#include "../libcs50/webpage.h"
#include "../libcs50/memory.h"

#define HT_SIZE 500 //macro to define the number of slots in our hashtable

/**************** global types ****************/
typedef struct pageinfo
{
    int id;
    int score;
} pageinfo_t;

/**************** local function prototypes ****************/
int validateArgs(const int argc, char *argv[]);
int hashTableSize(char *fname);
void queryTime(hashtable_t *index, char *dirname);
int sep_words(char input[], char **words);
int validateQuery(char **words, int size);
void printQuery(char **words, int size);
void rankQuery(hashtable_t *index, char **words, int size, char *dirname);
char *extract_url(char *dir, int id);
pageinfo_t *pageinfo_new(int id, int score);
void pagecount(void *arg, const int key, const int count);
void pageinsert(void *arg, const int key, const int count);
static void counters_merge(counters_t *countersA, counters_t *countersB);
static void counters_merge_helper(void *arg, const int key, const int item);
static void counters_intersect(counters_t *countersA, counters_t *countersB);
static void counters_intersect_helper(void *arg, const int key, const int item);
void itemprint(FILE *fp, const char *key, void *item);
void productScan(hashtable_t *index, counters_t *docscore, char **words, int size);
void sumScan(hashtable_t *index, counters_t *docscore, char **words, int size);
/**************** main ****************/
int main(const int argc, char *argv[])
{

    hashtable_t *inverted_index; //Contains words as keys, countercounterss as the respective item for each word
    int slots = HT_SIZE;
    char *dir = argv[1];   // pageDirectory
    char *fname = argv[2]; // indexFilename

    if (validateArgs(argc, argv) != 0) //perform arguement validations
    {
        return 1; //an arguement validation has failed
    }
    slots = hashTableSize(fname);

    inverted_index = hashtable_new(slots); //create our hashtable, remember to call delete on it
    if (inverted_index == NULL)
    {
        fprintf(stderr, "hashtable_new failed\n");
        return 2;
    }

    index_load(inverted_index, fname); // load our inverted index from the given index file
    hashtable_print(inverted_index, stdout, itemprint);
    queryTime(inverted_index, dir);
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
/* see directory libcs50 and common for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */

/**************** validateArgs ****************/
//function will validate the arguements for querier.c
//returns non-zero integer if a validation fails
int validateArgs(const int argc, char *argv[])
{
    char *program = argv[0]; // program name
    // check input arguments: # of arguments
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s pageDirectory indexFilename\n", program); //querier MUST always have 2 arguements(excluding the program name)
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
    FILE *fp = fopen(fname, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "%s must be a valid filepath name\n", fname);
        return 3;
    }
    fclose(fp); //close the index file
    return 0;   //return 0 if all validation tests are successful
}

//calculate an approximate size for our hashtable based on number of lines in the indexfile!
int hashTableSize(char *fname)
{
    FILE *fp = fopen(fname, "r");
    int toReturn = lines_in_file(fp);
    toReturn *= 2;
    fclose(fp);
    return toReturn;
}

//function for user interaction in entering into standard input for
//querier to perform queries on the search terms provided
void queryTime(hashtable_t *index, char *dirname)
{
    char *line; //for holding the string of the user's input
    char **words;
    while (!feof(stdin))
    {
        printf("Query? ");
        line = readlinep();
        if (line == NULL)
        {
            return;
        }                                                     //REMEMBER TO FREE; scanning a line of user's input
        words = calloc(strlen(line) / 2 + 1, sizeof(char *)); //remember to free!
        int word_count = sep_words(line, words);
        if (word_count == 0)
        {
            free(words);
            free(line);
            continue; //move on to the next search line
        }
        if (validateQuery(words, word_count) != 0)
        {
            free(words);
            free(line);
            continue;
        }
        printQuery(words, word_count);

        //time to do the rankings!
        rankQuery(index, words, word_count, dirname);

        //After being done with search
        // for (int k = 0; k < word_count; k++)
        // {
        //     free(words[k]);
        // }
        free(line);
        free(words);
        //free(line);
        //REMEMBER TO ALSO FREE EACH INDIVIDUAL WORD IN words due to to_lowercase
    }
}

// separate an input string into words, fill in words array of strings
int sep_words(char input[], char **words)
{

    // max # of pointers is the max # of words
    //   words = calloc(strlen(input)/2+1, sizeof(char*));

    // TODO: add your code here to split words...
    int count = 0;
    for (char *rest = input; *rest != '\0';)
    {
        char *start; // starting address of a word
        // seek the start of a word
        for (start = rest; isspace(*start) && *start != '\0'; start++)
        {
        }
        // seek the end of the word
        for (rest = start; !isspace(*rest) && *rest != '\0'; rest++)
        {
        }

        if (*rest != '\0')
        {
            *rest++ = '\0'; // insert '\0' to signal the end of a word
        }

        if (*start != '\0')
        {
            words[count++] = start;
        }
    }
    //clean up and make lower case
    for (int j = 0; j < count; j++)
    {
        printf("%s\n", words[j]);
        to_lowercase(words[j]); //NEEDS TO BE FREE'd
    }
    for (int k = 0; k < count; k++)
    {
        printf("%s\n", words[k]);
    }
    return count;
}

//validate one line of user's search query
int validateQuery(char **words, int size)
{
    //first check if first or last word is 'and' or 'or'
    if (strcmp(words[0], "or") == 0 || strcmp(words[0], "and") == 0)
    {
        fprintf(stderr, "Error: '%s' cannot be first\n", words[0]);
        return 1;
    }
    if (strcmp(words[size - 1], "or") == 0 || strcmp(words[size - 1], "and") == 0)
    {
        fprintf(stderr, "Error: '%s' cannot be last\n", words[size - 1]);
        return 1;
    }
    //second, check if there are any words less than 3 chars besides 'or'
    //also check if there are any non-alphabeticals contained in words
    //lastly, check if there are any adjacent 'and' or 'or'
    //also check if there are any words less than 3 chars
    for (int i = 0; i < size; i++)
    {
        if (strcmp(words[i], "or") != 0) //if it is not 'or'
        {
            if (is_alpha_str(words[i]) == false) //check if is alphabetical
            {
                fprintf(stderr, "Error: '%s' must not contain any non-alphabeticals and must be at least 3 characters long\n", words[i]);
                return 2;
            }
            if (strlen(words[i]) < 3)
            { //if less than three chars and is not 'or'
                fprintf(stderr, "Error: '%s' must not contain any non-alphabeticals and must be at least 3 characters long, besides 'or'\n", words[i]);
                return 3;
            }
        }

        if (strcmp(words[i], "or") == 0 || strcmp(words[i], "and") == 0)
        {
            if (i == size - 1)
            {
                continue; //we do not want a seg fault
            }
            //check if there are successive "or" or "and"
            if (strcmp(words[i + 1], "or") == 0 || strcmp(words[i + 1], "and") == 0)
            {
                fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent\n", words[i], words[i + 1]);
                return 1;
            }
        }
    }
    return 0; //query is valid!
}

void printQuery(char **words, int size)
{
    printf("Query: ");
    for (int i = 0; i < size; i++)
    {
        printf("%s ", words[i]);
    }
    printf("\n");
}

//rank user's line for query search terms amongst documents within the index
void rankQuery(hashtable_t *index, char **words, int size, char *dirname) //size is the number of words
{
    int numOfpages = 0;
    pageinfo_t **pages;
    counters_t *docscore;
    char *url;

    //counters's key shall be the page URL
    //the items will be an array consisting of the ID and the score

    if (size == 1)
    { //DEFINE A DATA STRUCT THAT CAN HOLD URL, ID, AND SCORE!
        //PUT THOSE STRUCTS IN AN ARRAY AND SORT BY ORDER
        //IN ORDER TO GET INFORMATION ABOUT A WORD, RETRIEVE ITS COUNTERS FROM INDEX (hashtable_find)
        char *word;
        word = words[0]; //first find out size of array to make
        char *url;

        counters_t *wordcount;
        wordcount = hashtable_find(index, word);

        counters_iterate(wordcount, &numOfpages, pagecount);
        pages = calloc(numOfpages, sizeof(struct pageinfo *)); //remember to free
        counters_iterate(wordcount, pages, pageinsert);        //remember to free
        //now we have pages in order and containing all pageinfo's
        if (numOfpages == 0)
        {
            printf("No documents match. \n");
            free(pages);
            printf("-----------------------------------------------\n");
            return;
        }
        printf("Matches %d documents (ranked): \n", numOfpages);
        for (int i = 0; i < numOfpages; i++)
        {
            url = extract_url(dirname, pages[i]->id); //REMEMBER TO FREE WHEN DONE!
            //REMEMBER TO ADD FORMAT SPECIFIERS FOR PRINTING OUTPUT TO BE CORRECT!
            printf("score %d doc %d: %s\n", pages[i]->score, pages[i]->id, url);
            free(url);
            free(pages[i]);
        }
        printf("-----------------------------------------------\n");
        free(pages);
    }
    else //loop through and add to counters_t for entire query
    {
        docscore = assertp(counters_new(), "failed to created counterset for all document scores\n");
        productScan(index, docscore, words, size);
        sumScan(index, docscore, words, size);
        //now we need to find the size of the array to print
        counters_iterate(docscore, &numOfpages, pagecount);
        pages = calloc(numOfpages, sizeof(struct pageinfo *)); //remember to free
        counters_iterate(docscore, pages, pageinsert);         //remember to free
        //now we have pages in order and containing all pageinfo's
        if (numOfpages == 0)
        {
            printf("No documents match. \n");
            free(pages);
            printf("-----------------------------------------------\n");
            return;
        }
        printf("Matches %d documents (ranked): \n", numOfpages);
        for (int i = 0; i < numOfpages; i++)
        {
            url = extract_url(dirname, pages[i]->id); //REMEMBER TO FREE WHEN DONE!
            //REMEMBER TO ADD FORMAT SPECIFIERS FOR PRINTING OUTPUT TO BE CORRECT!
            printf("score %d doc %d: %s\n", pages[i]->score, pages[i]->id, url);
            free(url);
            free(pages[i]);
        }
        printf("-----------------------------------------------\n");
        free(pages);
        counters_delete(docscore);
    }
}

//Extracts url from pagedir, USER IS RESPONSIBLE FOR FREEING return string!
char *extract_url(char *dir, int id)
{
    char buffer[200];                                                             //This will be our string buffer used to append the id to the filepath
    snprintf(buffer, sizeof(int) + sizeof(char) * strlen(dir), "%s/%d", dir, id); //our buffer will now be our new page's file path
    FILE *fp = fopen(buffer, "r");                                                //creates an empty file for writing
    if (fp == NULL)
    {
        exit(1); //nothing to look for in this file, its unreadable or does not exist
    }
    char *toReturn;            //the url shouldnt be larger than 200 chars
    toReturn = freadlinep(fp); //REMEMBER TO FREE THE URLs!
    fclose(fp);
    return toReturn;
}

/**************** pageinfo_new ****************/
pageinfo_t *pageinfo_new(int id, int score)
{
    pageinfo_t *p = assertp(count_malloc(sizeof(pageinfo_t)), "pageinfo struct could not be created \n");

    if (p == NULL || id == 0)
    {
        return NULL; // error in allocating hashtable or not enough slots
    }
    else
    {
        // initialize contents of hashtable structure
        p->id = id;
        p->score = score;
        return p;
    }
}

// print a key's count integer with an epic introduction
void pagecount(void *arg, const int key, const int count)
{
    int *p;
    p = (int *)arg;
    (*p)++; //see if this works
}

//after it is called throughout counters_interate, should have sorted arr in descending order
void pageinsert(void *arg, const int key, const int count)
{
    pageinfo_t **arr = arg;
    for (int i = 0; i < sizeof(arr) - 1; i++)
    {
        if (arr[i] == NULL)
        {
            arr[i] = pageinfo_new(key, count);
            return;
        }
        else
        {
            if (arr[i]->score < count)
            {
                pageinfo_t *swap;
                for (int j = sizeof(arr) - 1; j > i; j--)
                {
                    swap = arr[j];
                    arr[j] = arr[j - 1];
                    arr[j - 1] = swap;
                }
                arr[i] = pageinfo_new(key, count);
                return;
            }
        }
    }
    arr[sizeof(arr) - 1] = pageinfo_new(key, count);
    return;
}

/* Merge the second counters into the first counters;
 * the second counters is unchanged.
 */
static void
counters_merge(counters_t *countersA, counters_t *countersB)
{
    counters_iterate(countersB, countersA, counters_merge_helper);
}

/* Consider one item for insertion into the other counters.
 * If the other counters does not contain the item, insert it;
 * otherwise, update the other counters's item with sum of item values.
 */
static void
counters_merge_helper(void *arg, const int key, const int item)
{
    counters_t *countersA = arg;
    int itemB = item;

    // find the same key in countersA
    int itemA = counters_get(countersA, key);
    if (itemA == 0)
    {
        // not found: insert it
        counters_set(countersA, key, itemB);
        //printf("\t%d added\n", key);
    }
    else
    {
        // add to the existing value
        counters_set(countersA, key, itemB + itemA);
        //printf("\t%d exists\n", key);
    }
}

/* Take the intersection of two counterss and store it in the first counters.
 * the second counters is unchanged.
 */
static void
counters_intersect(counters_t *countersA, counters_t *countersB)
{
    counters_iterate(countersA, countersB, counters_intersect_helper);
}

static void
counters_intersect_helper(void *arg, const int key, const int item)
{
    counters_t *countersB = arg;
    int itemA = item;

    int itemB = counters_get(countersB, key);
    if (itemB == 0)
    {
        itemA = 0; // not found this key in counters B, counters the int to zero
    }
    else
    {
        itemA = itemA < itemB ? itemA : itemB; // found the key, take the minimum
    }
}

void itemprint(FILE *fp, const char *key, void *item)
{
    fprintf(fp, "word: %s", key);
    counters_t *c;
    c = item;
    counters_print(c, fp);
}
void productScan(hashtable_t *index, counters_t *docscore, char **words, int size)
{
    counters_t *wordcount1;
    counters_t *wordcount2;
    counters_t *result;
    char *word1;
    char *word2;
    for (int i = 0; i < size - 1; i++)
    {
        if (strcmp(words[i + 1], "and") == 0) //'and' sequence
        {
            result = assertp(counters_new(), "failed to created counterset for result document scores for 'and'\n");
            word1 = words[i];
            word2 = words[i + 2];
            wordcount1 = hashtable_find(index, word1);
            wordcount2 = hashtable_find(index, word2);
            counters_merge(result, wordcount2);
            counters_intersect(result, wordcount1);
            counters_merge(docscore, result);
            free(result);
            i++; //skip 'and'
        }
        if (strcmp(words[i + 1], "or") == 0)
        {
            i++;
        }
        if (strcmp(words[i + 1], "or") != 0)
        {
            result = assertp(counters_new(), "failed to created counterset for result document scores for 'and'\n");
            word1 = words[i];
            word2 = words[i + 1];
            wordcount1 = hashtable_find(index, word1);
            wordcount2 = hashtable_find(index, word2);
            counters_merge(result, wordcount2);
            counters_intersect(result, wordcount1);
            counters_merge(docscore, result);
            free(result);
        }
    }
}
void sumScan(hashtable_t *index, counters_t *docscore, char **words, int size)
{
    counters_t *wordcount1;
    counters_t *wordcount2;
    char *word1;
    char *word2;
    for (int i = 0; i < size - 1; i++)
    {
        if (strcmp(words[i + 1], "and") == 0) //'and' sequence
        {
            i++; //skip 'and'
        }
        if (strcmp(words[i + 1], "or") == 0) //'or' sequence
        {
            word1 = words[i];
            word2 = words[i + 2];
            wordcount1 = hashtable_find(index, word1);
            wordcount2 = hashtable_find(index, word2);
            counters_merge(docscore, wordcount2);
            counters_merge(docscore, wordcount1);
            i++; //skip 'or'
        }
    }
}

