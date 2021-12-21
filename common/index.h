/* 
 * index.h - a header file for CS 50 file index.c
 * 
 * This file contains the functions that are
 * used for saving and loading index files
 * 
 * Elizabeth Frey
 * CS 50 Lab 5
 * Winter 2021
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdlib.h>
#include "../libcs50/hashtable.h"

/********** GLOBAL FUNCTIONS **********/
/********** getIndexFromFile **********/
/*
 * saves and returns a hashtable using the content of an index file
 * caller is responsible for later freeing the hashtable
 * 
 * caller: 
 * - provides a valid filename to read from which is GUARANTEED to contain a
 * valid hashtable
 * 
 * we return: a valid hashtable, if one can be made from
 * the file
 */
hashtable_t *getIndexFromFile(FILE *filename);

/********** indexSaver **********/
/*
 * This functions takes in an index and a file, and
 * saves the index to the file, with 
 * one word on each line, and then numbers following each word
 * representing the id of the page the word was found on
 * and the number of times it was found
 * 
 * Caller: 
 * - is repsponsible for allocating memory for and later freeing
 *   both the index and the filename
 * 
 * We return:
 *    true: if save is successful
 *    false: if save is unsuccesful 
 */
bool indexSaver(hashtable_t *index, FILE *filename);

/********** indexDeleter ***********/
/*
 * indexDeleter takes in an index, which is a hashtable of counters
 * and deletes the index
 */
void indexDeleter(hashtable_t *index);

#endif //__INDEX_H
