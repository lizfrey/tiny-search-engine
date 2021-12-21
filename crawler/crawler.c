/*
 * crawler.c - a standalone program that crawls the web and 
 * retrives webpages given a "seed" URL. It parses the web, 
 * extracts any embedded URLs, and then retrives each 
 * of those pages, recursively, but limiting its exploration to a 
 * given "depth"
 *
 * usage: ./crawler seedURL pageDirectory maxDepth
 *  - where seedURL is the initial URL, and is internal
 *  - where pageDirectory is the directory in which to write 
 *  downloaded pages and already exists
 *  - where maxDepth represents the maximum crawl depth, and is
 *  an integer between 0 and 10, inclusive
 *
 * Elizabeth Frey
 * CS50 Lab 4
 * Winter 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../libcs50/memory.h"
#include "../libcs50/jhash.h"
#include "../libcs50/file.h"
#include "../libcs50/set.h"
#include "../common/pagedir.h"

/**** file-local global variable ****/
/* none */

/**** local types ****/
/* none */

/**** global types ****/
/* none */

/**** function declarations ****/

/********** crawler **********/
/* Crawls through webpages starting from a 
 * seedURL to a specific maxDepth. Creates pages in
 * the pageDir given for each internal webpage.
 * 
 * We return:
 *    Nothing, but pageDir has been populated with webpage files.
 * Caller is responsible for ensuring that the pageDir exists
 * and is writable.
 */
static void crawler(char *seedURL, char *pageDir, int maxDepth);

/********** pagefetcher **********/
/* Fetches the HTML of a webpage and stores it within the webpage.
 * 
 * We return:
 *      true on success.
 *      false on failure
 * 
 * This uses the webpage_fetch function defined in webpage.h
 */
static bool pagefetcher(webpage_t *page);

/********** pagesaver **********/
/* Saves a given webpage in a filename "pageDir/id"
 * 
 * We return:
 *      nothing
 * 
 * Each file has the URL, the depth, and then the HTML.
 * Each file is within the pageDir.
 * 
 * Caller is responsible for ensuring the pageDir exists and non-null *page.
 */
static void pagesaver(char *pageDir, webpage_t *page, int id);

/********** pagescanner **********/
/* Scans the pages for URLs, returning them one at a time.
 * 
 * We return:
 *      A pointer to the URL.
 * 
 * Caller is responsible for freeing the pointer to the URL.
 * 
 * Uses the webpage_getNextURL function defined in webpage.h
 */
static char *pagescanner(webpage_t *page, int *pos);

/**** local functions ****/

/********** main **********/
/* checks and validates parameters, and passes them to the crawler
 */
int
main(int argc, char* argv[]){

    //check parameters
    
    //check to see if number of arguments is correct
    //if not, print to stderr
    if(argc != 4){
        fprintf(stderr, "Wrong number of parameters given.\n");
        return 1;
    }
    
    //allocate memory for and initialize given parameters
    char *seedURL = argv[1];
    char *pageDir = argv[2];
    int depth;
    sscanf(argv[3], "%d", &depth);

    //check to make sure url is internal and normalizable
    if(IsInternalURL(seedURL) == false){
        fprintf(stderr, "%s is not internal.\n", seedURL);
        return 1;
    }
    if(NormalizeURL(seedURL) == false){
        fprintf(stderr, "%s cannot be normalized\n.", seedURL);
        return 1;
    }

    //check to make sure page directory path given exists
    if(isDirectory(pageDir) == false){
        return 1;
    }
    
    //check to make sure int is in the range
    if(depth < 0 || depth > 10){
        fprintf(stderr, "%d is not in the correct range.\n", depth);
        return 1;
    }

    char *copyOfURL = count_malloc((strlen(seedURL)+1)*sizeof(char));
    strcpy(copyOfURL, seedURL);
    crawler(copyOfURL, pageDir, depth);

    //pointers to null
    seedURL = NULL;
    pageDir = NULL;

    return 0;

}

/********** crawler **********/
/* See function declaration for description */

static void 
crawler(char *seedURL, char *pageDir, int maxDepth){
    //initialize data structures
    bag_t *unvisited = bag_new();
    hashtable_t *visited = hashtable_new(10); //check on size
    int depth = 0;
    int id = 1;
    char *nullpointer = NULL;

    //make seed webpage and insert it in
    NormalizeURL(seedURL);
    webpage_t *curr;
    hashtable_insert(visited, seedURL, " ");
    bag_insert(unvisited, webpage_new(seedURL, depth, nullpointer));
    //loop through the bag until it is empty 
    while((curr = (webpage_t *)bag_extract(unvisited))!= NULL){
        //check to see if html has already been gotten
        if(pagefetcher(curr)){
            printf("Fetched: %s\n", webpage_getURL(curr));
            //save the page with a unique id
            pagesaver(pageDir, curr, id);
            //update the id
            id = id + 1;
            //check to see if the current depth is less than the nax
            if(webpage_getDepth(curr) < maxDepth){
                depth = webpage_getDepth(curr);
                int pos = 0;
                char *url;
                printf("%d Scanning: %s\n",depth,webpage_getURL(curr));
                while((url = pagescanner(curr, &pos))){
                    //check url
                    printf("  Found: %s\n", url);
                    //checks for internal and normalizes
                    if(!IsInternalURL(url)){
                        printf("   IgnExtrn: %s\n", url);
                        count_free(url);
                    }
                    //check if url has already been seen
                    else if(!hashtable_insert(visited, url, " ")){
                        printf("   IgnDup: %s\n", url); 
                        //because hashtable won't be able to free it later
                        count_free(url);
                    }
                    else{
                        printf("   Added: %s\n", url);
                        bag_insert(unvisited, 
                                webpage_new(url, depth+1, nullpointer));
                    }
                }
            }
        }
        else{
            printf("could not fetch %s\n", webpage_getURL(curr));
        }
        webpage_delete(curr);
    }
    //delete bag and hashtable
    bag_delete(unvisited, webpage_delete);
    hashtable_delete(visited, NULL);
}

/********** pagefetcher **********/
/* See function declaration for description */
static bool 
pagefetcher(webpage_t *page){
    return webpage_fetch(page);
}

/********** pagesaver **********/
/* See function declaration for description */

static void 
pagesaver(char *pageDir, webpage_t *page, int id){
    saveWebpageIntoFile(pageDir, webpage_getURL(page),webpage_getDepth(page),
    webpage_getHTML(page),id);
}

/********** pagescanner **********/
/* See function declaration for description */
static char 
*pagescanner(webpage_t *page, int *pos){
    return webpage_getNextURL(page, pos);
}
