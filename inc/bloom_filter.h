#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include<array.h>

#include<probablistic_data_structure_util.h>

typedef struct bloom_filter bloom_filter;
struct bloom_filter
{
	// array consisting of all hash functions
	array data_hash_functions;

	// each data_hash_function has bucket_count number of buckets
	// total number of buckets = number of hash functions * bucket_count
	unsigned int bucket_count;

	// This is a bitmap consisting of (total bucket count) number of bits
	// this bitmap is a single chunk of memory
	// if the bitmap was managed as array by the c language,
	// then it would be accessible as bitmap[ ith_hash_function ][ jth_bit ]
	// but since c language does not allow dynamic multidimentional bit arrays
	// here we will use the functionality defined in bitmap.h and multi_dim_array_util.h (and related source (.c) file)
	char* bitmap;
};

void initialize_bloom_filter(bloom_filter* bf_p, unsigned int bucket_count, unsigned int hash_functions_count, data_hash_func* data_hash_functions);

void insert_in_bloom_filter(bloom_filter* bf_p, const void* data, unsigned int data_size);

typedef enum bloom_filter_presence bloom_filter_presence;
enum bloom_filter_presence
{
	ABSENT = 0,
	MAY_BE_PRESENT = 1,
};

// a result of exists in bloom_filter is 
// a definite not present (i.e. not present with 100% surety)
// or it may be present (it may be present or absent)
bloom_filter_presence exists_in_bloom_filter(const bloom_filter* bf_p, const void* data, unsigned int data_size);

void deinitialize_bloom_filter(bloom_filter* bf_p);

#endif