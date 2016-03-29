#include "khmap.h"

/* Gonna use a open addressing with double hashing */

#define INITIAL_SIZE (101) /* First prime in 100s range */
#define INITIAL_STEP_PRIME (97) /* The prime incremented every resize to get new step */
#define MAX_LOAD ((float)0.75) /* Anything above this requires resizing */

/* Calculates the new step in case of collision */
#define HASH_STEP(IDX, H, MAP)  ((IDX + (MAP->step_prime - (H % MAP->step_prime))) % MAP->size)

static size_t _hash(char *str);
static bool _compare(void *c1, void *c2);
static void _delete(void *c);
static int _resize(khmap **hashmap);
static uint32_t _next_prime(uint32_t n);
static bool _is_prime(uint32_t n);

typedef enum node_state_e{
	EMPTY = 0,
	USED,
	DELETED
}node_state;

typedef struct khmap_node_t{
	void *data;                      /* The data */
	char *key;                       /* The key */
	node_state ns;                   /* Node state */
}khmap_node;

struct khmap_t{
	struct khmap_node_t **table;
	size_t size;                     /* How long is the list */
	size_t count;                    /* How many live nodes it has */
	size_t step_prime;             /* The prime used to find the step size */
	do_hash this_hash;               /* Hashing function */
	do_cmp this_cmp;                 /* Compare function */
	do_delete this_delete;           /* Data delete function */
};

static size_t _hash(char *str){
	
    uint32_t hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

static bool _compare(void *c1, void *c2){
	return !strcmp((char *)c1, (char *)c2);
}

static void _delete(void *c){
	(void)c;
}

static int _resize(khmap **_hashmap){
	khmap *hm = *_hashmap;

	/* Preserve the original table */
	khmap_node **t = hm->table;
	size_t orig_len = hm->size;

	/* 
	   Create new and bigger table thats a least 
	   2x the original size and prime
	*/
	hm->size = _next_prime(3 * orig_len);
	hm->step_prime = _next_prime(3 * hm->step_prime);
	if(hm->step_prime >= hm->size){
		printf("step issue\n");
	}
	hm->table = malloc(hm->size * sizeof(*hm->table));
	if(!hm->table){
		return -1;
	}

	for(uint32_t i = 0; i < hm->size; i++){
		hm->table[i] = NULL;
	}

	/* Reset the object count */
	hm->count = 0;

	/* Now rehash all the objects */
	for(uint32_t i = 0; i < orig_len; i++){
		if(t[i] && t[i]->ns == USED){
			khmap_insert(hm, t[i]->key, t[i]->data);
		}
		free(t[i]);
	}

	/* Now cleanup the old list */
	free(t);
	
	return 0;
}

khmap *khmap_create(do_hash h, do_cmp cmp, do_delete del){
	khmap *hashmap;

	hashmap = malloc(sizeof(*hashmap));
	if(!hashmap) return NULL;
	
	hashmap->size = INITIAL_SIZE;
	hashmap->step_prime = INITIAL_STEP_PRIME;
	hashmap->count = 0;
	hashmap->this_hash = h ? h : _hash;
	hashmap->this_cmp = cmp ? cmp : _compare;
	hashmap->this_delete = del ? del : _delete;

	hashmap->table = malloc(hashmap->size * sizeof(*hashmap->table));
	if(!hashmap->table){
		free(hashmap);
		return NULL;
	}

	for(uint32_t i = 0; i < hashmap->size; i++){
		hashmap->table[i] = NULL;
	}

	return hashmap;
}

void khmap_destroy(khmap **hashmap){
	khmap *hm = *hashmap; 
	for(size_t i = 0; i < hm->size; i++){
		if(hm->table[i] && hm->table[i]->ns == USED){
			hm->this_delete(hm->table[i]->data);
		}
		free(hm->table[i]);
	}
	free(hm->table);
	free(hm);

	*hashmap = NULL;
}

int khmap_insert(khmap *hm, void *key, void *data){

	size_t h = 0, idx = -1;
	
	if(!key) return -1;
	
	h = hm->this_hash(key);
	idx = h % hm->size;

	while(hm->table[idx] && hm->table[idx]->ns == USED){
		/* if used just keep looking */
		idx = HASH_STEP(idx, h, hm);
	}

	/* Found empty slot so insert data */
	if(hm->table[idx] == NULL){
		khmap_node *node = malloc(sizeof(*node));
		if(!node){
			return -1;
		}
		*node = (khmap_node){
			.key = key,
			.data = data,
			.ns = USED
		};
		
		hm->table[idx] = node;
	}else{
		/* Not NULL but has been deleted */
		hm->table[idx]->key = key;
		hm->table[idx]->data = data;
		hm->table[idx]->ns = USED;
	}

	hm->count++;

	/* Check the load and resize if necessary */
	if(MAX_LOAD <= ((float)hm->count/hm->size)){
		_resize(&hm);
	}

	return 0;
}

void * khmap_get(khmap *hm, void *key){

	size_t h = 0, idx = -1;

	if(!key) return NULL;

	h = hm->this_hash(key);
	idx = h % hm->size;

	while(hm->table[idx]){

		if(hm->table[idx]->ns == USED && hm->this_cmp(hm->table[idx]->key, key)){
			return hm->table[idx]->data;
		}
		/* else its been deleted so keep looking */
		idx = HASH_STEP(idx, h, hm);
	}
	
	return NULL;
}

void *khmap_remove(khmap *hm, void *key){

	size_t h = 0, idx = -1;

	if(!key) return NULL;

	h = hm->this_hash(key);
	idx = h % hm->size;

	while(hm->table[idx]){
		
		if(hm->table[idx]->ns == USED && hm->this_cmp(hm->table[idx]->key, key)){
			hm->table[idx]->ns = DELETED;

			hm->count--;
			
			return hm->table[idx]->data;
		}
		idx = HASH_STEP(idx, h, hm);
	}
	
	return NULL;
}

static uint32_t _next_prime(uint32_t number){

	while(!_is_prime(++number));

	return number;
}

static bool _is_prime(uint32_t p){
	for(uint32_t i = 2; i < p; i++){

		uint32_t r = p / i;
		if(r < i) return true;

		if(r * i == p) return false;
	}
	return true;
}
