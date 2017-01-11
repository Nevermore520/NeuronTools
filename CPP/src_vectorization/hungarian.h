#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef RANDOM_DEVICE
#define RANDOM_DEVICE "/dev/urandom"
#endif


#define cell      double
#define CELL_STR  "%li"

#define llong    int_fast64_t
#define byte     int_fast8_t
#define boolean  int_fast8_t
#define null     0
#define true     1
#define false    0



#ifdef DEBUG
#  define debug(X) fprintf(stderr, "\033[31m%s\033[m\n", X)
#else
#  define debug(X) 
#endif




/**
* Cell marking:  none
*/
#define UNMARKED  0L

/**
* Cell marking:  marked
*/
#define MARKED    1L

/**
* Cell marking:  prime
*/
#define PRIME     2L



/**
* Bit set, a set of fixed number of bits/booleans
*/
typedef struct
{
	/**
	* The set of all limbs, a limb consist of 64 bits
	*/
	llong* limbs;

	/**
	* Singleton array with the index of the first non-zero limb
	*/
	size_t* first;

	/**
	* Array the the index of the previous non-zero limb for each limb
	*/
	size_t* prev;

	/**
	* Array the the index of the next non-zero limb for each limb
	*/
	size_t* next;

} BitSet;


typedef int ssize_t;

ssize_t** kuhn_match(cell** table, size_t n, size_t m);
void kuhn_reduceRows(cell** t, size_t n, size_t m);
byte** kuhn_mark(cell** t, size_t n, size_t m);
boolean kuhn_isDone(byte** marks, boolean* colCovered, size_t n, size_t m);
size_t* kuhn_findPrime(cell** t, byte** marks, boolean* rowCovered, boolean* colCovered, size_t n, size_t m);
void kuhn_altMarks(byte** marks, size_t* altRow, size_t* altCol, ssize_t* colMarks, ssize_t* rowPrimes, size_t* prime, size_t n, size_t m);
void kuhn_addAndSubtract(cell** t, boolean* rowCovered, boolean* colCovered, size_t n, size_t m);
ssize_t** kuhn_assign(byte** marks, size_t n, size_t m);

BitSet new_BitSet(size_t size);
void BitSet_set(BitSet thisBS, size_t i);
void BitSet_unset(BitSet thisBS, size_t i);
ssize_t BitSet_any(BitSet thisBS) /*__attribute__((pure))*/;

size_t lb(llong x) /*__attribute__((const))*/;



void print(cell** t, size_t n, size_t m, ssize_t** assignment);
