#include <iostream>
#include "helper.h"
#include "hashtable.h"


using namespace std;

int main(int argc, char ** argv) {
	int ht_sz, elt_sz, loops;
	get_arg(argc, argv, &ht_sz, &loops, &elt_sz);
	ht_initialize(ht_sz, elt_sz);

	return 0;
}
