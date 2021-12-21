/*
 * indexer.c - contains an indexer that goes 
 * through all the pages in a directory and indexes each
 * word to an inverted index. finally, it saves
 * this index to a file
 * 
 * usage: ./indexer pageDirectory indexFilename
 *    where `pageDirectory` is the pathname of a crawler directory 
 *    where `indexFilename` is the pathname of a file into 
 *    which the index should be written
 * 
 * the indexer creates the file (if needed) and 
 * overwrites the file (if it already exists).
 * 
 * Elizabeth Frey
 * CS50 Lab 5
 * Winter 2021
 */ 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "hashtable.h"
#include "webpage.h"
#include "memory.h"
#include "pagedir.h"
#include "counters.h"
#include "word.h"
#include "index.h"
#include "file.h"

/********** FUNCTION DECLARATION **********/
/********** indexer **********/
/* 
 * indexer read documents and build an
 * inverted-index data structure mapping
 * 
 * caller provides: a valid, opened indexFile
 * and a valid pageDir that is a crawler directory
 * 
 * we return nothing. 
 */
static void indexer(FILE *indexFile, char *pageDir);

/********** indexBuilder **********/
/* 
 * indexBuilder builds an index based 
 * on a word and id. it increments the count
 * for the word on that page if it already has been
 * seen.
 * 
 * caller provides: a valid hashtable index, 
 * a valid, normalized word, and an id
 * 
 * we return:
 *    true: on successful addition of the word 
 * and id pair to the hashtable
 *    false: otherwise
 */
static bool indexBuilder(hashtable_t *index,char *word,int id);

/********** FUNCTIONS **********/
/********** main ***********/
/* main method that validates arguments,
 * checking to make sure the right number
 * of arguments are given, that the pageDir
 * is a valid, crawlerDirectory, and that
 * the newIndexFilename file can be made. 
 * 
 * we return:
 *    0 on success
 *    non-zero otherwise
 */
int
main(int argc, char* argv[]){
    //check parameters
    
    //check to see if number of arguments is correct
    //if not, print to stderr
    if(argc != 3){
        fprintf(stderr, "Wrong number of parameters given.\n");
        return 1;
    }

    //initialize given parameters
    char *pageDir = argv[1];
    char *indexFilename = argv[2];

    //check if directory is real and contains .crawler file
    if(isCrawlerDirectory(pageDir)==false){
        return 1;
    }

    //make new file to hold the index
    FILE *indexFile;
    if((indexFile = fopen(indexFilename, "w")) == NULL){
        fprintf(stderr, "Error making %s.\n", indexFilename);
        return 1;
    }

    indexer(indexFile, pageDir);

    //close the file and set pointers to NULL
    fclose(indexFile);
    pageDir = NULL;
    indexFilename = NULL;
    return 0;
}

/********** indexer ***********/
/* see function declaration for more information*/
static void
indexer(FILE *indexFile, char *pageDir){
    //initialize data structures
    hashtable_t *index = hashtable_new(1000);
    webpage_t *curr;
    //start off with first page
    int id = 1;

    //loop through pagedir until there are no more files
    while((curr = getWebpageFromFile(pageDir, id)) != NULL){
        //loop through all words in the file
        int pos = 0;
        char *word;
        while ((word = webpage_getNextWord(curr, &pos)) != NULL) {
            //if word is valid, normalize it and add it to the index
            if(strlen(word)>=3){
                NormalizeWord(word);
                if(!indexBuilder(index,word,id)){
                    //if index builder not successful
                    fprintf(stderr,"error adding %s in %d to hashtable", word, id);
                }
            }
            free(word);
        }
        //increment to next page and free the webpage
        id++;
        webpage_delete(curr);
    }

    //save the index to the new file and delete it
    indexSaver(index,indexFile);
    indexDeleter(index);
}

/********** indexBuilder ***********/
/* see function declaration for more information */
static bool
indexBuilder(hashtable_t *index,char *word,int id){
    //see if word is already in hashtable, if not, add in, 
    //if yes, increment (or add key, count)
    counters_t *set = (counters_t *)hashtable_find(index,word);
    if(set == NULL){
        //if counters was not found, make a new one, 
        // and add the id as a key (default count is 1)
        set = counters_new();
        if(set == NULL){
            //error making counterset
            return false;
        }
        counters_add(set, id);
        //insert it into the index, return false on failure
        if(!hashtable_insert(index, word, set)){
            //error inserting
            return false;
        }
        return true;
    }
    else{
        //if it already exists, increment the count
        counters_add(set,id);
        return true;
    }
    return false;
}
