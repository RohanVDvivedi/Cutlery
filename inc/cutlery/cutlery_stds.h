#ifndef CUTLERY_STDS_H
#define CUTLERY_STDS_H

#ifndef NULL
	#define NULL ((void*)0)
#endif

#ifndef CHAR_BIT
	#define CHAR_BIT (8)
#endif

// below macros must be used only for unsigned integers
// returns greatest multiple of alignment lesser than or equal to x
#define UINT_ALIGN_DOWN(x, alignment) (((x) / (alignment)) * (alignment))
// returns least multiple of alignment greater than or equal to x
#define UINT_ALIGN_UP(x, alignment)   (UINT_ALIGN_DOWN(x, alignment) + ( ((x)%(alignment)) ? (alignment) : 0 ))

// by this below 2 level concatenate expansion we can first substitute params and then concatenate them
#define CAT_(a, b) a ## b
#define CAT(a, b) CAT_(a, b)

// all the bits set to 1
#define UNSIGNED_MAX_VALUE_OF(type) ((type)(~((type)(0))))

// below 2 macros only work with signed numbers, if represented with 2's complement

// only the most significant bit set to 1
#define SIGNED_MIN_VALUE_OF(type) ((type)(((type)(1)) << ((sizeof(type)*CHAR_BIT)-1)))

// all bits except the most significant bit set to 1
#define SIGNED_MAX_VALUE_OF(type) ((type)(~(SIGNED_MIN_VALUE_OF(type))))

// macro that can be used to fail a build is a certain compile time expression is true
#define fail_build_on(compile_time_expr) extern char CAT(__cutlery_build_failed__,__LINE__)[1 - (2 * (!!(compile_time_expr)))];

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
	#define CY_UINT_C(x)	((cy_uint)(x ## U))
*/

// else if (sizeof(void*) <= 16 bits)
/*
	typedef unsigned short cy_uint;
	#define PRIu_cy_uint "hu"
	#define PRIx_cy_uint "hx"
	#define CY_UINT_C(x)	((cy_uint)(x ## U))
*/

// else if (sizeof(void*) <= 32 bits)
/*
	typedef unsigned long cy_uint;
	#define PRIu_cy_uint "lu"
	#define PRIx_cy_uint "lx"
	#define CY_UINT_C(x)	(x ## UL)
*/

// else if (sizeof(void*) <= 64 bits)

	typedef unsigned long long cy_uint;
	#define PRIu_cy_uint "llu"
	#define PRIx_cy_uint "llx"
	#define CY_UINT_C(x)	(x ## ULL)


// We define cy_uint as an unsigned integer that is as wide as a void pointer
// It can hold any data pointer values
// So it can hold max value almost equal to the possible maximum number of bytes in your RAM
// Hence it can hold all possible values of size (element_count) and index of any c array

#define CY_UINT_MAX UNSIGNED_MAX_VALUE_OF(cy_uint)

// fail build if the size of cy_uint is lesser than size of void*
// we must ensure that sizeof(cy_uint) >= sizeof(void*)
fail_build_on(sizeof(cy_uint) < sizeof(void*))

#define INVALID_INDEX CY_UINT_MAX
// any array (struct array, struct dstring, c array or char array ) will never have this index
// INVALID_INDEX is used to report error (or a lack of result) to the user

#ifndef offsetof
	#define offsetof(structure, attribute) ((cy_uint)(&(((structure*)(0))->attribute)))
#endif

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
	NOTE:: data1 and data2 are processed as unsigned char arrays
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

/*
	this is a cutlery specific function to reverse contents of the data in chunks of chunk_size
	return :
		1 if success
		0 if size is not nultiple of chunk_size
	example :
		data -> 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14, size = 15, chunk_size 3
		after reverse operation
		data -> 12 13 14 09 10 11 06 07 08 03 04 05 00 01 02
		data in the chunk remains in the same order, but the chunks are reversed

	i.e. an array of ints can be reversed as follows
	memory_reverse_chunks(data, sizeof(data), sizeof(int));
*/
int memory_reverse_chunks(void* data, cy_uint size, cy_uint chunk_size);

// to reverse all bytes in the data use the below macro
#define memory_reverse(data, data_size) memory_reverse_chunks((data), (data_size), 1)

/*
	this is a cutlery specific function to right rotate a circular array of contiguous bytes
	return :
		1 if success
		0 for failure
	example :
		data -> 00 01 02 03 04, size = 5, right_rotate_amount = 2
		after right rotate operation
		data -> 03 04 00 01 02
		i.e. every byte moves circularly to the right by right_rotate_amount

		i.e. to right rotate an array of ints by right_rotate_amount_in_ints
		memory_right_rotate(data, sizeof(data), right_rotate_amount_in_ints * sizeof(int));
*/
int memory_right_rotate(void* data, cy_uint size, cy_uint right_rotate_amount);

// simple reverse of right rotate
int memory_left_rotate(void* data, cy_uint size, cy_uint left_rotate_amount);

// returns true if the byte at ptr is contained within the memory of size `size` pointed by the pointer data
// returns false if size = 0
int memory_contains(const void* data, cy_uint size, const void* ptr);

// utility function to get first aligned pointer in the memory region pointed by data of size = `size`
// returns NULL, if no such pointer is found
const void* memory_get_first_aigned_in_region(const void* data, cy_uint size, cy_uint alignment);

#endif