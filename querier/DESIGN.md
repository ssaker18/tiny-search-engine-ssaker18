#Sebastian Saker's Design spec for CS50 TSE querier module

User Interface:

The querier’s interface with the user is on the command-line as well as user's standard
input from scanf(). It must always have two arguments. 

./querier pageDirectory indexFilename

where pageDirectory is the pathname of a directory produced by the crawler

where indexFilename is the pathname of a file produced by the indexer.

For user's standard input, the user will be prompted to enter a query which will be executed
and ranked accordingly for each line until EOF is reached. 

Syntax: 

[search_word1] 'and' [search_word2] denotes a search where both of the words must appear in the file in order to count. The count is determined by the pairs of those words in each document.

[search_word1] 'or' [search_word2] denotes a search where either of the words can appear in the file in order to count. The count is determined by the sum of those words in each document.


Inputs and Outputs: 

Input: the inputs are command-line parameters as well as scanning user input for seaches;
please see the User Interface above.

In order to quit the program: enter (control + D) which equals EOF



Output:
The querier will determine the ranking of the search terms provided, and display to the user
the ranking of the search terms entered for that line.
Here is an example of the standard output which shall be in the following format:


Query? computer and science
Query: computer and science 
Matches 6 documents (ranked):
score 170 doc   7: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Computer_science.html
score   5 doc   4: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Dartmouth_College.html
score   4 doc   2: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Linked_list.html
score   4 doc   3: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/Hash_table.html
score   1 doc   1: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/
score   1 doc   6: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/C_(programming_language).html


- one search per line
- spaces do not matter
- non-letter characters are not allowed
- queries will be automatically converted to lowercase
- 'and' cannot be first
- 'or' cannot be first
- digits cannot be used at all


Functional decomposition into modules:

querier:

1. Process and validate command-line parameters
2. Load the index from indexFilename into an hashtable of countersets
3. (enter other functions here)
4. (enter other functions here)
5. clean up data structures


And some helper modules that provide data structures:

1. Hashtable, set, and counters shall be used in order to order to create an inverted index.



Pseudo code for logic/algorithmic flow

The querier will run as follows:

1. execute from a command line with usage syntax
    ./querier pageDirectory indexFilename
    where pageDirectory is the pathname of a directory produced by the Crawler, and
    where indexFilename is the pathname of a file produced by the Indexer.
2. load the index from indexFilename into an internal data structure.

3. read search queries from stdin, one per line, until EOF.
    - clean and parse each query according to the syntax described below.
    - if the query syntax is somehow invalid, print an error message, do not perform the query, and prompt for the next query.
    - print the ‘clean’ query for user to see.
    - use the index to identify the set of documents that satisfy the query, as described below.
    - if the query is empty (no words), print nothing.
    - if no documents satisfy the query, print No documents match.
    otherwise, rank the resulting set of documents according to its score, as described below and print the set of documents in decreasing rank order; for each, list the score document ID and URL. (Obtain the URL by reading the first line of the relevant document file from the pageDirectory.)
    - Exit with zero status when EOF is reached on stdin.

Dataflow through modules
1. main parses parameters and builds an inverted-index data structure from indexFilename
2. querier will read search queries from stdin, one per line, until EOF
3. lastly, in main we will clean up the data structures we used


Major data structures
Three helper modules provide data structures:

1. Hashtable to hold set
2. Set will point to counters for each item, each key is the word that counters is tracking for within pageDirectory specified
3. Counters will be used in order to track count of the amount of times that each word appears on each webpage
