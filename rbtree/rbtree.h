#ifndef __HASHTABLE__
#define __HASHTABLE__


////global variables
//int size = 0;            //avaliable space
//int capacity = 0;        //max space

enum Color {RED, BLACK};

struct Node_s {
	char * key;
	char * value;
	struct Node_s * left, * right;
	bool color;
}

typedef struct Node_s Node_t;

struct rbtree_s {
	int size;//available space
	int capacity;//max space
	struct Node_s * dummyRoot;	
};

typedef struct rbtree_s rbtree_t;


hashtable_t *ht_create(int size);
int ht_hash( hashtable_t *hashtable, char *key );
entry_t *ht_newpair(int elt_size);
void ht_set( hashtable_t *hashtable, entry_t * newpair);
void ht_delete( hashtable_t *hashtable, entry_t * newpair);
hashtable_t * ht_initialize( int size, int elt_size);
void run_bypass( hashtable_t *hashtable, int size, int loops, int elt_size);
void run_cache( hashtable_t *hashtable, int size, int loops, int elt_size);

#endif
