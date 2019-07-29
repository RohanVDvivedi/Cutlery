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

// delete a bucket pointed to by bucket_p
void delete_bucket(bucket* bucket_p);

#endif