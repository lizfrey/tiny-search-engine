/* 
 * word.c - a file that contains functions used
 * to normalize words
 * 
 * Elizabeth Frey
 * CS 50 Lab 5
 * Winter 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "word.h"
#include <ctype.h>
#include "../libcs50/memory.h"

/********** functions **********/
/* see word.h for description */
char *
NormalizeWord(char *word){
    if(word == NULL)
        return NULL;

    //iterate through word and set each character to lowercase
    for(int i = 0; word[i]!= '\0'; i++){
        word[i] = tolower(word[i]);
    }
    return word;
}
