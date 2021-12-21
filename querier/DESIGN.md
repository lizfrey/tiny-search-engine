# Querier Design Spec 
## Elizabeth Frey, CS 50, Winter 2021

### About
Recall the Querier Requirements Spec; the TSE **querier** is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Crawler, and answers search queries submitted via stdin.

### User Interface
The querier's only interface with the user is on the command-line; it must always have two arguments.
```bash
$ ./querier crawlerDirectory indexFilename
```
- `crawlerDirectory` is the pathname of a directory produced by the Crawler
- `indexFilename` is the pathname of a file produced by the Indexer

For example, to query the pageDirectory `pageDir` using the index in `indexFile`

```bash
$ ./querier pageDir indexFile
```

### Inputs and Outputs
*Input:* The inputs are command-line parameters and queries that come once the querier is running through stdin. The files that **querier** will use come from the parameter `crawlerDirectory` and `indexFilename`. It will continue to take input until it reaches an EOF. 

*Output:* The **querier** will output the results of each query to the user through the command-line. It has no other outputs and will not modify any of the files in `crawlerDirectory` or `indexFilename`.
It will output "Query?" to prompt input from the user. Upon recieving the input, it will `normalize` each part of the query and send it back to the user, with the format "Query: [query]".
Next, it will output the number (if any) of matches, and print each match with the following format on a new line:
"score [score] doc [doc #]: [url]". 

### Functional Decomposition into Modules
We anticipate the following modules or functions:
1. *main*, which will parse arguments and pass them to *querier*
2. *querier*, which will  call *prompt* to prompt the user, validate each query, and loop though all the words in the query. 
3. *processquery* which will be in charge determining the precedence of evaluating the query and calling *intersects* or *unions* on the resulting counters holding the scores and ids for each document for each word. 
4. *intersects* will take in two counters for queries, free them both, and create a new one which contains the new score and pages on which the queries intersect
5. *unions* will take in two counters for queries, free them both, and create a new one which contains all the page son which the queries appear
6. *buildOutput* which will take in the counters of `results` and print out the results of the query to the user
7. *validateQuery* will take in the query array and will take in the query, and print nothing if it is valid, returning 0. if it is invalid, it will print an error message and return 1.
8.  *prompt*, from the Lab 6 assignment page, which will prompt the user

And some helper functions and modules that provide data structures:
1. *counters*, which hold word occurances in different files
2. *hashtable*, which holds words and the *counters* for each word
3. *intersectsHelper1*, which is used when calling `counters_iterate` in *intersects*
4. *intersectsHelper2*, which is used when calling `counters_iterate` in *intersects*
5. *unionHelper*, which is used when calling `counters_iterate` in *union*
6. *countersCopyHelper*, which is used when calling `counters_iterate` in *counterCopy*
7. *docCounterHelper*, which is used when calling `counters_iterate` in *buildOutput*
8. *sortDocsHelper*, which is used when calling `counters_iterate` in *buildOutput*


### Pseudo Code for Logic/Algorithmic Flow
1. execute from the command line
2. *main* will validate the arguments and pass them to *querier*
3. *querier* will prompt the user for the first query
    * it will save an array that holds each word in the query. if it finds any error in the query, whether when saving the array or after calling *validateQuery*, it will tell the user and prompt the user for another query
    * it will call *processQuery*, which calling *intersects* and *unions* to process the next two queries (from left to right but accounting for precedence). It will delete the original counters, and combine them into a new counter. until one final counters object remains
        * *intersect* and *unions* will take care of any deletions necessary using `counters_delete`
4. *querier* will call *buildsOutput* to print the final output to the user based on the final `counter`
5. it will prompt the user for the next query and repeats step 3 and 4.
6. on EOF from stdin, it will free all allocated memory, and end

### Dataflow through Modules
1. *main* will process and validate arguments, passing them to *querier*
2. *querier* will prompt the user for queries. It will process the queries, and save them to an array. It will pass the queries to *validateQuery* to validate them
3. *processQuery* will repeatively get the `counters` object for each word from the `index`, make a copy of them, and use *unions* and *intersects* to form a final `counters` object
3. the final result of the query will be made by *querier* and output to the user
4. step 2 will repeat until EOF is received from stdin

### Major Data Structures
1. *hashtable*, used to store `char *words` as *keys* and `counters` as *values*
2. *counters*, used to keep track of *counts* of `words` in a specific document with document `id` as the *key*. Also used to keep track of total query `score` for each document (representing a webpage) with an `id`

### Testing Plan
To test, use `fuzzquery` provided to generate random queries to send to **querier**.
Example usage:

```bash
$ ./fuzzquery indexFile  5 1 > testfile
$ ./querier crawlerDirecotry indexfile < testfile
```
