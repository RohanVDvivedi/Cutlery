#ifndef HASHMAP_H
#define HASHMAP_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<array.h>
#include<bucket_array.h>

#include<linkedlist.h>
#include<balancedbst.h>

// to avoid name collision with functions of balancedbst
#define insert_entry	insert_entry_in_hash
#define find_value		find_value_from_hash
#define update_value	update_value_in_hash
#define delete_entry 	delete_entry_from_hash
#define for_each_entry 	for_each_entry_in_hash

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

typedef struct hashmap hashmap;
struct hashmap
{
	// attribute defines how the collision is handled in the hashmap
	collision_resolution_policy hashmap_policy;

	// hash function ( <3 my love )
	unsigned long long int (*hash_function)(const void* key);

	// compare keys and returns 0 if they are same, else non-zero
	// it returns 0 if they are same, >0 if key1 is greater than key2 else it must return <0 value
	int (*key_compare)(const void* key1, const void* key2);

	// pinter to the array of buckets
	array buckets_holder;

	// this is the number of buckets, which are occupied in the hashmap
	unsigned long long int bucket_occupancy;

	// this is the number of total buckets in the hashmap
	unsigned long long int bucket_count;
};

// build and get hashmap with a fixed bucket count,
// bucket count remains the same unless rehash is called with a new size
hashmap* get_hashmap(unsigned long long int bucket_count, unsigned long long int (*hash_function)(const void* key), int (*key_compare)(const void* key1, const void* key2), collision_resolution_policy hashmap_policy);

// initializes hashmap and it will depend on initialize_array to give necessary memory to manage internal element contents
void initialize_hashmap(hashmap* hashmap_p, unsigned long long int bucket_count, unsigned long long int (*hash_function)(const void* key), int (*key_compare)(const void* key1, const void* key2), collision_resolution_policy hashmap_policy);

// place a new bucket in the hashmap, with key as key and value as value
void insert_entry(hashmap* hashmap_p, const void* key, const void* value);

// get the value from the hashmap, stored at a particular key
const void* find_value(const hashmap* hashmap_p, const void* key);

// find a bucket in hashmap, whose key is key_p, and update it to hold value_p, returns 1 if operation completed successfully
int update_value(hashmap* hashmap_p, const void* key_p, const void* value_p, const void** return_value);

// returns 1 if the bucket is found and removed from hashmap and deleted
int delete_entry(hashmap* hashmap_p, const void* key, const void** return_key, const void** return_value);

// print complete hashmap
void print_hashmap(const hashmap* hashmap_p, void (*print_key)(const void* key), void (*print_value)(const void* value));

// frees all the data being held by the hashmap, this function, does not release memory of the actual hashmap structure, only the memory of the components
// the same hashmap can be reused by calling initialize_hashmap function, after it is deinitialized
void deinitialize_hashmap(hashmap* hashmap_p);

// deletes all the data allocated by the hashmap and the hashmap itself
void delete_hashmap(hashmap* hashmap_p);

// perform operation on all the elements of the hashmap
void for_each_entry(const hashmap* hashmap_p, void (*operation)(const void* key, const void* value, const void* additional_params), const void* additional_params);

#undef insert_entry
#undef find_value
#undef update_value
#undef delete_entry
#undef for_each_entry

#endif