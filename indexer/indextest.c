/*
 * indexer.c - contains an indexer that goes 
 * through all the pages in a directory and indexes each
 * word to an inverted index. finally, it saves
 * this index to a file
 * 
 * usage: ./indextest oldIndexFilename newIndexFilename
 *    where `oldIndexFilename` is the name of a file produced by the indexer 
 *    where `newIndexFilename` is the name of a file into which the index should be written 
 * 
 * the indextest loads the index from oldIndexFilename
 * and creates the new file (if needed) and 
 * overwrites the file (if it already exists).
 * 
 * Elizabeth Frey
 * CS50 Lab 5
 * Winter 2021
 */ 

#include <stdlib.h>
#include <stdio.h>
#include "../common/index.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"

/********** FUNCTIONS **********/
int 
main(int argc, char* argv[]){
    //parse parameters

    if(argc != 3){
        fprintf(stderr, "Wrong number of parameters given.\n");
        return 1;
    }
    
    //initialize given parameters
    char *oldIndexFilename = argv[1];
    char *newIndexFilename = argv[2];

    //open the old and new file, print error on failure
    FILE *old;
    FILE *new;

    if((old = fopen(oldIndexFilename,"r")) == NULL){
        fprintf(stderr,"Unable to open %s for reading\n", oldIndexFilename);
        return 1;
    }

    if((new = fopen(newIndexFilename,"w")) == NULL){
        fprintf(stderr, "Unable to open %s for writing\n", newIndexFilename);
        return 1;
    }

    //save index from oldIndexFilename
    hashtable_t *index = getIndexFromFile(old);
    //save index to newIndexFilename
    indexSaver(index, new);
    indexDeleter(index);

    //close files
    fclose(old);
    fclose(new);

    return 0;
}
