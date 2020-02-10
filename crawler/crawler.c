/* 
 * crawler.c - program for CS50 crawler module
 *
 * usage: ./crawler seedURL pageDirectory maxDepth
 * 
 * In validateArgs and main: code from activity fetchweb-solution.c used for input validations
 * Also borrowed function save_page from fetchweb-solution.c
 * Also borrowed example code for webpage_getNextURL
 * 
 * CS50, February 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //for checking if a directory is writable and exists
#include <stdbool.h>
#include "../libcs50/set.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/file.h"
#include "../libcs50/webpage.h"
#include "../libcs50/memory.h"

#define MAX_DEPTH 10

/**************** local function prototypes ****************/
void save_page(webpage_t *page, const char *fname, int id);
int validateArgs(const int argc, char *argv[]);
//void itemdelete(void *item);

/**************** main ****************/
int main(const int argc, char *argv[])
{
    bag_t *pages_to_explore;
    hashtable_t *urls_seen;
    int id = 1; //ID for files created within pageDirectory

    if (validateArgs(argc, argv) != 0) //perform arguement validations
    {
        return 1; //an arguement validation has failed
    }

    pages_to_explore = bag_new(); //create a bag that will hold our pages to explore
    if (pages_to_explore == NULL)
    {
        fprintf(stderr, "bag_new failed\n");
        return 2;
    }

    urls_seen = hashtable_new(10); //create a hashtable that will hold 10 sets
    if (urls_seen == NULL)
    {
        fprintf(stderr, "hashtable_new failed\n");
        return 3;
    }

    char *url = argv[1];        // seedURL
    char *dir = argv[2];        // pageDirectory
    char *depth = argv[3];      // maxDepth desired by user from program's arguements
    int maxDepth = atoi(depth); // convert string into an integer

    // create webpage for seedURL
    webpage_t *page = webpage_new(url, 0, NULL);
    if (page == NULL)
    {
        fprintf(stderr, "webpage_new failed\n");
        return 4;
    }

    bag_insert(pages_to_explore, page); //insert page into bag of webpages to crawl
    if (!hashtable_insert(urls_seen, webpage_getURL(page), ""))
    { //insert URL to hashtable of URLs seen
        fprintf(stderr, "hashtable_insert failed on seedUrl \n");
        return 5; //This should not occur on first insert
    }
    while (true)
    {                                         //while there are more webpages to crawl:
        page = bag_extract(pages_to_explore); //extract a webpage from the bag to be crawled
        sleep(1);                             //pause a second between fetches
        if (page == NULL)
        { //break out of the loop
            break;
        }
        if (webpage_fetch(page)) //if html is successfully fetched from webpage
        {
            save_page(page, dir, id);
            id++; //id is incremented on each call
        }
        else
        { // failed to fetch the page
            fprintf(stderr, "failed to fetch %s\n", webpage_getURL(page));
            webpage_delete(page);
            continue; //skip to the next page
        }
        if (webpage_getDepth(page) < maxDepth)
        {
            int pos = 0;
            char *result;
            while ((result = webpage_getNextURL(page, &pos)) != NULL)
            {
                // normalize the URL
                if (!NormalizeURL(result))
                {
                    fprintf(stderr, "%s cannot be normalized\n", result);
                    count_free(result);
                    continue; //skip to the next URL
                }
                // check URL to see if it is internal to the cs servers
                if (!IsInternalURL(result))
                {
                    fprintf(stderr, "%s is not an internal url\n", result);
                    count_free(result);
                    continue; //skip to the next URL
                }
                printf("Url:%s   -------> Discovered!", result);
                if (!hashtable_insert(urls_seen, result, "")) //insert URL to hashtable of URLs seen
                {
                    count_free(result);
                    continue; //We have already encountered this URL in our hashtable
                }
                else //new URL discovered! Make a new page!
                {
                    webpage_t *toAdd = webpage_new(result, webpage_getDepth(page) + 1, NULL);
                    if (toAdd == NULL)
                    {
                        fprintf(stderr, "webpage_new failed\n");
                        count_free(result);
                        continue;
                    }
                    bag_insert(pages_to_explore, toAdd); //insert toAdd into bag of webpages to crawl
                    count_free(result);                  //free the URL we just inspected
                }
            }
        }
        printf("Url:%s ---> Explored!\n", webpage_getURL(page));
        webpage_delete(page); //we will delete our current page, we are done inspecting it
    }
    bag_delete(pages_to_explore, NULL); //delete all webpages in the bag
    //bag_delete(pages_to_delete, webpage_delete);  //delete all webpages in the bag
    hashtable_delete(urls_seen, NULL); //delete the hashtable and the empty strings stored inside

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

/**************** save_page ****************/
void save_page(webpage_t *page, const char *fname, int id)
{
    char buffer[50];                               //This will be our string buffer used to append the id to the filepath
    snprintf(buffer, 51, "%s/%d", fname, id);      //our buffer will now be our new page's file path
    FILE *fp = fopen(buffer, "w");                 //creates an empty file for writing
    assertp(fp, "cannot open file for writing\n"); //check if file can be created and wrote to

    fprintf(fp, "%s\n%d\n%s", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));

    fclose(fp);
}

/**************** validateArgs ****************/
//function will validate the arguements for crawler.c
//returns non-zero integer if a validation fails
int validateArgs(const int argc, char *argv[])
{
    char *program = argv[0]; // program name
    // check input arguments: # of arguments
    if (argc != 4)
    {
        fprintf(stderr, "usage: %s seedURL pageDirectory maxDepth\n", program); //Crawler MUST always have 3 arguements(excluding the program name)
        return 1;
    }
    char *url = argv[1];          // seedURL
    char *dir = argv[2];          // pageDirectory
    char *depth = argv[3];        // maxDepth desired by user from program's arguements
    int trueMaxDepth = MAX_DEPTH; // TrueMaxDepth defined by program for safety

    // check input arguments: URL validity
    if (!NormalizeURL(url))
    {
        fprintf(stderr, "%s cannot be normalized\n", url);
        return 3;
    }
    // check input arguments: Check if seedUrl is an internal URL
    if (!IsInternalURL(url))
    {
        fprintf(stderr, "%s is not an internal url\n", url);
        return 2;
    }
    // check input arguments: pageDirectory is a directory that exists and is writable
    if ((access(dir, W_OK) != 0) || (access(dir, F_OK) != 0)) // if pageDirectory does not exist or is not writable
    {
        fprintf(stderr, "%s must be a directory that exists and is writable\n", dir);
        return 4;
    }
    // check input arguments: maxDepth validity
    int maxDepth = atoi(depth); // convert string into an integer
    //Note: if the string contains a character other than a '-' or ' ' before the first integer, then
    //the function atoi will return a default value of 0. Therefore maxDepth will default to 0 in such cases.
    if (maxDepth > trueMaxDepth)
    {
        fprintf(stderr, "Depth %d cannot exceed the program's defined max depth: %d \n", maxDepth, trueMaxDepth);
        return 5;
    }
    if (maxDepth < 0)
    {
        fprintf(stderr, "Depth %d must be a non-negative integer \n", maxDepth);
        return 6;
    }
    return 0; //return 0 if all validation tests are successful
}
// void itemdelete(void *item)
// {
//     count_free(item);
// }
