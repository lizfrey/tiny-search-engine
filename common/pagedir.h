/* 
 * pagedir.h - header file for CS50 pagedir.c file
 * 
 * This file contains functions that are used
 * to check whether a directory exists or not. It
 * does this by trying to create a file. If it returns NULL
 * then the directory does not exist or is not writable.
 * 
 * Elizabeth Frey
 * CS 50 Lab 4
 * Winter 2021
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"

/********** FUNCTIONS **********/

/********** isDirectory **********/
/* Takes in a direcotry name, and tries to make a file there.
 * 
 * We return:
 *    true: if the directory exists and is writable
 *    false: if the directory does not exists or is not writable
 * 
 * A new .crawler file is left in the diretory if it exists.
 */
bool isDirectory(char *pageDir);

/********** isCrawlerDirectory **********/
/* Takes in a direcotry name, and tries to read a .crawler file there
 * 
 * We return:
 *    true: if the directory exists and is readable
 *    false: if the directory does not exists or is not readable
 *           or if it does not contain a .crawler file
 */
bool isCrawlerDirectory(char *pageDir);

/******* saveWebpageIntoFile **********/
/* Takes in a webpage and a pageDir.
 * 
 * Saves the contents of the webpage into a 
 * file in the pageDir, with the format
 * URL
 * Depth
 * HTML
 */
void saveWebpageIntoFile(char *pageDir, char *url, int depth, char *HTML, int id);

/******* getWebpageFromFile **********/
/* Takes in a file and returns a webpage
 * on failure opening, returns null
 * caller is later responsible for calling webpage_delete
 */
webpage_t *getWebpageFromFile(char *pageDir, int id);
#endif //__PAGEDIR_H
