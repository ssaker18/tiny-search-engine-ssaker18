/* 
 * pagedir.h - header file for CS50 pagedir module
 *
 * Sebastian Saker, cs50
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"

/**************** global types ****************/

/**************** functions ****************/

/**************** newPageDirectory(char *dir) ****************/
//initialize page directory, caller is responsible for closing the file returned afterwards!
//Return NULL if fails to create new pageDirectory
//Return pointer to fp if successful
FILE *newPageDirectory(char *dir);

/**************** isCrawlerDirectory(char *dir) ****************/
//Check if our directory was created by the crawler (meaning it should contain a .crawler file)
//Return false if directory is unaccessable or not created by crawler
//Return true if created by crawler and exists and is readable
bool isCrawlerDirectory(char *dir);

// /**************** save_page ****************/
// //save page's information to a directory that will contain crawler's found pages
// //Additionally, will ensure there is a .crawler file within the page directory
// void save_page(void *page, const char *fname, int id);

/**************** index_build ****************/
//Builds an inverted index out of a crawler output directory
//This will build an inverted index when given a filepath to a directory created by
void index_build(hashtable_t *index, char *dir);

/**************** extract_words ****************/
//Shall extract words from one of the files stored in the crawler made directory and store it in the index
//Return false if unsuccessful in order to stop index_build
bool extract_words(hashtable_t *index, char *dir, int id);

//PLEASE NOTE: THIS CODE IS 'BASICALLY' copied from webpage.c's webpage_getNextWord
//This will get the next word in an html-styled string
char *html_getNextWord(char *html, int *pos);

//check if the string is alphabetical (NO NUMBERS OR OTHER NON-ALPHABETICALS)
bool is_alpha_str(char *result);

#endif // __PAGEDIR_H
