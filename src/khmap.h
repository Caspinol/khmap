#ifndef __KHMAP_H__
#define __KHMAP_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct khmap_t khmap;

typedef size_t (*do_hash)(char *str);
typedef bool (*do_cmp)(void *c1, void *c2);
typedef void (*do_delete)(void *c);

khmap *khmap_create(do_hash h, do_cmp cmp, do_delete del);
void khmap_destroy(khmap **hashmap);
int khmap_insert(khmap *hm, void *key, void *data);
void * khmap_get(khmap *hm, void *key);
void * khmap_remove(khmap *hm, void *key);

#endif /* __KHMAP_H__ */
