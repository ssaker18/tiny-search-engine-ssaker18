#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //for checking if a directory is writable and exists
#include <stdbool.h>
#include <ctype.h>

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

//Normalize our word to lowercase
//caller is responsible for freeing return string
char *to_lowercase(char *result)
{
    int i = 0;
    char c; //lowercased char
    char *toReturn = malloc(sizeof(char) * strlen(result) + 1);
    for (i = 0; i < strlen(result); i++)
    {
        c = tolower(result[i]);
        toReturn[i] = c;
    }
    free(result);
    return toReturn;
}
