/*
 * counters.c - implements a set of integer counters
 * with int keys (must be non-negative) 
 *
 * Elizabeth Frey
 * Winter 2021
 * CS 50 Lab 3
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>//check if this is needed
#include "counters.h"
#include "memory.h"

/********** file-local global variables **********/
/* none */

/********** local types **********/
typedef struct counternode{
    struct counternode *next;
    int key;
    int count;
} counternode_t;

/********** global types **********/
typedef struct counters{
    struct counternode *head;
} counters_t;

/********** global functions **********/
/* see counter.h for comments about exported functions */

/********** local functions **********/
static counternode_t *counternode_new(int key, int count);

/********** counters_new() **********/
/* see counters.h for description */
counters_t 
*counters_new(void){
    //allocate memory for counter
    counters_t *counter = count_malloc(sizeof(counters_t));

    if(counter == NULL){
        //if memory allocation fails
        count_free(counter);
        return NULL;
    }
    else{
        //counter set is initialized empty
        counter->head = NULL;
        return counter;
    }
}

/********** counters_add() **********/
/* see counters.h for description*/
int
counters_add(counters_t *ctrs, const int key){
    //if key is negative, return 0
    if(key < 0){
        return 0;
    }
    else if (ctrs == NULL){ //if counters is null
        return 0;
    }
    else{
        //if key doesn't exist, make new counter with value 1
        if(counters_get(ctrs, key) == 0){
            counternode_t *new = counternode_new(key, 1);
            new->next = ctrs->head;
            ctrs->head = new;
            return 1;
        }
        //if key does exist
        else{
            counternode_t *node = ctrs->head;
            //find the node with the key
            while(node->key != key)
            {
                node = node->next;
            }
            //increment its counter by 1
            node->count= node->count + 1;
            return node->count;
        }
    }
}

/********** counternode_new **********/
/* allocate and initialize a counternode */
static counternode_t *
counternode_new(int key, int count){
    counternode_t *node = count_malloc(sizeof(counternode_t));

    if(node == NULL){
        //if memory allocation fails
        count_free(node);
        return NULL;
    }
    else{
        //set the nodes key to the key and count to count
        node->key = key;
        node->count = count;
        node->next = NULL;
        return node;
    }
}

/********** counters_get **********/
/* see counters.h for description */
int 
counters_get(counters_t *ctrs, const int key){
    //if counters isnt null and key is valid (>=0)
    if(ctrs != NULL && key >= 0){
        //search for the node
        for(counternode_t *node = ctrs->head; node != NULL; 
                node = node->next){
            //if the node is found, return its count
            if(node->key == key){
                return node->count;
            }
        }
    }
    //if counters is NULL or key is not found
    return 0;
}

/********** counters_set **********/
/* see counters.h for description */
bool
counters_set(counters_t *ctrs, const int key, const int count){
    //if counters isn't NULL, key isnt negative, and count is valid
    if(ctrs != NULL && key >= 0 && count >= 0){
        //if counter doesn't exist
        if(counters_get(ctrs, key)==0){
            counternode_t *new = counternode_new(key, count);
            if(new != NULL){
                new->next = ctrs->head;
                ctrs->head = new;
                return true;
            }
            else{
                //if for some reason new counternode is null
                //(out of memory)
                return false;
            }
        }
        //if it exists
        else {
            //find the node
            counternode_t *node = ctrs->head;
            while(node->key != key)
            {
                node = node->next;
            }
            //set the new count to the given value
            node->count = count;
            return true;
        }
    }
    //if counters is null, key<0 or count<0
    return false;
}

/********** counters_print **********/
/* see counters.h for description */
void
counters_print(counters_t *ctrs, FILE *fp){
    if(fp != NULL){
        if(ctrs != NULL){
            fputc('{',fp);
            for(counternode_t *node = ctrs -> head; node != NULL; node = node->next){
                fprintf(fp,"(%d=%d)",node->key, node->count);
                //if isn't the last node
                if(node->next != NULL){
                    fputc(',',fp);
                }
            }
            fputc('}', fp);
        }
        else{
            //if ctrs is null, print (null)
            fputs("(null)",fp);
        }
    }
    //if fp is null, don't do anything
}

/********** counters_iterate() **********/
/* see counters.h for description */
void
counters_iterate(counters_t *ctrs, void *arg, void(*itemfunc)(void *arg,
            const int key, const int count)){
    if(ctrs != NULL && itemfunc != NULL){
        //iterate through each node and call item func on it
        for(counternode_t *node = ctrs->head; node!=NULL; 
                node = node-> next){
            (*itemfunc)(arg, node->key, node->count);
        }
    }
    //do nothing if ctrs == NULL or itemfunc == NULL
}

/********** counters_delete() **********/
/* see counters.h for description */
void
counters_delete(counters_t *ctrs){
    if(ctrs != NULL){
        for(counternode_t *node = ctrs->head; node!=NULL; ){
            counternode_t *next = node->next;
            count_free(node);
            node = next;
        }
        count_free(ctrs);
    }
    //if ctrs == null, do nothing
}
/* *********************************************************** */
/* Unit test: glass-box test of the counters module.
 * Create a counter, work with it, and verify its innards.
 * based off code from bag.c
 */

/* *********************************************************** */
/* Unit test: glass-box test of the bag module.
 *  * Create a bag, work with it, and verify its innards.
 *   */
#ifdef UNIT_TEST
#include <stdbool.h>

//file-local global variables
static int unit_tested = 0;     // number of test cases run
static int unit_failed = 0;     // number of test cases failed

// a macro for shorthand calls to expect()
#define EXPECT(cond) { unit_expect((cond), __LINE__); }

// Checks 'condition', increments unit_tested, prints FAIL or PASS
void unit_expect(bool condition, int linenum)
{
   unit_tested++;
     if (condition) {
         printf("PASS test %03d at line %d\n", unit_tested, linenum);
           } else {
               printf("FAIL test %03d at line %d\n", unit_tested, linenum);
                   unit_failed++;
           }
}

/**** test item func ****/
/* prints the count of the item plus the key to stdout (random function)
 */
static void 
testitemfunc(void *arg, const int key, const int count){
    int sum = key + count;
    FILE *itemfunc = fopen("itemfunc-result.txt", "w");
    fprintf(itemfunc, "random func on item: %d", sum);
    fclose(itemfunc);
}

/****** main ******/
/*
 * main for the unittest
 */
int
main(){
    //make a counter
    counters_t *ctrs = counters_new();
    EXPECT(ctrs!=NULL);
    EXPECT(ctrs->head == NULL);

    //put things into the counter
    EXPECT(counters_add(ctrs, 1) == 1);
    EXPECT(counters_add(ctrs, 2) == 1);
    //check to make surre ignore if ctrs is NULL or key is negative
    EXPECT(counters_add(NULL, 2) == 0);
    EXPECT(counters_add(ctrs, -3) == 0);

    //print the counter
    FILE *fpwrite = fopen("output.txt", "w");
    counters_print(ctrs, fpwrite);

    //get things from the counter
    EXPECT(counters_get(ctrs, 2) == 1);
    //chcck to see if return 0 if ctrs is NULL or key not found or <0
    EXPECT(counters_get(ctrs, 10) == 0);
    EXPECT(counters_get(NULL, 3) == 0);
    EXPECT(counters_get(ctrs, -1) == 0);

    //set counts in the counter and test for boundary
    EXPECT(counters_set(ctrs, 2, 10));
    EXPECT(counters_set(NULL, 2, 1) == false);
    EXPECT(counters_set(ctrs, -1, 2) == false);
    EXPECT(counters_set(ctrs, 2, -5) == false);

    //get the modified thing from the counter
    EXPECT(counters_get(ctrs, 2) == 10);

    //test counters_iterate
    //test function just prints the sum of key + count to a random file
    counters_iterate(ctrs, NULL, testitemfunc);

    //delete the counter
    counters_delete(ctrs);
    fclose(fpwrite);
    if (unit_failed > 0) {
        printf("FAILED %d of %d tests\n", unit_failed, unit_tested);
        return unit_failed;
    } 
    else {
        printf("PASSED all of %d tests\n", unit_tested);
        return 0;
    }
}
#endif // UNIT_TEST

