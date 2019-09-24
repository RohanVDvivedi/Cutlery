#ifndef HEAP_H
#define HEAP_H

#include<stdio.h>
#include<stdlib.h>

#include<array.h>

typedef enum heap_type heap_type;
enum heap_type
{
	MIN_HEAP,
	MAX_HEAP
};

typedef struct heap heap;
struct heap
{
	// the type of heap, a heap can be a min heap or a max heap
	// as a MIN_HEAP or MAX_HEAP
	heap_type type;

	// compare keys and returns 0 if they are same, else non-zero
	// it returns 0 if they are same, >0 if key0 is greater than key1 else it must return <0 value
	int (*key_compare)(const void* key1, const void* key2);

	// number of buckets in the heap
	unsigned long long int heap_size;

	// array of buckets in heap
	array* heap_holder;
};

// build and get a new heap, with expected size, and of the given heap type (either MIN_HEAP or MAX_HEAP)
void* get_heap(unsigned long long int expected_size, heap_type type, int (*key_compare)(const void* key0, const void* key1));

// push a new bucket to the heap
void push(heap* heap_p, void* key, void* value);

// returns pointer to the value of the bucket, whose key is either minimum for MIN_HEAP (or maxinum if it is a MAX_HEAP)
const void* get_top(heap* heap_p);

// pop the top bucket from the heap
void pop(heap* heap_p);

// delete heap and heap_holder array, along with all its buckets
void delete_heap(heap* heap_p);

#endif