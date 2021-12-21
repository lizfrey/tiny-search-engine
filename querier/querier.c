/*
 * querier.c - contains an querier that goes 
 * prompts the user for a query, and uses the index
 * made by indexer to return a list of documents
 * with a score for that query
 * 
 * usage: ./querier crawlerDirectory indexFilename
 *    where `crawlerDirectory` is the pathname of a crawler directory 
 *    where `indexFilename` is the pathname of a file into which 
 *          an index is written
 * 
 * Elizabeth Frey
 * CS50 Lab 6
 * Winter 2021
 */ 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include "webpage.h"
#include "hashtable.h"
#include "memory.h"
#include "pagedir.h"
#include "counters.h"
#include "word.h"
#include "index.h"
#include "file.h"
#include "bag.h"

/********** FUNCTION DECLARATION **********/
/********** QUERIER **********/
/*
 * querier prompts the user for an input, 
 * processes that input into a query array, 
 * validates the query, and calls processQuery
 * to process the query. it then calls buildOutput
 * to print out the results of the query. it 
 * continues to prompt the user until it reaches 
 * EOF
 * 
 * caller provides: valid crawler directory and index
 * 
 * returns: nothing
 * 
 * on error: if query is invalid, it tells the 
 * user and prompts the user for a new query
 */
static void querier(char *dir, hashtable_t *index);

/********** PROCESSQUERY **********/
/*
 * process query takes in an array of queries, 
 * a valid index, the number of queries, and returns
 * a single counters_t representing the final
 * query result
 * 
 * caller provides: valid queries array and index
 * 
 * we return: a final counters_t showing the 
 * results of all the queries
 * 
 * it assumes that the queries array is valid. 
 * whenever it retrieves a counters_t from index, 
 * it uses countersCopy to make a copy. 
 */
static counters_t *processQuery(char* queries[], hashtable_t *index, int numQueries);

/********** COUNTERSCOPY **********/
/*
 * countersCopy copies all of the key,count
 * pairs from source to dest.
 * 
 * caller provides: valid dest and source counters
 * 
 * we return: nothing
 */
static void countersCopy(counters_t *dest, counters_t *source);

/********** COUNTERSCOPYHELPER **********/
/*
 * countersCopyHelper copies the current
 * key, count pair to void *dest, which
 * is a valid counters array
 * 
 * caller provides: valid counters in
 * void *dest argument
 * 
 * we return: nothing
 */
static void countersCopyHelper(void *dest, const int key, const int count);


/********** INTERSECTS **********/
/*
 * will take in two counters for queries, and 
 * create a new one which contains the new score
 * and pages on which the queries intersect. 
 * The new score for each document is the minimum 
 * of the count for each document.
 * 
 * caller provides: valid one and two counters
 * 
 * we return: new counters that has the intersection
 * of the counters given through the parameters
 * 
 * we guarantee: the provided counters are deleted 
 * using counters_delete
 * 
 * caller is responsible for later freeing the
 * counter that we return
 */
static counters_t *intersects(counters_t *one, counters_t *two);

/********** INTERSECTSHELPER1 **********/
/*
 * intersectsHelper1 sets all the keys that exist
 * in the current counters but not in the *arg
 * counters to a count of 0 in *arg.
 * 
 * caller provides: a valid counters object as *arg
 * 
 * we guarantee: if key does not exist in arg,
 * it is added, with a value of 0
 */
static void intersectsHelper1(void *arg, const int key, int count);

/********** INTERSECTSHELPER1 **********/
/*
 * intersectsHelper2 sets the value of keys
 * in arg to the minimum of the value of the current
 * key and the key in arg
 * 
 * caller provides: valid counters object as arg
 * 
 * we guarantee: counts in arg are the minimum of
 * the counts in the current counter and arg
 */
static void intersectsHelper2(void *arg, const int key, int count);

/********** UNIONS **********/
/*
 * unions ill take in two counters for queries, 
 * and create a new one which contains all the 
 * pages on which the queries appear. The total 
 * score for each document will be the sum of 
 * the scores for each document.
 * 
 * caller provides: valid one and two counters
 * 
 * we return: new counters that has the union
 * of the counters given through the parameters
 * 
 * we guarantee: the provided counters are deleted 
 * using counters_delete
 * 
 * caller is responsible for later freeing the
 * counter that we return
 */
static counters_t *unions(counters_t *one, counters_t *two);

/********** UNIONHELPER **********/
/*
 * unionsHelper adds all key,count pairs
 * to the sum of the count at key in arg and
 * the current count
 * 
 * caller provides: valid counters as *arg
 * 
 * we guarantee: arg contains the sum of the 
 * current counter's count and its previous counts for 
 * all keys in the current counter
 */
static void unionHelper(void *arg, const int key, int count);

/********** VALIDATEQUERY **********/
/*
 * validateQuery will take in query array
 * and validate it. Queries must contain
 * only words with alphanumeric characters, cannot
 * have "and" or "or" consecutively or at the 
 * beginning or end of the query.
 * 
 * we return and print:
 * - If the query is valid:
 *    we return true
 *    we print nothing
 * 
 * - If the query is invalid:
 *    we return false
 *    we print an error message
 * 
 * caller provides: query array and number of words
 */
static bool validateQuery(char* queries[], int numWords);


/********** BUILDOUTPUT **********/
/*
 * which will take in the counters of `results` 
 * and print out the results of the query to the 
 * user in the following format
 * 1. the number of documents the query matched (if any)
 * 2. each document that the query matched on its own line, 
 *    with the format:
 *        `score [score] doc [id]: [url]`
 * 
 * caller provides: valid crawlerDir as dir, and a valid final counters
 * 
 * we return: nothing
 */
static void buildOutput(char *dir, counters_t *final);

/********** DOCCOUNTERHELPER **********/
/*
 * docCounterHelper is used by buildOutput
 * to find the total number of documents with a score
 * greater than zero in final. It increments the arg
 * pointer, assumed to be an int *, by 1 if the count is 
 * greater than zero
 * 
 * caller provides: valid int * as the *arg
 * 
 * we return: nothing
 */
static void docCounterHelper(void *arg, const int key, int count);

/********** SORTDOCSHELPER **********/
/*
 * docCounterHelper is used by buildOutput
 * to find the highest score in the final counters.
 * It takes in a int [] as the *arg, with the 0th index
 * saving the id of the highest score and the 1st index
 * storing the highest score. it updates *arg whenever
 * it encounters a count that is higher than the current highest
 * score
 * 
 * caller provides: valid int [2] as the *arg
 * we return: nothing
 */
static void sortDocsHelper(void *arg, const int key, int count);

/********** PROMPT **********/
/* 
 * prompt, taken from the lab 6 assignment page
 * prompts only an interactive user (keyboard)
 * for a prompt
 * 
 */
int fileno(FILE *stream);
static void prompt(void);

/********** MAIN **********/
/*
 * main is responsible for validating the arguments
 * and passing the directory, and index
 * from the indexFilename to querier
 * 
 * validates arguments:
 * - ensures that directory is a crawler directory
 * - ensures that indexFilename is readable
 * 
 * on error: prints error message and exit
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
    if((indexFile = fopen(indexFilename, "r")) == NULL){
        fprintf(stderr, "Error opening %s.\n", indexFilename);
        return 1;
    }
    hashtable_t *index = getIndexFromFile(indexFile);
    if(index == NULL){
      fprintf(stderr, "Error getting index from %s.\n", indexFilename);
        return 1;
    }
    querier(pageDir, index);
    fclose(indexFile);
    indexDeleter(index);
}

/********** QUERIER **********/
/* see function declaration for more information */
static void 
querier(char *dir, hashtable_t *index){

  //prompt user and get input
  prompt();
  char *input;
  while((input = freadlinep(stdin))!=NULL){
    printf("Query: %s\n", input);

    //save input into a char * array
    //also check to see if there are any invalid characters
    int numWords = 0;
    bool validquery = true;
    for(char *c = input; *c!='\0'; c++){
      //check to see if has invalid characters
      if(!isalpha(*c) && !isspace(*c)){
        validquery=false;
      }
      else if(isalpha(*c)){
        numWords++;
        //read until you get to the end of the word
        while(isalpha(*(c+1))){
          c++;
        }
      }
    }

    if(validquery){
      int currentIndex = 0;
      char* queries[numWords];
      for(char *c = input; *c!='\0'; c++){
      //start at the start of a word
          if(isalpha(*c)){
              queries[currentIndex] = c;
              currentIndex++;
              while(isalpha(*(c+1))){
              c++;
            }
          
              //at end of word, put null
              *(c+1) = '\0';
              c++;
              NormalizeWord(queries[currentIndex-1]);
          }
      }

      if(validateQuery(queries, numWords)){
        counters_t *final = processQuery(queries, index, numWords);
        buildOutput(dir,final);
        counters_delete(final);
      }
    }
    else{
        printf("Invalid characters found in query.\n");
    }
    count_free(input);
    printf("---------------------------------------------------\n");
    prompt();
  }
}

/********** PROCESSQUERY **********/
/* see function declaration for more information */
static counters_t 
*processQuery(char* queries[], hashtable_t *index, int numQueries){
  //save the current index that we're at in the query
  int current = 0;
  counters_t *final = counters_new();
  counters_t *temp = counters_new();
  countersCopy(temp, hashtable_find(index, queries[current]));
  if(temp == NULL){
      return NULL;
  }
  current++;
  while(current<numQueries){
    //if hit an or, merge temp and final
    if(strcmp(queries[current], "or") == 0){
      final = unions(final, temp);
      temp = counters_new();
      countersCopy(temp,hashtable_find(index, queries[current+1]));
      current ++;
    }
    else{
      //else, just merge everything, ignoring ands
      if(strcmp(queries[current], "and")!=0){
        counters_t *temp2 = counters_new();
        countersCopy(temp2, hashtable_find(index, queries[current]));
        temp = intersects(temp, temp2);
      }
      current ++;
    }
  }
  //if there was never an or, this makes sure it works
  //otherwise, this step doesn't do anything
  final = unions(final, temp);
  return final;
}


/********** COUNTERSCOPY **********/
/* see function declaration for more information */
static void
countersCopy(counters_t *dest, counters_t *source){
  counters_iterate(source,dest,countersCopyHelper);
}

/********** COUNTERSCOPYHELPER **********/
/* see function declaration for more information */
static void 
countersCopyHelper(void *dest, const int key, const int count){
    counters_set(dest,key,count);
}

/********** INTERSECT **********/
/* see function declaration for more information */
static counters_t 
*intersects(counters_t *one, counters_t *two){
  counters_t *result = counters_new();
  countersCopy(result, one);
  counters_iterate(result, two, intersectsHelper1);
  counters_iterate(two, result, intersectsHelper2);
  counters_delete(one);
  counters_delete(two);
  return result;
}

/********** INTERSECTSHELPER1 **********/
/* see function declaration for more information */
static void 
intersectsHelper1(void *arg, const int key, int count){
  counters_t *other = arg;
  //if it doesn't exist in the other, set it to zero
  if(counters_get(other,key) == 0){
    counters_set(other, key, 0);
  }
}

/********** INTERSECTSHELPER2 **********/
/* see function declaration for more information */
static void
intersectsHelper2(void *arg, const int key, int count){
    counters_t *other = arg;
    
    if(counters_get(other,key)>count){
        counters_set(other,key,count);
    }

}

/********** UNION **********/
/* see function declaration for more information */
static counters_t 
*unions(counters_t *one, counters_t *two){
  counters_t *result = counters_new();
  counters_iterate(one, result, unionHelper);
  counters_iterate(two, result, unionHelper);
  counters_delete(one);
  counters_delete(two);
  return result;
}

/********** UNIONHELPER **********/
/* see function declaration for more information */
static void 
unionHelper(void *arg, const int key, int count){
  counters_t *other = arg;
  counters_set(other, key, count + counters_get(other,key));
}


/********** BUILDOUTPUT **********/
/* see function declaration for more information */
static void 
buildOutput(char *dir, counters_t *final){
  //find total count of documents
  if(final == NULL){
      printf("Found in 0 documents.\n");
  }
  else{
    int numDocs = 0;
    counters_iterate(final, &numDocs, docCounterHelper);
    printf("Found in %d documents.\n", numDocs);

    for(int i = 0; i<numDocs; i++){
      //find the document with the highest id
      int highest[2];
      //0 index is the id, 1st index is the count
      highest[0] = 0;
      highest[1] = 0;
      for(int i=0; i<numDocs; i++){
          counters_iterate(final, (int *)highest, sortDocsHelper);
      }
      //get url
      webpage_t *page = getWebpageFromFile(dir,highest[0]);
      char *url = webpage_getURL(page);
      printf("score %4d doc %3d : %s\n", highest[1], highest[0],url);
      webpage_delete(page);
      //set that highest value to zero, and repeat
      counters_set(final,highest[0],0);
    }
  }
}

/********** DOCCOUNTERHELPER **********/
/* see function declaration for more information */
static void 
docCounterHelper(void *arg, const int key, int count){
  if(count!=0){
    *(int *)arg = *(int *)arg + 1;
  }
}

/********** SORTDOCSHELPER **********/
/* see function declaration for more information */
static void
sortDocsHelper(void *arg, const int key, int count){
    int *highest = (int *)arg;
    if(count>highest[1]){
      highest[0] = key;
      highest[1] = count;
    }
}

/********** VALIDATEQUERY **********/
/* see function declaration for more information */
static bool validateQuery(char* queries[], int numQueries){
  if(numQueries <= 0){
    fprintf(stderr, "Invalid query.\n");
    return false;
  }
  if(strcmp(queries[0], "or") == 0 || strcmp(queries[0], "and") == 0){
    fprintf(stderr, "You cannot have \"and\" or \"or\" at the beginning of your query.\n");
    return false;
  }
  if(strcmp(queries[numQueries-1], "or") == 0 || strcmp(queries[numQueries-1], "and") == 0){
    fprintf(stderr, "You cannot have \"and\" or \"or\" at the end of your query.\n");
    return false;
  }
  for(int i = 0; i<numQueries-1; i++){
    bool connector = false;
    if(strcmp(queries[i], "or") == 0 || strcmp(queries[i], "and") == 0){
      connector = true;
    }
    if(strcmp(queries[i+1], "or") == 0 || strcmp(queries[i+1], "and") == 0){
      if(connector){
        fprintf(stderr, "You cannot have two consecutive \"and\"s or \"or\"s.\n");
        return false;
      }
    }
  }
  return true;
}

/********** PROMPT **********/
/* see function declaration for more information */
static void
prompt(void)
{
  // print a prompt iff stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}
