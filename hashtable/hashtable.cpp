#include <iostream>
#include <emmintrin.h>
#include <time.h> //random, clock_gettime
#include <sys/times.h>//tms struct
#include <unistd.h>
#include <string.h> //strlen
#include <climits> //ulong_max
//#include <stdlib.h> // malloc

#include "hashtable.h"
#include "helper.h"
//#include "defines.h"

using namespace std;

char * randomStr(int length) {
	char alphanum[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

	char * word = (char *) malloc (length * sizeof(char));
	for (int i = 0; i < length; ++i) {
		int index = rand() % (sizeof (alphanum) - 1);
		word[i] = alphanum[index];
	}

	return word;
}

/* Create a new hashtable. */
hashtable_t *ht_create( int size ) {

	hashtable_t *hashtable = NULL;
	int i;

	if( size < 1 ) return NULL;

	/* Allocate the table itself. */
	if( ( hashtable = (hashtable_t *)malloc( sizeof( hashtable_t ) ) ) == NULL ) {
		return NULL;
	}

	/* Allocate pointers to the
	 * head nodes. */
	if( ( hashtable->table = (entry_t **)malloc( sizeof( entry_t * ) * size ) ) == NULL ) {
		return NULL;
	}
	for( i = 0; i < size; i++ ) {
		hashtable->table[i] = NULL;
	}

	hashtable->size = size;
	hashtable->capacity = size;

	return hashtable;   
}

/* Hash a string for a particular hash table. */
int ht_hash( hashtable_t *hashtable, char *key ) {

	unsigned long int hashval;
	int i = 0;

	/* Convert our string to an integer */
	while( hashval < ULONG_MAX && i < strlen( key ) ) {
		hashval = hashval << 8;
		hashval += key[ i ];
		i++;
	}

	return hashval % hashtable->capacity;
}

/* Create a key-value pair. */
entry_t *ht_newpair(int elt_size) {
	entry_t *newpair = (entry_t *)malloc(sizeof(entry_t));

	newpair->key   = randomStr(elt_size);
	newpair->value = randomStr(elt_size);
	newpair->next = NULL;

	return newpair;
}

/* Insert a key-value pair into a hash table. */
void ht_set( hashtable_t *hashtable, entry_t * newpair) {
	int bin = 0;
	//entry_t *newpair = NULL;
	entry_t *next = NULL;
	entry_t *last = NULL;

	//newpair = ht_newpair(len);
	char * key   = newpair->key;
	char * value = newpair->value;

	bin = ht_hash( hashtable, key );

	next = hashtable->table[ bin ];

	//search the chain
	while( next != NULL && next->key != NULL && strcmp( key, next->key ) > 0 ) {
		last = next;
		next = next->next;
	}

	/* There's already a pair. Let's replace that string. */
	if( next != NULL && next->key != NULL && strcmp( key, next->key ) == 0 ) {

		free( next->value );
		next->value = strdup( value );

	/* Nope, could't find it. Time to grow a pair. */
	} else {

		if( next == hashtable->table[ bin ] ) {
			newpair->next = next;
			hashtable->table[ bin ] = newpair;
		} else if ( next == NULL ) {
			last->next = newpair;

		} else  {
			newpair->next = next;
			last->next = newpair;
		}

		//update the size
		hashtable->size--;
	}
}

void ht_delete( hashtable_t *hashtable, entry_t * newpair) {
	int bin = 0;
	entry_t *curr = NULL;
	entry_t *next = NULL;

	char * key   = newpair->key;

	bin = ht_hash( hashtable, key );

	curr = hashtable->table[ bin ];
	if ( curr != NULL) {
		hashtable->table[bin] = curr->next;
		free(curr);
		hashtable->size++;
	}
}


hashtable_t *ht_initialize( int size, int elt_size)
{

	//allocate space in memory
	hashtable_t * hashtable = ht_create(size);

	//assign key value pairs to this hashtable
	for (int i = 0; i < size; ++i) {
		entry_t * newpair = ht_newpair(elt_size);
		if (hashtable->size > 0) {
			ht_set(hashtable, newpair);
		}
		else {
			ht_delete(hashtable, newpair);
		}
	}

	return hashtable;

}


void run_bypass( hashtable_t *hashtable, int size, int loops, int elt_size) {
	//initialize log
	int * log = (int *)malloc(elt_size * 2 * size * loops);
	uint64_t start, end;
	start = rdtsc();

	for (int j = 0; j < loops; ++j) {
		for (int i = 0; i < size; ++i) {
			//cout << "j: " << j << endl;
			//cout << "i: " << i << endl;
			entry_t * newpair = ht_newpair(elt_size);
			char * key = newpair->key;
			char * value = newpair->value;
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
			if (hashtable->size > 0) {
				//insertion
				ht_set(hashtable, newpair);
			}
			else {
				//deletion
				ht_delete(hashtable, newpair);
			}
		}
	}
}



void run_cache( hashtable_t *hashtable, int size, int loops, int elt_size) {
	//initialize log
	char * log = (char *)malloc(elt_size * 2 * size * loops * 10);
	uint64_t start, end;
	start = rdtsc();

	for (int j = 0; j < loops; ++j) {
		for (int i = 0; i < size; ++i) {
			entry_t * newpair = ht_newpair(elt_size);
			char * key = newpair->key;
			char * value = newpair->value;
			//log updates
			//for simplicity, log the random kv pair
			strcpy(log, key);
			log += sizeof(key);
			strcpy(log, value);
			log += sizeof(value);

			asm volatile("sfence");

			//write working data
			if (hashtable->size > 0) {
				//insertion

			}
			else {
				//deletion
			}
		}
	}
}

