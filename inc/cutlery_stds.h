#ifndef CUTLERY_STDS_H
#define CUTLERY_STDS_H

#ifndef NULL
	#define NULL ((void*)0)
#endif

#ifndef CHAR_BIT
	#define CHAR_BIT (8)
#endif

#ifndef UINT_MAX
	#define UINT_MAX (~((unsigned int)(0)))
#endif

#define INVALID_INDEX UINT_MAX
// any array (struct array, struct dstring, c array or char array ) will never have this index
// INVALID_INDEX is used to report error (or a lack of result) to the user

#ifndef offsetof
	#define offsetof(structure, attribute) ((unsigned int)(&(((structure*)(0))->attribute)))
#endif

/*
*	MACROS TO CONVERT FROM DATA TO NODE AND NODE TO DATA
*  This is only used by embedded node based data structures.
*  All embedded node based data structures (like linkedlist, singlylist, bst and heap)
*  store node_offset to their nodes.
*  Since, their nodes (that these datastructures operate on) are always at this fixed offset inside the user's data (structs).
*/
#define get_data(node_p, ds_p) 	(((const void*)(node_p)) - ((ds_p)->node_offset))
#define get_node(data_p, ds_p) 	(((void*)(data_p)) + ((ds_p)->node_offset))

/* this is cutlery specific memory move implementation
   this is the function to be used in place of memcpy/memmove std library functions
   features : 	
		* moves size number of bytes from src to dest pointer
		* There is protection against the memory locations overlap (similar to memmove)
		  * this is solved by reverse iteration for copying memory
		* it copies processor accessible word side if and when possible
		  * this function copies unsigned int instead of unsigned char where ever possible
*/
void memory_move(void* dest, const void* src, unsigned int size);

/*
	this is cutlery specific memory set implementation
	this is the function to be used in place of memset std library function
	features : 	
		* it sets size number of bytes at the dest pointer with the given byte_value
		* it sets processor accessible word side if and when possible
		  * this function copies unsigned int instead of unsigned char where ever possible
*/
void memory_set(void* dest_start, char byte_value, unsigned int size);

/*
	this is cutlery specific memory compare implementation
	this is the function to be used in place of memcmp std library function
	features : 	
		* it accesses processor accessible word size if and when possible
		  * this function compares unsigned int instead of unsigned char where ever possible
	return2
		1 if data1[0:(size-1)] > data2[0:(size-1)]
		-1 if data1[0:(size-1)] < data2[0:(size-1)]
		0 if data1[0:(size-1)] == data2[0:(size-1)]
*/
int memory_compare(const void* data1, const void* data2, unsigned int size);

/*
	this is cutlery specific memory swap implementation
	there is not std library replacement for this function
	this function swaps size number of bytes at the given memory locations
	it uses XOR-style in-place (no additional memory) for this swap
	it fails with a 0 if the memory locations are overlapping
	features : 	
		* it swaps processor accessible word size if and when possible
		  * this function swaps unsigned int instead of unsigned char where ever possible
	return2
		1 if success
		0 if memory locations overlap each other
*/
int memory_swap(const void* data1, const void* data2, unsigned int size);

#endif