#ifndef __HASHTABLE__
#define __HASHTABLE__

//#include <stdint.h>

//struct entry_s {
	//char *key;
	//char *value;
	//struct entry_s *next;
//};

//typedef struct entry_s entry_t;

//struct hashtable_s {
	//int size;//available space
	//int capacity;//max space
	//struct entry_s **table;	
//};

//typedef struct hashtable_s hashtable_t;


void randomStr(int elt_size, char * str);
char * sps_initialize( int size, int elt_size);
void run_bypass( char * array, int size, int loops, int elt_size);
void run_cache( char * array, int size, int loops, int elt_size);

#endif
