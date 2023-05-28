#ifndef CUTLERY_STDS_H
#define CUTLERY_STDS_H

// macro that can be used to fail a build is a certain compile time expression is true
#define fail_build_on(compile_time_expr) char CAT(_,__LINE__)[1-(!!(compile_time_expr))];

#ifndef NULL
	#define NULL ((void*)0)
#endif

#ifndef CHAR_BIT
	#define CHAR_BIT (8)
#endif

// by this below 2 level concatenate expansion we can first substitute params and then concatenate them
#define CAT_(a, b) a ## b
#define CAT(a, b) CAT_(a, b)

// Appropriately typedef cy_uint, depending on the size of void* on your system
// cy_uint is a cutlery's drop in replacement for size_t
// cy_uint is capable to hold indexes, sizes, capacities and element counts of (structures <where applicable> and ) C arrays of all valid C structures
// cy_uint is atleast as wide as the void*, i.e. it is also capable to hold any data and function pointer
// it fills in a unique niche in Cutlery

// if (sizeof(void*) <= 8 bits)
/*
	typedef unsigned char cy_uint;
	#define PRIu_cy_uint "hhu"
	#define PRIx_cy_uint "hhx"
*/

// else if (sizeof(void*) <= 16 bits)
/*
	typedef unsigned short cy_uint;
	#define PRIu_cy_uint "hu"
	#define PRIx_cy_uint "hx"
*/

// else if (sizeof(void*) <= 32 bits)
/*
	typedef unsigned long cy_uint;
	#define PRIu_cy_uint "lu"
	#define PRIx_cy_uint "lx"
*/

// else if (sizeof(void*) <= 64 bits)

	typedef unsigned long long cy_uint;
	#define PRIu_cy_uint "llu"
	#define PRIx_cy_uint "llx"


// We define cy_uint as an unsigned integer that is as wide as a void pointer
// It can hold any data pointer values
// So it can hold max value almost equal to the possible maximum number of bytes in your RAM
// Hence it can hold all possible values of size (element_count) and index of any c array

#define CY_UINT_MAX (~((cy_uint)(0)))

#define INVALID_INDEX CY_UINT_MAX
// any array (struct array, struct dstring, c array or char array ) will never have this index
// INVALID_INDEX is used to report error (or a lack of result) to the user

#ifndef offsetof
	#define offsetof(structure, attribute) ((cy_uint)(&(((structure*)(0))->attribute)))
#endif

// fail build if the size of cy_uint is lesser than size of void*
fail_build_on(sizeof(cy_uint) < sizeof(void*))

/*
*	MACROS TO CONVERT FROM DATA TO NODE AND NODE TO DATA
*  This is only used by embedded node based data structures.
*  All embedded node based data structures (like linkedlist, singlylist, bst and heap)
*  store node_offset to their nodes.
*  Since, their nodes (that these datastructures operate on) are always at this fixed offset inside the user's data (structs).
*/
#define get_data(node_p, ds_p) 	(((const void*)(node_p)) - ((ds_p)->node_offset))
#define get_node(data_p, ds_p) 	(((void*)(data_p)) + ((ds_p)->node_offset))

// in array or bst you can search an element that equals, returning first or last occurence
// this is the enum that is used as parameter to those function
typedef enum search_occurence search_occurence;
enum search_occurence
{
	FIRST_OCCURENCE = 0,
	LAST_OCCURENCE
};

/* this is cutlery specific memory move implementation
   this is the function to be used in place of memcpy/memmove std library functions
   features :
		* moves size number of bytes from src to dest pointer
		* There is protection against the memory locations overlap (similar to memmove)
		  * this is solved by reverse iteration for copying memory
		* it copies processor accessible word side if and when possible
		  * this function copies unsigned int instead of unsigned char where ever possible
	return :
		1 if success or a memory_move was not required
		0 otherwise
*/
int memory_move(void* dest, const void* src, cy_uint size);

/*
	this is cutlery specific memory set implementation
	this is the function to be used in place of memset std library function
	features :
		* it sets size number of bytes at the dest pointer with the given byte_value
		* it sets processor accessible word side if and when possible
		  * this function copies unsigned int instead of unsigned char where ever possible
	return :
		1 if success or a memory_set was not required
		0 otherwise
*/
int memory_set(void* dest_start, char byte_value, cy_uint size);

/*
	this is cutlery specific memory compare implementation
	this is the function to be used in place of memcmp std library function
	features :
		* it accesses processor accessible word size if and when possible
		  * this function compares unsigned int instead of unsigned char where ever possible
	return :
		1 if data1[0:(size-1)] > data2[0:(size-1)]
		-1 if data1[0:(size-1)] < data2[0:(size-1)]
		0 if data1[0:(size-1)] == data2[0:(size-1)]
*/
int memory_compare(const void* data1, const void* data2, cy_uint size);

/*
	this is cutlery specific memory swap implementation
	there is not std library replacement for this function
	this function swaps size number of bytes at the given memory locations
	it uses XOR-style in-place (no additional memory) for this swap
	it fails with a 0 if the memory locations are overlapping
	features :
		* it swaps processor accessible word size if and when possible
		  * this function swaps unsigned int instead of unsigned char where ever possible
	return :
		1 if success
		0 if memory locations overlap each other
*/
int memory_swap(void* data1, void* data2, cy_uint size);

#endif