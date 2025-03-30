#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include<cutlery/array.h>

#include<cutlery/probabilistic_data_structure_util.h>

/*
** Bloom Filter is a probablistic data structure
** It intends to provide you with a check to see if an object/data has been seen by you or not
** Since it is a probalistic data structure it may return false positives, but there are no false negatives
** higher the total number of buckets (number of hash functions * bucket_count per hash function) higher is its accuracy
** (with ofcourse better hash_functions with wide distribution and uniform over the range)
**
** you must call "insert_in_bloom_filter" to mark a data as seen
** while you can call "exists_in_bloom_filter" any time to check if a piece of data was seen earlier. (i.e. was marked earlier as seen)
*/

typedef struct bloom_filter bloom_filter;
struct bloom_filter
{
	// array consisting of all hash functions
	array data_hash_functions;

	// there are capacity_in_bytes number of bytes at address pointed by bitmap
	// each data_hash_function has ((capacity_in_bytes * CHAR_BIT) / get_capacity_array(data_hash_functions)) number of buckets
	// total number of buckets = get_capacity_array(data_hash_functions) * ((capacity_in_bytes * CHAR_BIT) / get_capacity_array(data_hash_functions))
	cy_uint capacity_in_bytes;

	// This is a bitmap consisting total of (capacity_in_bytes * CHAR_BIT) number of bits
	// this bitmap is a single chunk of memory
	// if the bitmap was managed as array by the c language, then it would be accessible as bitmap[ ith_hash_function ][ jth_bit ]
	// here we will use the functionality defined in bitmap.h and multi_dim_array_util.h to create a packed (single allocation) multi dimensional bitmap
	char* bitmap;

	// memory allocator to be used for above mentioned bitmap
	// by default it would be set to STD_c_mem_allocator
	memory_allocator bitmap_allocator;
};

// initialize_bloom_filter* functions may fail, if the initial memory allocation fails
// in case of a failure, there has not been any memory allocation, and the error is fatal, the bloom filter instance is unusable

int initialize_bloom_filter(bloom_filter* bf_p, cy_uint bucket_count, cy_uint hash_functions_count, const data_hash_func data_hash_functions[]);

int initialize_bloom_filter_with_allocator(bloom_filter* bf_p, cy_uint bucket_count, cy_uint hash_functions_count, const data_hash_func data_hash_functions[], memory_allocator data_hash_functions_array_allocator, memory_allocator bitmap_allocator);

// in the initialize with_memory function the bitmap parameter (the last parameter) is optional
// it can be provided as NULL, if you want to use STD_C_mem_allocator for allocating and maintaining bitmap
int initialize_bloom_filter_with_memory(bloom_filter* bf_p, cy_uint bucket_count, cy_uint hash_functions_count, const data_hash_func data_hash_functions[], char bitmap[]);

// this is the number of hash functions that would be used by the bloom filter
// equivalent to get_capacity_array(&(bf_p->data_hash_functions));
cy_uint get_hash_function_count_bloom_filter(const bloom_filter* bf_p);

// this is the number of buckets per hash function in the bloom filter
// equivalent to (bf_p->capacity_in_bytes * CHAR_BIT) / get_hash_function_count_bloom_filter(bf_p)
cy_uint get_bucket_count_per_hash_function_bloom_filter(const bloom_filter* bf_p);

void insert_in_bloom_filter(bloom_filter* bf_p, const void* data, cy_uint data_size);

typedef enum bloom_filter_presence bloom_filter_presence;
enum bloom_filter_presence
{
	ABSENT = 0,
	MAY_BE_PRESENT = 1,
};

// a result of exists in bloom_filter is 
// a definite not present (i.e. not present with 100% surety)
// or it may be present (it could be present or absent)
bloom_filter_presence exists_in_bloom_filter(const bloom_filter* bf_p, const void* data, cy_uint data_size);

// all bits will be reset i.e. all the elements will be made ABSENT
void reset_bits_in_bloom_filter(bloom_filter* bp_p);

double get_fraction_of_bloom_filter_bits_set(const bloom_filter* bf_p);

void sprint_bloom_filter_bitmap(dstring* append_str, const bloom_filter* bf_p, unsigned int tabs);

void deinitialize_bloom_filter(bloom_filter* bf_p);

#endif