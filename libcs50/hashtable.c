/*
 * hashtable.c - implements a set of void* with char* keys.
 * acts like a set, but is far more efficient.
 *
 * Elizabeth Frey
 * Winter 2021
 * CS 50 Lab 3
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "jhash.h"
#include "memory.h"
#include "set.h"

/********** file-local global variables **********/
/* none */

/********** local types **********/
/* none */

/********** global types **********/
typedef struct hashtable{
    set_t **htable;
    int num_slots;
} hashtable_t;

/********** global functions **********/
/* see hashtable.h for comments about exported functions  */

/********** local functions **********/
/* none */

/********** hashtable_new() **********/
/* see hashtable.h for description */
hashtable_t 
*hashtable_new(const int num_slots){
    //if caller provides slots not >0
    if(num_slots < 1){
        return NULL;
    }
    hashtable_t *ht = count_malloc(sizeof(hashtable_t));
   
    if(ht == NULL){
        //if error
        count_free(ht);
        return NULL;
    }

    ht->htable = count_calloc(num_slots, sizeof(struct set_t*));
    if(ht->htable == NULL){
        //if error allocating
        count_free(ht->htable);
        return NULL;
    }

    ht->num_slots = num_slots;
    for(int i = 0; i<num_slots; i++){
        //hashtable initialized empty
        ht->htable[i] = set_new();
    }
    return ht;
}

/********** hashtable_insert() **********/
/* see hashtable.h for description */
bool
hashtable_insert(hashtable_t *ht, const char *key, void *item){
    //if the key exists
    if(ht == NULL || key == NULL || item == NULL || 
            hashtable_find(ht, key) != NULL){
        //if any parameter is null
        return false;
    }
    else{
        int index = JenkinsHash(key, ht->num_slots);
        if(ht->htable[index] != NULL){
            //if key exists, will be in the set, return false
            if(set_find(ht->htable[index], key) != NULL){
                return false;
            }
            //set_insert will allocate space for all the keys
            return(set_insert(ht->htable[index],key,item));
        }
        //if error
        return false;
    }
}

/********** hashtable_find() **********/
/* see hashtable.h for description */
void
*hashtable_find(hashtable_t *ht, const char *key){
    if(ht == NULL || key == NULL || ht->htable == NULL){
        return NULL;
        //if any parameter is null
    }
    for(int i = 0; i<ht->num_slots; i++){
        int index = JenkinsHash(key, ht->num_slots);
        if(set_find(ht->htable[index], key) != NULL){
            return set_find(ht->htable[index], key);
        }
    }
    //if key not found
    return NULL;
}

/********** hashtable_print() **********/
/* see hashtable.h for description */
void
hashtable_print(hashtable_t *ht, FILE *fp, 
        void (*itemprint)(FILE *fp, const char *key, void *item)){
    if(fp != NULL){
        if(ht!=NULL){
            for(int i = 0; i<ht->num_slots; i++)
            {
                if(itemprint == NULL){
                    fputc('\n', fp);
                }
                else{
                    set_print(ht->htable[i], fp, itemprint);
                }
                fputc('\n', fp);
            }
        }
        else{
            //if hashtable is null, print (null)
            fputs("(null)", fp);
        }
        //if fp == null, do nothing
    }
}
/********** hashtable_iterate() **********/
/* see hashtable.h for description */
void
hashtable_iterate(hashtable_t *ht, void *arg, 
        void (*itemfunc)(void *arg, const char *key, void *item)){
    if(ht != NULL && itemfunc != NULL){
        for(int i = 0; i<ht->num_slots; i++){
            //call itemfunc on every item
            set_iterate(ht->htable[i], arg, itemfunc);
        }
    }
    //if ht is NULL or itemfunc is NULL, do nothing
}

/********** hashtable_delete() **********/
/* see hashtable.h for description */
void
hashtable_delete(hashtable_t *ht, void(*itemdelete)(void *item)){
    if(ht != NULL){
        for(int i = 0; i<ht->num_slots; i++){
            //set_delete will free all the keys
            set_delete(ht->htable[i], itemdelete);
        }
        count_free(ht->htable);
        count_free(ht);
        ht = NULL;
    }
    //if hashtable is null, do nothing
}

/* ********************************************* */
/* Unit test: glass-box test of the set module
 * based off of code form bag.c
 * Create a set, work with it, and verify its innards.
 */
 
#ifdef UNIT_TEST
#include <stdbool.h>

//file-local global variables
static int unit_tested = 0;
static int unit_failed = 0;

#define EXPECT(cond) {unit_expect((cond), __LINE__);}

//from bag.c
void unit_expect(bool condition, int linenum){
    unit_tested++;
    if(condition){
        printf("PASS test %03d at line %d\n", unit_tested, linenum);
    }
    else{
        printf("FAIL test %03d at line %d\n", unit_tested, linenum);
        unit_failed++;
    }
}

static void
testitemprint(FILE *fp, const char *key, void *item){
    fprintf(fp, "(%s -> %d)", key, (*(int*)item));
}

static void 
testitemfunc(void *arg, const char *key, void *item){
    (*((int*)item))++;
}

int
main(){

    //make a new hashtable
    hashtable_t *ht = hashtable_new(2);
    EXPECT(ht!=NULL);
    int a = 1, b = 2, c = 3;
    const char *key1 = "one", *key2 = "two";

    //check insert
    EXPECT(hashtable_insert(ht, key1, &a));
    EXPECT(hashtable_insert(ht, key2, &b));
    //if key already exists
    EXPECT(!hashtable_insert(ht, key1, &c));
    EXPECT(!hashtable_insert(NULL, key2, &c));

    //check contents
    EXPECT(hashtable_find(ht, key1) != NULL);
    EXPECT(hashtable_find(ht, "nonexistent key") == NULL);
    EXPECT(hashtable_find(NULL, key1) == NULL);
    EXPECT(hashtable_find(ht, NULL) == NULL);

    //try to print
    FILE *fpwrite = fopen("output.txt", "w");
    fprintf(fpwrite, "print test 1:\n");
    hashtable_print(ht, fpwrite, testitemprint);

    //test iterating
    hashtable_iterate(ht, NULL, NULL);
    EXPECT(*((int*)hashtable_find(ht, key1)) == 1);
    hashtable_iterate(ht, NULL, testitemfunc);
    EXPECT(*((int*)hashtable_find(ht, key1)) == 2);
    hashtable_print(ht, fpwrite, testitemprint);
    hashtable_delete(ht, NULL);

    //close file
    fclose(fpwrite);

    if(unit_failed>0){
        printf("FAILED %d of %d tests\n", unit_failed, unit_tested);
        return unit_failed;
    }
    else{
        printf("PASSED all of %d tests\n", unit_tested);
        return 0;
    }
}
#endif //UNIT_TEST
