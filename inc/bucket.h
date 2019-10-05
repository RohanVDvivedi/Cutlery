#ifndef BUCKET_H
#define BUCKET_H

#include<stdio.h>
#include<stdlib.h>

// this is bucketted datastructures specific enum
// for the put_entry operation
typedef enum put_type put_type;
enum put_type
{
	PUT_IF_EXISTS 		= 0x01,	// update = 01 decimal
	PUT_IF_NOT_EXISTS 	= 0x10	// insert = 16 decimal
};

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

// delete a bucket pointed to by bucket_p
void delete_bucket(bucket* bucket_p);

// compare bucket function
// since a bucket is unique by its key
// we compare buckets, by their keys
int bucket_compare(const bucket* bucket_p1, const bucket* bucket_p2, int (*key_compare)(const void* key_1, const void* key_2));

#endif