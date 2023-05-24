#ifndef COUNT_MIN_SKETCH_H
#define COUNT_MIN_SKETCH_H

#include<array.h>

#include<probabilistic_data_structure_util.h>

/*
** Count Min Sketch is a probablistic data structure
** It intends to provide you with an approximate number of times an object was seen by you
** higher the total number of buckets (number of hash functions * bucket_count) higher is its accuracy
** (with ofcourse better hash_functions with distributed and uniform over the range)
**
** you must call "increment_frequency_in_count_min_sketch" to increment frequency for an item that you are seeing at an instance
** while you can call "get_frequency_from_count_min_sketch" any time to check frequency of any of the data element
**
** you may call "reset_frequencies_in_count_min_sketch" to reset all the frequencies to 0
** this function can be used timely to avoid overflows inside the count_min_sketch
**
** This implementation of count min sketch only increments the count of the bucket_number-s (hash_value % bucket_count) that have the minimum count
**
** additionally a bucket with a value of UINT_MAX is not incremented, to avoid overflow
*/

typedef struct count_min_sketch count_min_sketch;
struct count_min_sketch
{
	// array consisting of all hash functions
	array data_hash_functions;

	// each data_hash_function has bucket_count number of buckets
	// total number of buckets = number of hash functions * bucket_count
	cy_uint bucket_count;

	// This is a 2-dimensional array consisting of (total bucket count) number of cy_uint-s
	// if this 2d dynamic-dimensional array was managed as array by the c language,
	// then it would be accessible as frequencies[ ith_hash_function ][ jth_bit ]
	// but since c language does not allow dynamic-sized multidimentional arrays
	// here we will use the functionality defined in multi_dim_array_util.h to access each of its elements
	cy_uint* frequencies;

	// uint_allocator is used for the above attribute frequencies,
	memory_allocator uint_allocator;
};

#define MAX_COUNT_MIN_SKETCH_TOTAL_BUCKET_COUNT (CY_UINT_MAX / sizeof(cy_uint))

// initialize_count_min_sketch* functions may fail, if the initial memory allocation fails
// in case of a failure, there has not been any memory allocation, and the error is fatal, the count min sketch instance is unusable

int initialize_count_min_sketch(count_min_sketch* cms_p, cy_uint bucket_count, cy_uint hash_functions_count, const data_hash_func data_hash_functions[]);

int initialize_count_min_sketch_with_allocator(count_min_sketch* cms_p, cy_uint bucket_count, cy_uint hash_functions_count, const data_hash_func data_hash_functions[], memory_allocator data_hash_functions_array_allocator, memory_allocator uint_allocator);

// in the initialize with_memory function the frequencies parameter (the last parameter) is optional
// it can be provided as NULL, if you want to use STD_C_mem_allocator for allocating and maintaining frequencies
int initialize_count_min_sketch_with_memory(count_min_sketch* cms_p, cy_uint bucket_count, cy_uint hash_functions_count, const data_hash_func data_hash_functions[], cy_uint frequencies[]);

// the below functions increments the frequency, and also returns the new frequency
cy_uint increment_frequency_in_count_min_sketch(count_min_sketch* cms_p, const void* data, cy_uint data_size);

cy_uint get_frequency_from_count_min_sketch(const count_min_sketch* cms_p, const void* data, cy_uint data_size);

void reset_frequencies_in_count_min_sketch(count_min_sketch* cms_p);

void sprint_count_min_sketch(dstring* append_str, const count_min_sketch* cms_p, unsigned int tabs);

void deinitialize_count_min_sketch(count_min_sketch* cms_p);

#endif