#include <iostream>
#include "helper.h"
#include "rbtree.h"


using namespace std;

int main(int argc, char ** argv) {
	int rb_sz, elt_sz, loops;
	get_arg(argc, argv, &rb_sz, &loops, &elt_sz);
	rbtree_t * rb = rb_initialize(rb_sz, elt_sz);
	//cout << "ht sz = " << ht_sz << " loops = " << loops << " elt sz = " << elt_sz << endl;
	run_bypass(rb, rb_sz, loops, elt_sz);

	//test
	//rbtree_t * rb = rb_create(rb_sz);
	//rbtree_t * rb = rb_initialize(rb_sz, elt_sz);
	//if (rb->root == NULL) {
		//cout << "root Null\n";
		//return 1;
	//}

	//Node_t * n0 = rb_newNode(elt_sz, RED);
	//Node_t * n1 = rb_newNode(elt_sz, RED);
	
	
	//cout << "before insert: ";
	//cout << rb->size << endl;
	//rb->root = rb_insert(rb, rb->root, n0);
	//cout << "after insert: ";
	//cout << rb->size << endl;


	//cout << "before del: ";
	//cout << rb->size << endl;
	//rb_delete(rb, rb->root, n1);
	//cout << "after del: ";
	//cout << rb->size << endl;
	

	return 0;
}
