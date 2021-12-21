/*
 * word.h - a header file for CS50 word.file
 * 
 * This file contais functions that are used to 
 * normalize a word, converting each letter to 
 * lowercase.
 * 
 * Elizabeth Frey
 * CS50 Lab 5
 * Winter 2021
 */

#ifndef __WORD_H
#define __WORD_H

#include <stdio.h>
#include <stdbool.h>

/********** FUNCTIONS **********/

/********** normalizeWord **********/
/* Takes in a word and returns the 
 * normalized version of it. 
 * 
 * Caller: inputs char *word, has to be valid word
 * 
 * We return:
 * - false if fail
 * - true if succeeds
 * 
 * The same pointer that is given is returned.
 */
char *NormalizeWord(char *word);

#endif //__WORD_H
