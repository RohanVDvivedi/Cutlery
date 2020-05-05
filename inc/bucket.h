#ifndef BUCKET_H
#define BUCKET_H

#include<stdio.h>
#include<stdlib.h>

typedef struct bucket bucket;
struct bucket
{
	// the key of the bucket
	const void* key;

	// value of the bucket
	const void* value;
};

// get a new bucket
const bucket* get_bucket(const void* key, const void* value);

// print the bucket
void print_bucket(const bucket* bucket_p, void (*print_key)(const void* key), void (*print_value)(const void* value));

// print bucket wrapper functions
void print_bucket_wrapper_for_array(void* bucket_p_to_print, unsigned long long int index, const void* bucket_p_functions);

// delete a bucket pointed to by bucket_p
void delete_bucket(bucket* bucket_p);

// delete bucket wrapper functions
void delete_bucket_wrapper_for_array(void* data_p, unsigned long long int index, const void* additional_params);

// compare bucket function
// since a bucket is unique by its key
// we compare buckets, by their keys
int bucket_compare(const bucket* bucket_p1, const bucket* bucket_p2, int (*key_compare)(const void* key_1, const void* key_2));

#endif