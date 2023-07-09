#ifndef COUNT_MIN_SKETCH_H
#define COUNT_MIN_SKETCH_H

#include<array.h>

#include<probabilistic_data_structure_util.h>

/*
** Count Min Sketch is a probablistic data structure
** It intends to provide you with an approximate number of times an object was seen by you
** higher the total number of buckets (number of hash functions * bucket_count per hash function) higher is its accuracy
** (with ofcourse better hash_functions with distributed and uniform over the range)
**
** you must call "increment_frequency_in_count_min_sketch" to increment frequency for an item that you are seeing at an instance
** while you can call "get_frequency_from_count_min_sketch" any time to check frequency of any of the data element
**
** you may call "reset_frequencies_in_count_min_sketch" to reset all the frequencies to 0
**
** This implementation of count min sketch only increments the count of the bucket_number-s (hash_value % bucket_count) that have the minimum count
** And no increment is performed if the count overflows cy_uint
*/

typedef struct count_min_sketch count_min_sketch;
struct count_min_sketch
{
	// array consisting of all hash functions
	array data_hash_functions;

	// capacity_in_bytes is the number of bytes pointed to by frequencies array
	// each data_hash_function has ((capacity_in_bytes / sizeof(cy_uint)) / get_capacity_array(data_hash_functions)) number of buckets
	// total number of buckets = get_capacity_array(data_hash_functions) * ((capacity_in_bytes / sizeof(cy_uint)) / get_capacity_array(data_hash_functions))
	cy_uint capacity_in_bytes;

	// This is a 2-dimensional array consisting of (total bucket count) number of cy_uint-s
	// if this 2d dynamic-dimensional array was managed as array by the c language,
	// then it would be accessible as frequencies[ ith_hash_function ][ jth ]
	// here we will use the functionality defined in multi_dim_array_util.h to access each of its elements
	cy_uint* frequencies;

	// frequencies_allocator is used for allocating memory for the above attribute frequencies array
	memory_allocator frequencies_allocator;
};

#define MAX_COUNT_MIN_SKETCH_TOTAL_BUCKET_COUNT (CY_UINT_MAX / sizeof(cy_uint))

// initialize_count_min_sketch* functions may fail, if the initial memory allocation fails
// in case of a failure, there has not been any memory allocation, and the error is fatal, the count min sketch instance is unusable

int initialize_count_min_sketch(count_min_sketch* cms_p, cy_uint bucket_count, cy_uint hash_functions_count, const data_hash_func data_hash_functions[]);

int initialize_count_min_sketch_with_allocator(count_min_sketch* cms_p, cy_uint bucket_count, cy_uint hash_functions_count, const data_hash_func data_hash_functions[], memory_allocator data_hash_functions_array_allocator, memory_allocator frequencies_allocator);

// in the initialize with_memory function the frequencies parameter (the last parameter) is optional
// it can be provided as NULL, if you want to use STD_C_mem_allocator for allocating and maintaining frequencies
int initialize_count_min_sketch_with_memory(count_min_sketch* cms_p, cy_uint bucket_count, cy_uint hash_functions_count, const data_hash_func data_hash_functions[], cy_uint frequencies[]);

// this is the number of hash functions that would be used by the count min sketch
// equivalent to get_capacity_array(&(cms_p->data_hash_functions));
cy_uint get_hash_function_count_count_min_sketch(const count_min_sketch* cms_p);

// this is the number of buckets per hash function in the count min sketch
// equivalent to (cms_p->capacity_in_bytes / sizeof(cy_uint)) / get_hash_function_count_bloom_filter(bf_p)
cy_uint get_bucket_count_per_hash_function_count_min_sketch(const count_min_sketch* cms_p);

// the below functions increments the frequency, and also returns the new frequency
cy_uint increment_frequency_in_count_min_sketch(count_min_sketch* cms_p, const void* data, cy_uint data_size);

cy_uint get_frequency_from_count_min_sketch(const count_min_sketch* cms_p, const void* data, cy_uint data_size);

void reset_frequencies_in_count_min_sketch(count_min_sketch* cms_p);

void sprint_count_min_sketch(dstring* append_str, const count_min_sketch* cms_p, unsigned int tabs);

void deinitialize_count_min_sketch(count_min_sketch* cms_p);

#endif