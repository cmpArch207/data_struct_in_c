#include <iostream>
#include <emmintrin.h>
#include <time.h> //random, clock_gettime
#include <sys/times.h>//tms struct
#include <unistd.h>
#include <string.h> //strlen
#include <climits> //ulong_max

#include "rbtree.h"
#include "helper.h"
//#include "defines.h"

using namespace std;

char * randomStr(int length) {
	char alphanum[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

	char * word = (char *) malloc (length * sizeof(char));
	int i;
	for (i = 0; i < length; ++i) {
		int index = rand() % (sizeof (alphanum) - 1);
		word[i] = alphanum[index];
	}
	word[i] = '\0';

	return word;
}

/* Create a new root. */
rbtree_t *rb_create( int size ) {

	rbtree_t *rbtree = NULL;
	int i;

	if( size < 1 ) return NULL;

	/* Allocate the tree itself. */
	if( ( rbtree = (rbtree_t *)malloc( sizeof( rbtree_t ) ) ) == NULL ) {
		return NULL;
	}

	//Allocate pointers to the
	//head nodes. 
	if( ( rbtree->root = (Node_t *)malloc( sizeof( Node_t * ) ) ) == NULL ) {
		return NULL;
	}
	//for( i = 0; i < size; i++ ) {
		//hashtable->table[i] = NULL;
	//}

	rbtree->size = size;
	rbtree->capacity = size;
	rbtree->root = NULL;

	return rbtree;   
}

bool isRed(Node_t * x) {
	if (x == NULL) return false;
	return x->color == RED;
}

Node_t * rotateLeft(Node_t * h) {
	Node_t * x = h->right;
	h->right = x->left;
	x->left = h;
	x->color = h->color;
	h->color = RED;
	return x;
}

Node_t * rotateRight(Node_t * h) {
	Node_t * x = h->left;
	h->left = x->right;
	x->right = h;
	x->color = h->color;
	h->color = RED;
	return x;
}

void flipColors(Node_t * h) {
	h->color = RED;
	h->left->color = BLACK;
	h->right->color = BLACK;
}

	

/* Create a key-value pair. */
Node_t *rb_newNode(int elt_size, bool clr) {
	Node_t *newnode = (Node_t *)malloc(sizeof(Node_t));

	newnode->key   = randomStr(elt_size);
	newnode->value = randomStr(elt_size);
	newnode->left = NULL;
	newnode->right = NULL;
	newnode->color = clr;

	return newnode;
}

Node_t * rb_insert(rbtree_t * rb, Node_t * root, Node_t * node) {
	if (root == NULL) {
		root = node;
		root->color = RED;
		rb->size--;
		//cout << "first node\n";
		//cout << rb->root->key << " " << rb->root->value << endl;

		return node;
	}

	int cmp = strcmp(node->key, root->key);

	if (cmp < 0) 
		root->left = rb_insert(rb, root->left, node);
	else if (cmp > 0) 
		root->right = rb_insert(rb, root->right, node);
	else // if (cmp == 0)
		root->value = node->value;

	//rotate nodes if it is necessary
	if (isRed(root->right) && !isRed(root->left))
		root = rotateLeft(root);
	if (isRed(root->left)  && isRed(root->left->left))
		root = rotateRight(root);
	if (isRed(root->left) &&isRed(root->right))
		flipColors(root);

	return root;
}


Node_t * rb_delete( rbtree_t *rb, Node_t * root, Node_t * node) {
	if (root == NULL) { 
		cout << "reach NULL\n";
		return NULL;
	}
	if (root->left == NULL && root->right == NULL) {//if reach a leave, delete it
		cout << "delete happens\n";
		free(root);
		rb->size++;
		return NULL;
	}

	int cmp = strcmp(node->key, root->key);

	if (cmp < 0) 
		root->left = rb_delete(rb, root->left, node);
	else if (cmp > 0) 
		root->right = rb_delete(rb, root->right, node);
	else // if (cmp == 0) update value
		root->value = node->value;

	//rotate nodes if it is necessary
	if (root->left == NULL && root->right != NULL)
		root = rotateLeft(root);
	if (root->left != NULL && root->right == NULL)
		root->left->color = RED; 

	return root;
}


rbtree_t *rb_initialize( int size, int elt_size)
{
	//allocate space in memory
	rbtree_t * rbtree = rb_create(size);

	//assign key value pairs to this hashtable
	for (int i = 0; i < size; ++i) {
		//entry_t * newpair = ht_newpair(elt_size);
		Node_t * node = rb_newNode(elt_size, RED);
		//if (node == NULL) 
			//cout << "null node\n";
		//else {
			//if (node->key == NULL)
				//cout << "null key\n";
			//else
				//cout << "key: " << node->key << endl;
			//if (node->value == NULL)
				//cout << "null val\n";
			//else
				//cout << "val: " << node->value << endl;
		//}
		//cout << "i = " << i << endl;

		if (rbtree->size > 0) {
			rbtree->root = rb_insert(rbtree, rbtree->root, node);
		}
		//else {
			//ht_delete(hashtable, newpair);
		//}
	}

	return rbtree;

}


void run_bypass( rbtree_t *rbtree, int size, int loops, int elt_size) {
	//initialize log
	int * log = (int *)malloc(elt_size * 2 * size * loops);
	uint64_t start, end;
	start = rdtsc();

	for (int j = 0; j < loops; ++j) {
		for (int i = 0; i < size; ++i) {
			//cout << "j: " << j << endl;
			//cout << "i: " << i << endl;
			Node_t * node = rb_newNode(elt_size, RED);
			char * key = node->key;
			char * value = node->value;
			//log updates
			//for simplicity, log the random kv pair
			int n, p;
			for (int k = 0; k < elt_size / 4; ++k) {
				p = *(int*)(key + 4 * k);
				n = *(int*)(value + 4 * k);
				_mm_stream_si32(log, p);
				log++;
				_mm_stream_si32(log, n);
				log++;
			}

			asm volatile("sfence");

			//cout << "hashtable size = " << hashtable->size << endl;
			//write working data
			if (rbtree->size > 0) {
				//insertion
				rb_insert(rbtree, rbtree->root, node);
			}
			else {
				//deletion
				rb_delete(rbtree, rbtree->root, node);
			}
		}
	}
}



//void run_cache( hashtable_t *hashtable, int size, int loops, int elt_size) {
	////initialize log
	//char * log = (char *)malloc(elt_size * 2 * size * loops * 10);
	//uint64_t start, end;
	//start = rdtsc();

	//for (int j = 0; j < loops; ++j) {
		//for (int i = 0; i < size; ++i) {
			//entry_t * newpair = ht_newpair(elt_size);
			//char * key = newpair->key;
			//char * value = newpair->value;
			////log updates
			////for simplicity, log the random kv pair
			//strcpy(log, key);
			//log += sizeof(key);
			//strcpy(log, value);
			//log += sizeof(value);

			//asm volatile("sfence");

			////write working data
			//if (hashtable->size > 0) {
				////insertion

			//}
			//else {
				////deletion
			//}
		//}
	//}
//}

