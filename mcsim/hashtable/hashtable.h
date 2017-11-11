#ifndef __HASHTABLE__
#define __HASHTABLE__

//#include <stdint.h>

struct entry_s {
	char *key;
	char *value;
	struct entry_s *next;
};

typedef struct entry_s entry_t;

struct hashtable_s {
	int size;//available space
	int capacity;//max space
	struct entry_s **table;	
};

typedef struct hashtable_s hashtable_t;


hashtable_t *ht_create(int size);
int ht_hash( hashtable_t *hashtable, char *key );
entry_t *ht_newpair(int elt_size);
void ht_set( hashtable_t *hashtable, entry_t * newpair);
void ht_delete( hashtable_t *hashtable, entry_t * newpair);
hashtable_t * ht_initialize( int size, int elt_size);
void run_bypass( hashtable_t *hashtable, int size, int loops, int elt_size);
void run_cache( hashtable_t *hashtable, int size, int loops, int elt_size);

#endif
