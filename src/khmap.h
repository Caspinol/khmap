/** @file khmap.h
    @brief Public interface for the khmap library
    
*/
#ifndef __KHMAP_H__
#define __KHMAP_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Main hash map structure
 */
typedef struct khmap_t khmap;

/**
 * User provided hashing function.
 * If no function is specified a default hash (DJB hash)
 * will be used.
 * 
 * @param   key - value to hash
 * @return  hash - of the key
 */
typedef size_t (*do_hash)(char *key);

/**
 * User provided compare function
 * If no function specified a good 'ol strcmp will be used 
 *
 * @param c1 - void pointer to first object
 * @param c2 - void pointer to second object
 * @return true if equal or false if not 
 */
typedef bool (*do_cmp)(void *c1, void *c2);

/**
 * User provided cleanup function
 * If not specified user needs to delete the objects himself
 * 
 * @param c - void pointer to object 
 * @return none
 */
typedef void (*do_delete)(void *c);

/**
 * Creates a new hashmap object
 *
 * @param h - hash function (can be NULL)
 * @param cmp - compare function (can be NULL)
 * @param del - cleanup function (can be NULL)
 * @return none
 */
khmap *khmap_create(do_hash h, do_cmp cmp, do_delete del);

/**
 * Cleanup the hashmap object
 *
 * @param hashmap - pointer pointer to hashmap object
 * @return none
 */
void khmap_destroy(khmap **hashmap);

/**
 * Inserts the data into hm according to the key
 *
 * @param hm - the hashmap object
 * @param key 
 * @param data
 * @return 0 - success and -1 - failure
 */
int khmap_insert(khmap *hm, void *key, void *data);

/**
 * Find the data in hashmap according to specified key.
 * The object is NOT removed from the table
 *
 * @param hm - the hashmap object
 * @param key
 * @return pointer to the found object or NULL if not present
 */
void * khmap_get(khmap *hm, void *key);

/**
 * Finds the data in hashmap according to specified key.
 * The object IS removed from the table.
 * It is up to the user to cleanup the object afterwards.
 *
 * @param hm - the hashmap object
 * @param key
 * @return pointer to the found object or NULL if not present
 */
void * khmap_remove(khmap *hm, void *key);

#endif /* __KHMAP_H__ */
