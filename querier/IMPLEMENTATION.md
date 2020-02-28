#Implementation spec and testing plan for querier 
#By Sebastian Saker, cs50

Detailed pseudo code for each of the objects/components/functions:

Objects:
For counters, set, and hashtable modules please refer their respective
header files within libcs50 directory for their descriptions

In order to represent an inverted-index, I will be creating a hashtable whose keys are unique words and the items are countersets that track the count of that unique item throughout all of the documents, by mapping their docID to their wordcount.


Functions:

main

(translate main to pseudocode)


(other functions used, write them down in pseudocode)


Definition of detailed APIs, interfaces, function prototypes and their parameters:

APIs:

Again, for counters, set, and hashtable modules please refer their respective
header files within libcs50 directory for their descriptions

webpage: refer to header.h within libcs50

(Enter all other functions here and definte their purpose)


(Other APIs, function prototypes and their parameters—-describe them here)
 


Data structures:

(Insert all structures names and members here)
(Go through code and enter ALL structs used for querier program)


Security and privacy properties:

(Insert description of function visability/explain what the user/main program can access)
Since crawler will not be able to crawl past URLs that are not internal to the CS50 server,
the querier will be somewhat limited in regards to accessing non-internal links by whatever non-internal links are contained within the html of the CS50 server's internal webpages.

Error handling and recovery:

1. 

2.

(Insert a description of what each error code refers to)


Resource management:

(what functions allocate memory that the user/main is then responible for freeing)



Persistant storage:

pageDirectory—This is the pathname for an existing directory produced by the crawler

indexFilename—This is the pathname of the index file which will allow us to build an inverted index data structure by scanning through the index file


