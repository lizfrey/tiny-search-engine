/* 
 * index.c 
 * 
 * This file contains the functions that are
 * used for saving and loading index files
 * 
 * Elizabeth Frey
 * CS 50 Lab 5
 * Winter 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include "index.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"

/********** GLOBAL FUNCTIONS DECLARATION **********/
/* see index.h for function declaration*/

/********** LOCAL FUNCTION DECLARATION **********/
/********** saveHashtable **********/
/*
 * saveHashtable is a helper function for indexSaver
 * it is passed into hashtable_iterate in
 * indexSaver, and it prints out the word on a new line
 * in the newIndexFilename
 */
static void saveHashtable(void *arg, const char *key, void *item);

/********** saveCounter **********/
/* 
 * saveCounter is a helper function for indexSaver. 
 * it is passed into counters_iterate in
 * saveHashtable, and it prints out id count
 * to the right of each word in the newIndexFilename
 */
static void saveCounter(void *arg, const int key, const int count);

/********** indexDeleteHelper **********/
/*
 * indexDeleteHelper is a helper function for 
 * indexDelete. It is passed into hashtable_delete
 * and is responsible for deleting each counter in the 
 * hashtable. 
 */
static void indexDeleteHelper(void *item);

/********** GLOBAL FUNCTIONS **********/
/********* getIndexFromFile *********/
/* see index.h for description */
hashtable_t 
*getIndexFromFile(FILE *filename){
    int numLines = lines_in_file(filename);
    hashtable_t *index = hashtable_new(numLines);

    //make pointers
    counters_t *counters;
    char *word;
    int id;
    int count;

    for(int i = 0; i<numLines; i++){
        word = freadwordp(filename);
        if(word == NULL){
            return NULL;
        }
        counters = counters_new();

        //read through that line
        int success = fscanf(filename, "%d %d", &id, &count);
        while(success == 2){
            counters_set(counters, id, count);
            success = fscanf(filename, "%d %d", &id, &count);
        }
        hashtable_insert(index,word,counters);
        free(word);
    }
    return index;
}

/********* indexSaver *********/
/* see index.h for description */
bool 
indexSaver(hashtable_t *index, FILE *filename){
    hashtable_iterate(index, filename, saveHashtable);
    return true;
}

/********* indexDeleter *********/
/* see index.h for description */
void indexDeleter(hashtable_t *index){
    hashtable_delete(index, indexDeleteHelper);
}


/********** LOCAL FUNCTIONS **********/
/********** saveHashtable **********/
/* see function declaration for description */
static void saveHashtable(void *arg, const char *key, void *item){
    //save as file and print word as first thing in line
    FILE *filename = arg;
    fprintf(filename, "%s ", key);
    //iterate through each id key and print to file
    counters_iterate(item, filename, saveCounter);
    //print a new line
    fprintf(filename, "\n");
}

/********** saveCounter **********/
/* see function declaration for description */
static void saveCounter(void *arg, const int key, const int count){
    FILE *filename = arg;
    fprintf(filename, "%d %d ", key, count);
}

/********** indexDeleteHelper **********/
/* see function declaration for description */
static void 
indexDeleteHelper(void *item){
    counters_delete(item);
}
