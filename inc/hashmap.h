#ifndef HASHMAP_H
#define HASHMAP_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<linkedlist.h>
#include<bst.h>

typedef enum collision_resolution_policy collision_resolution_policy;
enum collision_resolution_policy
{
	// each element of the hashmap, is itself a bucket
	// collision is handled using Robinhood hashing,
	// true hashtable, truely O(1)
	ROBINHOOD_HASHING = 0,

	// each element if the hashmap is a linkedlist of buckets
	// worst case search is O(n)
	ELEMENTS_AS_LINKEDLIST = 1,

	// each element of the hashmap is a red black binary search tree of buckets
	// worst case search is O(log(n))
	ELEMENTS_AS_RED_BLACK_BST = 10,

	// each element of the hashmap is a avl binary search tree of buckets
	// worst case search is O(log(n))
	ELEMENTS_AS_AVL_BST = 11
};

// inserting comparatively equal data is allowed
// inserting same data (same address) is not allowed

typedef struct hashmap hashmap;
struct hashmap
{
	// attribute defines how the collision is handled in the hashmap
	collision_resolution_policy hashmap_policy;

	// if it is using open addressing, this is the node_offset for instructing the linkedlist or binary search tree
	unsigned int node_offset;

	// hash function to hash the data
	unsigned int (*hash_function)(const void* data);

	// compare data and returns 0 if they are equal, else non-zero
	// it returns 0 if they are same, >0 if data1 is greater than data2 else it must return <0 value
	int (*compare)(const void* data1, const void* data2);

	// array storing pointer to all the of data in the hashmap
	void** holder;

	// this is the number of buckets, which are occupied in the hashmap
	unsigned int occupancy;

	// this is the number of total buckets in the hashmap
	unsigned int total_bucket_count;
};

// initializes hashmap and it will depend on initialize_array to give necessary memory to manage internal element contents
void initialize_hashmap(hashmap* hashmap_p, collision_resolution_policy hashmap_policy, unsigned int total_bucket_count, unsigned int (*hash_function)(const void* key), int (*compare)(const void* data1, const void* data2), unsigned int node_offset);

int exists_in_hashmap(hashmap* hashmap_p, const void* data);

// place a new data in the hashmap, fails with return 0, if the element already exists in the hashmap
// or fails with 0, if the hashmap does not have enough space to hold the new data element
int insert_in_hashmap(hashmap* hashmap_p, const void* data);

// get the data from the hashmap, that equals the data given to us
//
// the data you provide in the hashmap must have all the fields required to compare and hash equivalent to the actual data
// i.e. it must have all the fields/attrubutes of the struct that are used in comparison and hash_function that you provided
const void* find_equals_in_hashmap(const hashmap* hashmap_p, const void* data);

// returns 1 if the element exists in the hashmap and is removed
// fails with 0, if the data provided does not exist in this hashmap
int remove_from_hashmap(hashmap* hashmap_p, const void* data);

// print complete hashmap
void print_hashmap(const hashmap* hashmap_p, void (*print_element)(const void* data));

// frees all the data being held by the hashmap
// the same hashmap can be reused by calling initialize_hashmap function, after it is deinitialized
void deinitialize_hashmap(hashmap* hashmap_p);

// perform operation on all the elements of the hashmap
void for_each_in_hashmap(const hashmap* hashmap_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params);

#endif