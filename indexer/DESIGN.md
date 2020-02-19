#Sebastian Saker's Design spec for CS50 TSE indexer module

User Interface:

The indexer’s only interface with the user is on the command-line; it must always have two arguments.

./indexer pageDirectory indexFilename

where pageDirectory is the pathname of a directory produced by the Crawler

where indexFilename is the pathname of a file into which the index should be written;
the indexer creates the file (if needed) and overwrites the file (if it already exists).


Inputs and Outputs: 

Input: the only inputs are command-line parameters; see the User Interface above.

Output:
The indexer writes the inverted index to a file, and both the index tester and the querier read the inverted index from a file; the file shall be in the following format.

- one line per word, one word per line
- each line provides the word and one or more (docID, count) pairs, in the format
	- word docID count [docID count]…

- where word is a string of lower-case letters,
- where docID is a positive non-zero integer,
- where count is a positive non-zero integer,
- where the word and integers are separated by spaces.
- Within the file, the lines may be in any order.
- Within a line, the docIDs may be in any order.


Functional decomposition into modules:

indexer:

1. Process and validate command-line parameters
2. Initialize data structure index
3. index\_build(directory, index)
4. index\_save(file, index);
5. clean up data structures


And some helper modules that provide data structures:

1. Hashtable, set, and counters shall we used in order to order to create an inverted index


Pseudo code for logic/algorithmic flow

The indexer will run as follows:

1. execute from a command line as shown in the User Interface
2. parse the command line, validate parameters, initialize other modules
3. read documents from the pageDirectory
4. build an inverted-index data structure mapping from words to (documentID, count) pairs,
	- count represents the number of occurrences of the given word in the given document
	- normalize(to lowercase) the word before indexing
	- ignore words with fewer than three characters
5. create a file indexFilename and write the index to that file


Dataflow through modules
1. main parses parameters and passes them to the indexer
2. indexer will build an inverted-index data structure by utilizing set, hashtable, and counters
3. index\_save will save the inverted index output to a designated file
4. lastly, in main we will clean up the data structures we used


Major data structures
Three helper modules provide data structures:

1. Hashtable to hold set
2. Set will point to counters for each item, each key is the word that counters is tracking for within pageDirectory specified
3. Counters will be used in order to track count of the amount of times that each word appears on each webpage

