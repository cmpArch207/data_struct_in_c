#include <iostream>
#include <emmintrin.h>
#include <time.h> //random, clock_gettime
#include <sys/times.h>//tms struct
#include <unistd.h>
#include <string.h> //strlen
#include <climits> //ulong_max
//#include <stdlib.h> // malloc

#include "sps.h"
#include "helper.h"
//#include "defines.h"

using namespace std;

void randomStr(int length, char * word) {
	char alphanum[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

	int i;
	for (i = 0; i < length; ++i) {
		int index = rand() % (sizeof (alphanum) - 1);
		word[i] = alphanum[index];
	}
	word[i] = '\0';
}


char * sps_initialize( int array_size, int elt_size ) {
	char * array = (char *)malloc(array_size * elt_size); 
	char new_word[1000];// element size should be less than 1000
	if (elt_size >= 1000) {
		cout << "element size should be less than 1000\n";
		exit(1);
	}

	srand( time( NULL ) );  
	for (int i = 0; i < array_size; i++) {
		randomStr(elt_size, new_word);
		strcpy(array + i * elt_size, new_word);
		//cout << new_word << endl;
		//cout << idx << endl;
	}
	//cout << "finish init\n";

	//for (int i = 0; i < array_size * elt_size; ++i) {
		//cout << array[i] << endl;
	//}

	//cout << "array size = ";
	//cout << sizeof(array) << endl;

	return array;
}


void run_bypass( char *array, int size, int loops, int elt_size) {
	//if (array == NULL) {
		//cout << "empty array\n";
		//return;
	//}

	//initialize log
	int * log = (int *)malloc(elt_size * size * loops * 2); //store 2 strings in each loop
	uint64_t start, end;
	start = rdtsc();
	//char * val0 = (char *)malloc(elt_size);
	//char * val1 = (char *)malloc(elt_size);	
	char val0[1000], val1[1000];

	//for (int i = 0; i < size * elt_size; ++i) {
		//cout << array[i] << endl;
	//}

	for (int j = 0; j < loops; ++j) {
		for (int i = 0; i < size; ++i) {
			//cout << "j: " << j << endl;
			//cout << "i: " << i << endl;
			
			//custmized memcpy
			for (int k = 0 ; k < elt_size; ++k) {
				val0[k] = array[i + k];
				val1[k] = array[size - 1 - i + k];
			}

			//strcpy(val0, &array[i]);
			//strcpy(val1, &array[size - 1 - i]);
			//cout << "j: " << j << endl;
			//cout << "i: " << i << endl;
			//log updates
			//for simplicity, log the random kv pair
			int v0, v1;
			for (int k = 0; k < elt_size / 4; ++k) {
				v0 = *(int*)(val0 + 4 * k);
				v1 = *(int*)(val1 + 4 * k);
				_mm_stream_si32(log, v0);
				log++;
				_mm_stream_si32(log, v1);
				log++;
			}

			asm volatile("sfence");

			//write working data, swap 2 elements
			//custmized memcpy
			for (int k = 0 ; k < elt_size; ++k) {
				array[i + k] = val0[k];
				array[size - 1 - i + k] = val1[k] ;
			}
			//strcpy(&array[i], val1);
			//strcpy(&array[size - 1 -i], val0);
		}
	}

	//free(val0);
	//free(val1);
	//cout << "finish run\n";
	end = rdtsc();
	cout << "total cycles = " << end - start << endl;
}



void run_cache( char * array, int size, int loops, int elt_size) {
	//initialize log
	char * log = (char *)malloc(elt_size * 2 * size * loops * 10);
	uint64_t start, end;
	start = rdtsc();

	for (int j = 0; j < loops; ++j) {
		for (int i = 0; i < size; ++i) {
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
		}
	}
}

