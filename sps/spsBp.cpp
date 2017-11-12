#include <iostream>
#include "helper.h"
#include "sps.h"


using namespace std;

int main(int argc, char ** argv) {
	int array_sz, elt_sz, loops;
	get_arg(argc, argv, &array_sz, &loops, &elt_sz);
	char * array = sps_initialize(array_sz, elt_sz);
	//cout << "array sz = " << array_sz << " loops = " << loops << " elt sz = " << elt_sz << endl;
	run_bypass(array, array_sz, loops, elt_sz);

	free(array);

	return 0;
}
