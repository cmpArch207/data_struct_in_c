#include <iostream>
#include "helper.h"
#include "hashtable.h"


using namespace std;

int main(int argc, char ** argv) {
	int ht_sz, elt_sz, loops;
	get_arg(argc, argv, &ht_sz, &loops, &elt_sz);
	hashtable_t * ht = ht_initialize(ht_sz, elt_sz);
	//cout << "ht sz = " << ht_sz << " loops = " << loops << " elt sz = " << elt_sz << endl;
	run_bypass(ht, ht_sz, loops, elt_sz);

	return 0;
}
