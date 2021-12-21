# Indexer Design Spec 
## Elizabeth Frey, CS 50, Winter 2021

Recall the Indexer Requirements Spec; the **indexer** reads document files produced by the TSE, builds an index, and writes that index to a file. Its companion, the *index tester*, loads an index file proced by **indexer** and saves it to another file. 

### User Interface
The indexer's only interface with the user is on the command-line; it must always have two arguments.
```bash
$ indexer pageDirectory indexFilename
```
- `pageDirectory` is the pathname of a directory produced by the Crawler
- `indexFilename` is the pathname of a file into which the index should be written; the indexer creates the file (if needed) and overwrites the file (if it already exists).

For example, to store the files created in the directory `pageDir` and create a file called `indexFile`, 

```bash
$ indexer pageDir indexFile
```

### Inputs and Outputs
*Input:* the inputs are command-line parameters. The files that **indexer** will use come from the parameter `pageDir`.

*Output:* the output is a filename, with the name `indexFilename` given from the command-line parameter. This file will contain an inverted-index data structure mapping from *words* to *(documentID, count) pairs*, wherein each *count* represents the number of occurrences of the given word in the given document. The `indexFilename` will have the format:
 * one line per word, one word per line
 * each line provides the word and one or more (docID, count) pairs, in the format
	 * *word docID count [docID count]...*
 * where *word* is a string of lower-case letters,
 * where *docID* is a positive non-zero integer,
 * where *count* is a positive non-zero integer,
 * where the word and integers are separated by spaces.

Within the file, the lines may be in any order.

Within a line, the docIDs may be in any order.

### Functional Decomposition into Modules

`indexer`
We anticipate the following modules or functions:
1. *main*, which will parse arguments and pass them to *indexer*
2. *indexer*, which will read documents and build an inverted-index data structure mapping
3. *indexBuilder*, which will build an index based on a word and id
4. *indexSaver*, which will save the inverted index to a file 

And some helper modules that provide data structures:
1. *counters*, which hold word occurances in different files
2. *hashtable*, which holds words and the *counters* for each word

### Pseudo Code for Logic/Algorithmic Flow
1. execute from the command line
2. *main* will validate the arguments and pass them to *indexer*
3. *indexer* will initialize the modules, and then will loop through all the documents in the `pageDir`
	* for each word in each page in the directory, if the word is more than three characters
		1. normalize the word
		2. iterate the counter (if it exists) or create the counter and insert into hashtable using *indexBuilder*
4. *indexSaver* will save the inverted index to a file

**normalize the word** means to convert it into a clean, canonical form.

### Dataflow through Modules
1. *main* will process and validate the arguments. it will then pass them to  *indexer*
2. *indexer* uses the arguments from *main* to loop through and read the files in `pageDir`. It will call on *indexBuilder*
3. the data from these files are used to create the hashtable in *indexBuilder*
4. the final inverted-index hashtable will be saved into a file using *indexSaver*

### Major Data Structures
1. *hashtable*, used to store `char *words` as *keys* and `counters` as *values*
2. *counters*, used to keep track of *counts* of `words` in a specific document with document `id` as the *key*

### Testing Plan
To test whether `indexer` works, it will be run on a variety of different `pageDir`s created by `crawler`. It will use the testcases provided in `/thayerfs/home/d29265d/shared/tse-output/crawler/crawler.data`.
