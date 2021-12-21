/* pagedir.c - checks to see if a directory exists 
 * and is writable by trying to make a file in it. 
 * 
 * Elizabeth Frey
 * Winter 2021
 * CS 50 Lab 4
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pagedir.h"
#include "../libcs50/file.h"
#include "../libcs50/webpage.h"
#include "../libcs50/memory.h"

/********** global functions **********/
/* see pagedir.h for comments about exported functions */

/********** FUNCTIONS **********/
/********** isDirectory **********/
/* see pagedir.h for description */
bool
isDirectory(char *pageDir){
    //declare and allocate memory
    char filename[strlen(pageDir) + 1];
    strcpy(filename,pageDir);
    strcat(filename,"/.crawler");

    //if fp is null, dir doesn't exist or is not writable
    FILE *fp;
    if((fp = fopen(filename,"w"))== NULL){
        fprintf(stderr, "%s does not exists or is not writable.\n", pageDir);
        return false;
    }
    //else, dir exists and is writable, close it and return true
    else{
        fclose(fp);
        return true;
    }
}

bool
isCrawlerDirectory(char *pageDir){
    //declare and allocate memory
    char filename[strlen(pageDir) + 1];
    strcpy(filename,pageDir);
    strcat(filename,"/.crawler");

    //if fp is null, dir doesn't exist or is not writable
    FILE *fp;
    if((fp = fopen(filename,"r"))== NULL){
        fprintf(stderr, "%s does not exists, is not readable, or is not a crawler directory.\n", pageDir);
        return false;
    }
    //else, dir exists and is writable, close it and return true
    else{
        fclose(fp);
        return true;
    }
}

void
saveWebpageIntoFile(char *pageDir, char *url, int depth, char *html, int id){
    //make new filename
    //find number of digits in the id
    int num = id;
    int dig = 1;
    while(num>9){
        dig++;
        num/=10;
    }
    char *filename = count_malloc(sizeof(pageDir) + dig*sizeof(char));
    sprintf(filename,"%s/%d",pageDir,id);

    //make file
    FILE *fp = fopen(filename, "w");
    //failure opening
    if(fp == NULL){
        fprintf(stderr,"Error writing %s\n", filename);
        count_free(filename);
        return;
    }
    //write URL, depth, and html
    fprintf(fp, "%s\n%d\n%s", url, depth, html);
    fclose(fp);
    count_free(filename);
}

webpage_t
*getWebpageFromFile(char *pageDir, int id){
    //make new filename
    //find number of digits in the id
    int num = id;
    int dig = 1;
    while(num>9){
        dig++;
        num/=10;
    }
    char *filename = count_malloc(sizeof(char)*strlen(pageDir) + (dig+1)*sizeof(char) +1);
    sprintf(filename,"%s/%d",pageDir,id);

    //make file
    FILE *fp = fopen(filename, "r");
    //failure opening
    if(fp == NULL){
        count_free(filename);
        return NULL;
    }

    //read in the url, depth, and html from the file
    char *url = freadlinep(fp);
    int depth = fscanf(fp, "%d\n", &depth);
    char *html = freadfilep(fp);

    //save into a new page
    webpage_t *page = webpage_new(url, depth, html);
    //set pointers to NULL
    url = NULL;
    html = NULL;

    //free file and filename
    fclose(fp);
    count_free(filename);
    return page; 
}