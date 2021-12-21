# Querier Implementation Spec 
## Elizabeth Frey, CS50, Winter 2021

### About
the TSE **querier** is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Crawler, and answers search queries submitted via stdin.

### Functions
1. `main`
2. `querier`
3. `processQuery`
4. `intersects`

    a. `intersectsHelper1`
    
    b. `intersectsHelper2`
5. `unions`

    a. `unionsHelper`
6. `countersCopy`

    a. `countersCopyHelper`
7. `buildOutput`

    a. `docCounterHelper`
    
    b. `sortDocsHelper`
8. `validateQuery`
9. `prompt`

`main`

`main` will first validate the arguments, checking the number of arguments, that `crawlerDirectory` exists and is a crawler directory (calling `isCrawlerDirectory` from `pagedir`). It will also check to see if `indexFilename` exists and can opened to read It then gets the `index` from the `indexFilename` and passes it to `querier`. It will exit on any error. At the end, it will close the file and delete the `index` by calling `indexDeleter`.

`querier`

`querier` will first call `prompt` to prompt the user. It will then take in the *query* as a `char *input`. First, it will go through the `input` and count the total number of words. If it finds any invalid characters, it will mark a `bool validquery` as false. If `validquery` is true, it will make a `char* queries[numWords]` based on the number of words it found. It will then call `validateQuery` to check again that the query is valid. If it is, it will save the result from `processQuery` into a `counters_t *final`, passing that to `buildOutput` with the `crawlerDirectory`, and then delete `final`. Afterwards, it will free the `input` and prompt the user again, repeating the process.

`processQuery`

`processQuery` will take in an array of queries `queries[]` and the `index`, `numQueries`, and return a single `counters` representing the final query result. When accessing the `counters` from index, it will always make a copy of each counter using `countersCopy` before finding the intersection or union. It will assume that the query is valid, and process the query in the following order:
    1. Make an empty `counters_t *final` to store the final `counters`
    2. Save the first query into a `counters_t *temp`
    3. If you hit an "or", call `unions` on `final` and `temp`, and then save `temp` as the next query in `queries[]`
    4. Otherwise, ignoring "and"s, call `intersects` on `temp` and the next query `temp2`.
    5. At the very end, find the union of `temp` and `final` and return `final`

`intersects` 

`intersects` will take in two counters for queries, and create a new one which contains the new score and pages on which the queries intersect. The new score for each document is the minimum of the count for each document. It will first create a new `counters_t *result` to store the intersection of the two counters. It will then copy over the first counter to the `result` counter using `countersCopy`. Then, it will call `counters_iterate` with `intersectsHelper1` on `result` and the second counter. This will set all keys in result that do not exist in the second counter to zero. Next, it will call `counters_iterate` with `intersectsHelper2`. This will set the score of all counters that exist in `result` and the second counter to the minimum of the two scores. Finally, it will delete the two counter inputs and return `result`.

`unions`

`unions` will take in two counters for queries, and create a new one which contains all the pages on which the queries appear. The total score for each document will be the sum of the scores for each document. It will first create a new `counters_t *result` to store the union of the two counters. Then, it will call `counters_iterate` with `unionsHelper` on `result` and the first counter, and on `result` and the second counter. `unionsHelper` will set (and subsequently add to `result`) the key values for the two input counters, with the count as the sum of the two. At the end, it will delete the two input counters and return `result`.

`counterCopy`

`counterCopy` will take in a `counters_t *dest` and a `counters_t *source` and will copy all of `source` to `dest`. It will do this by calling `counters_iterate` with `countersCopyHelper` on the two counters. `countersCopyHelper` simply uses `counters_set` to add the key,count pairs in `source` to the `dest`.

`buildOutput`

`buildOutput` which will take in the counters of `results` and print out the results of the query to the user in the following format
    1. the number of documents the query matched (if any)
    2. each document that the query matched on its own line, with the format `score [score] doc [id]: [url]`
It will do this by first counting the number of documents by calling `counters_iterate` with a pointer to an `int numDocs` and `docCounterHelper`. `docCountHelper` will increment `numDocs` as long as the count is not zero. Then, it will continuously find the next highest score and document `numDocs` times. It will do this by passing in an `int highest[]` to `counters_iterate` with `sortDocsHelper`. `int highest[]` stores the id in index 0 and count in index 1. `sortDocsHelper` will continuously replace the values in `highest[]` as it loops through the `counters`, so by the end, `highest[]` contains the id and count of the document with the highest score. It will then print this document with the format above, set the count in `final` for that document to 0, and repeat until all the scores have been printed in order from highest to lowest. 

`validateQuery` 

`validateQuery` will take in the query, and print nothing if it is valid, returning 0. If it is invalid, it will print an error message and return 1. If will be invalid if it contains multiple ands or ors in a row, or have an and or or at the beginning or end of the query, or if the query given does not have any values.

`prompt` 

`prompt` taken from the lab 6 assignment page will prompt the user for a new query.

### Additional Files and Data Structures Used
1. *hashtable* used to store the inverted index. Creation, insertion, find, and deletion methods used.

2. *counters* used to store a map from `id` to `count` of a word, and the combination of these maps. Creation, addition, iteration,and deletion methods used. It is also used to store a map from `id` to `scores` of a query.

3. *file* functions used:  `freadlinep`, `freadwordp`
- `lines_in_file` is used to find the number of words in a `indexFilename`
- `freadwordp` is used to read in words from an index file

4. *word* functions used: `NormalizeWord`
- `NormalizeWord` takes in a `char *word` and returns the same pointer, with each letter converted to lowercase. It uses `tolower` from `ctype.h`.

5. *pagedir* functions used:`getWebpageFromFile`, and `isCrawlerDirectory`
- `isCrawlerDirectory` tries to open the file `pageDir/.crawler` to check to see if `pageDir` exists and is a crawler directory. 

6. *index*, functions used: `getIndexFromFile`, `indexDeleter`

- `getIndexFromFile` takes in a `FILE *filename` as an input, uses `lines_in_file` to save the number of lines (equivalent to number of words), and loops through all the lines in the file, saving each line and `id key` pairs on a line. 

- `indexDeleter` takes in an index, and calls `hashtable_delete(index, indexDeleteHelper)`. `indexDeleteHelper` calls `counters_delete` to delete each `counter` in the `index`.

### Error Handling and Recovery
Upon any error involving improperly formatted queries, the `querier` will send the user a message, do not perform the query, and prompt for the next query.

Upon any other error, the `querier` should print the error to `stderr` and exit with a non-zero exit code. 

### Security and Privacy Properties

There are no significant steps taken to ensure security and privacy for the user. 
