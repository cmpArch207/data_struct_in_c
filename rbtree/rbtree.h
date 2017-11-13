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
};

typedef struct Node_s Node_t;

struct rbtree_s {
	int size;//available space
	int capacity;//max space
	struct Node_s * root;	
};

typedef struct rbtree_s rbtree_t;


char * randomStr(int length);
rbtree_t *rb_create(int size);//create a root
bool isRed(Node_t * x);
Node_t * rotateLeft(Node_t * h);
Node_t * rotateRight(Node_t * h);
void flipColors(Node_t * h);
Node_t * rb_newNode(int elt_size, bool color);
Node_t * rb_insert(rbtree_t * rb, Node_t * root, Node_t * node);
Node_t * rb_delete(rbtree_t * rb, Node_t * root, Node_t * node );
rbtree_t * rb_initialize(int size, int elt_size);//create root and then add nodes to root
void run_bypass(rbtree_t * rb, int size, int loops, int elt_size);
////void run_cache( hashtable_t *hashtable, int size, int loops, int elt_size);

#endif
