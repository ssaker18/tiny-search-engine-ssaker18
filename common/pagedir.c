#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //for checking if a directory is writable and exists
#include <stdbool.h>
#include <ctype.h>
#include "../libcs50/file.h"
#include "../libcs50/webpage.h"
#include "../libcs50/memory.h"
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"
#include "./word.h"

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

/**************** newPageDirectory(char *dir) ****************/
//initialize page directory, call is responsible for closing the file returned afterwards!
FILE *newPageDirectory(char *dir)
{
    FILE *fp = fopen(dir, "w"); //I might need to check 'errno' for this!
    if (fp == NULL)
    {
        fprintf(stderr, "%s must be a valid filepath name\n", dir);
        return NULL;
    }
    return fp;
}

/**************** isCrawlerDirectory(char *dir) ****************/
//Check if our directory was created by the crawler (meaning it should contain a .crawler file)
bool isCrawlerDirectory(char *dir)
{
    if (dir == NULL)
    {
        return false; // error conditions: directory must have a name
    }
    char dirpath[200];    //initialize our string that will contain the filepath
    strcpy(dirpath, dir); //Place directory name as part of filepath
    char *toAppend = "/.crawler";
    strncat(dirpath, toAppend, strlen(toAppend)); //append toAppend to dirpath
    FILE *fp = fopen(dirpath, "r");
    if (fp == NULL)
    {
        return false; //either the dir is not a valid pathname, not a directory, not a readable directory,
        //it’s not a Crawler produced directory
    }
    fclose(fp);  //Remember to close our file!
    return true; //dir is a crawler made directory that exists and is readable
}

//THIS IS FROM CRAWLER WHICH I MESSED UP
/**************** save_page ****************/
//save page's information to a directory that will contain crawler's found pages
void save_page(webpage_t *page, const char *fname, int id)
{
    char buffer[50];  //This will be our string buffer used to append the id to the filepath
    char buffer2[50]; //This will be the string buffer that will be used to add in a
    // .crawler file to the directory

    snprintf(buffer2, 50, "%s/.crawler", fname);
    FILE *fp2 = fopen(buffer2, "w"); //creates a .crawler file for
    //checking if crawler created this directory
    assertp(fp2, "cannot create a .crawler file for this directory\n"); //check if file was created
    fclose(fp2);

    snprintf(buffer, 51, "%s/%d", fname, id);      //our buffer will now be our new page's file path
    FILE *fp = fopen(buffer, "w");                 //creates an empty file for writing
    assertp(fp, "cannot open file for writing\n"); //check if file can be created and wrote to

    fprintf(fp, "%s\n%d\n%s", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));

    fclose(fp);
}

//PLEASE NOTE: THIS CODE IS 'BASICALLY' copied from webpage.c's webpage_getNextWord
//This will get the next word in an html-styled string
char *html_getNextWord(char *html, int *pos)
{
    // make sure we have something to search, and a place for the result
    if (html == NULL || pos == NULL)
    {
        return NULL;
    }

    const char *doc = html; // the html document
    const char *beg;        // beginning of word
    const char *end;        // end of word

    // consume any non-alphabetic characters
    while (doc[*pos] != '\0' && !isalpha(doc[*pos]))
    {
        // if we find a tag, i.e., <...tag...>, skip it
        if (doc[*pos] == '<')
        {
            end = strchr(&doc[*pos], '>'); // find the close

            if (end == NULL || *(++end) == '\0')
            { // ran out of html
                return NULL;
            }

            *pos = end - doc; // skip over the <...tag...>
        }
        else
        {
            (*pos)++; // just move forward
        }
    }

    // ran out of html
    if (doc[*pos] == '\0')
    {
        return NULL;
    }

    // doc[*pos] is the first character of a word
    beg = &(doc[*pos]);

    // consume word
    while (doc[*pos] != '\0' && isalpha(doc[*pos]))
    {
        (*pos)++;
    }

    // at this point, doc[*pos] is the first character *after* the word.
    // so doc[*pos-1] is the last character of the word.
    end = &(doc[*pos - 1]);

    // 'beg' points to first character of the word
    // 'end' points to last character of the word
    int wordlen = end - beg + 1;

    // allocate space for length of new word + '\0'
    char *word = calloc(wordlen + 1, sizeof(char));
    if (word == NULL)
    { // out of memory!
        return NULL;
    }
    else
    {
        // copy the new word
        strncpy(word, beg, wordlen);
        return word;
    }
}

//MAKE IT SO IT DOES WORDS LESS THAN 3 CHARS,
// THEN VALIDATE THAT WITHIN querier.c
//check if the string is alphabetical (NO NUMBERS OR OTHER NON-ALPHABETICALS)
bool is_alpha_str(char *result)
{
    // if (strlen(result) < 3)
    // {
    //     return false; //We cannot have words less than 3 characters
    // }
    int status;
    int i = 0;
    for (i = 0; i < strlen(result); i++)
    {
        status = isalpha(result[i]);
        if (status == 0)
        {
            return false;
        }
    }
    return true;
}

/**************** extract_words ****************/
//extracts words from a page from pageDirectory and inserts them into index
bool extract_words(hashtable_t *index, char *dir, int id)
{
    counters_t *word_count;                                                       //for a given word: (doc ID, wordcount) for all documents in pageDirectory
    char buffer[200];                                                             //This will be our string buffer used to append the id to the filepath
    snprintf(buffer, sizeof(int) + sizeof(char) * strlen(dir), "%s/%d", dir, id); //our buffer will now be our new page's file path
    FILE *fp = fopen(buffer, "r");                                                //creates an empty file for writing
    if (fp == NULL)
    {
        return false; //nothing to look for in this file, its unreadable or does not exist
    }
    const char *c = "\n"; // Check if this works!
    char html[4000];      // shouldnt be more than 4000 chars in file
    char toAppend[200];   //each shouldnt be more than 200 chars
    while (fgets(toAppend, 200, fp) != NULL)
    {
        strncat(html, toAppend, strlen(toAppend)); //append toAppend to html string
        strncat(html, c, 1);                       //newline character between each line
    }
    printf("HTML:\n%s", html); //CHECKPOINT IF HTML WAS TRANSLATED CORRECTLY!

    int pos = 0;
    char *result;                                           //this will hold the words found in the page's file
    while ((result = html_getNextWord(html, &pos)) != NULL) //while we are not at EOF
    {
        if (result[0] == '\0' || !is_alpha_str(result)) //check if result is an alphabetical word and is not empty or less than 3 chars
        {
            count_free(result);
            continue; //skip to next word
        }
        result = to_lowercase(result); //normalize our word, still need to free after
        //now we need to add that word to our index, and then a counter for it
        word_count = hashtable_find(index, result);
        if (word_count == NULL)          //word is not in index yet!
        {                                //create a counterset for our new word to track its word count!
            word_count = counters_new(); //create our counterset for our word
            if (word_count == NULL)
            {
                fprintf(stderr, "counters_new failed\n");
            }
            counters_add(word_count, id); //page id for tracking
            hashtable_insert(index, result, word_count);
            count_free(result);
        }
        counters_add(word_count, id); //page id for tracking
        count_free(result);
        //check to see if I need to use hashtable iterate to change hashtable's items
        //hashtable_iterate(index, result, updatecounter)
    }
    fclose(fp);
    return true; //words from file were successfully extracted!
}

/**************** index_build ****************/
//function shall build our inverted index and store it into
//a hashtable. fp_dir is the pageDirectory which we must parse through
//in order to build our inverted index.
void index_build(hashtable_t *index, char *dir)
{
    //first we need to find the words of our pages
    //We need to read each file within the directory, to do so,
    //we will build strings in order to access those files
    //ASSUME ID starts at 1 and increments!
    int id = 1;
    while (extract_words(index, dir, id))
    {
        id++;
    }
    //finished building index
}

