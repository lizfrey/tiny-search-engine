# Indexer Implementation Spec 
## Elizabeth Frey, CS50, Winter 2021

The indexer.c file contains an indexer that goes through all the files in a `pageDir`and indexes each word into a `hashtable` that holds `counters` for each word based on the page id. It will create an inverted-index in the `hashtable` and save it to a `indexFilename`

### Functions
`indexer` will implement the following functions:
1. `main`
2. `indexer`
3. `indexBuilder`

`main`
`main` will first validate the arguments. It will check the number of arguments, check that `pageDir` exists, is readable, and contains a .crawler file (made when running `crawler`). This will ensure that the `pageDir` contains the output of `crawler`. Next, it makes the file `indexFilename`. It will exit on any error. Finally, it will call and pass arguments to`indexer`. At the end, it will free the file opened. 

`indexer`
`indexer` will first instantiate a `hashtable` to hold `char *keys` containing each word and `counters *values` containing the page id and count for each key. 
It will continually call `getFile(pageDir, id)` until it returns null to loop through the entire directory. For each file in `pageDir`, it parses the file to make a new `webpage`. Next, it will use `webpage_getNextWord` to get each word from the file. It will call `NormalizeWord` to normalize the word. 
Next, it will call `indexBuilder` to add the word into the index. Once the `hashtable` is made, it will call `indexSaver` to save the inverted index into `newIndexFilename`. Finally, it will free the index by calling `indexDelete` (see index.h section for more information).

`indexBuilder`
Index builder will check to see if a counter already exists for the word at that page id in the `counterset` for that word in the `hashtable`. If it does not exist, it will create a new `counters` object with the `id` as the key and insert it. If it already exists, then it will increment it using `counters_add`.

### Additional Files and Data Structures Used
1. *hashtable* used to store the inverted index. Creation, insertion, find, and deletion methods used.

2. *counters* used to store a map from `page id` to instances of a word. Creation, addition, and deletion methods used. 

3. *file* functions used: `lines_in_file`, `freadlinep`, `freadwordp`, `freadfilep`
- `lines_in_file` is used to find the number of words in a 
- `pageDir/id`
- `freadlinep` is used to read in the url of a `webpage`
- `freadwordp` is used to read in words from an index file
- `freadfilep` is used to get the rest of the HTML from a file

4. *word* functions used: `NormalizeWord`
- `NormalizeWord` takes in a `char *word` and returns the same pointer, with each letter converted to lowercase. It uses `tolower` from `ctype.h`.

5. *pagedir* functions used:`getWebpageFromFile`, and `isCrawlerDirectory`
- `isCrawlerDirectory` tries to open the file `pageDir/.crawler` to check to see if `pageDir` exists and is a crawler directory. 
- `getWebpageFromFile` takes in a `pageDir` and an `id`, and then opens a filename for reading, and scans in the `url`, `depth` and `html` to make a new `webpage`.

6. *index*, functions used: `indexSaver`, `getIndexFromFile`, `indexDeleter`
`indexSaver` will take in the inverted-index as a `hashtable` and the `newIndexFilename` as file from `indexer` and try to save the index to that file. 
It writes the inverted index to the file in the following format: 
* one line per word, one word per line
 * each line provides the word and one or more (docID, count) pairs, in the format
	 * *word docID count [docID count]...*
 * where *word* is a string of lower-case letters,
 * where *docID* is a positive non-zero integer,
 * where *count* is a positive non-zero integer,
 * where the word and integers are separated by spaces.

It does this by using helper `saveHashtable` function, which is passed to `hashtable_iterate`. This function will call `counter_iterate` with `saveCounter` to print each `id` `count` pair for each word (*key* in the hashtable) to the file. 

- `getIndexFromFile` takes in a `FILE *filename` as an input, uses `lines_in_file` to save the number of lines (equivalent to number of words), and loops through all the lines in the file, saving each line and `id key` pairs on a line. 

- `indexDeleter` takes in an index, and calls `hashtable_delete(index, indexDeleteHelper)`. `indexDeleteHelper` calls `counters_delete` to delete each `counter` in the `index`.

### Error Handling and Recovery

Upon any error, the `indexer` should print the error to `stderr` and exit with a non-zero exit code. 

### Security and Privacy Properties

There are no significant steps taken to ensure security and privacy for the user. 
