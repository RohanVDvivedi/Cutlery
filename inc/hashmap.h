#ifndef HASHMAP_H
#define HASHMAP_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<array.h>
#include<linkedlist.h>
#include<balancedbst.h>
#include<bucket.h>

// hashmap only manages (create and free) the buckets
// you are responsible for managing key and value pointers, passed to hashmap
// collisions are resolved by managing buckets as a linked list
// hashmap will not clone/touch/modify/free the memory pointed to by the pointer passed for key and value referencing
// and definately not create any new look alikes of key,value,
// hashmap will just store them directly in bucket and use the memory pointed by them when required

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
	// no collision is handled, if there is colision is happening, key and value of that hash index are replaced
	// true hashtable, truely O(1)
	NO_POLICY = 0,

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
	array* buckets_holder;
};

// build and get hashmap with a fixed bucket count,
// bucket count remains the same unless rehash is called with a new size
hashmap* get_hashmap(unsigned long long int bucket_count, unsigned long long int (*hash_function)(const void* key), int (*key_compare)(const void* key1, const void* key2), collision_resolution_policy hashmap_policy);

// place a new bucket in the hashmap, with key as key and value as value
void insert_entry(hashmap* hashmap_p, const void* key, const void* value);

// get the value from the hashmap, stored at a particular key
const void* find_value(const hashmap* hashmap_p, const void* key);

// find a bucket in hashmap, whose key is key_p, and update it to hold value_p, returns 1 if operation completed successfully
int update_value(hashmap* hashmap_p, const void* key_p, const void* value_p, const void** return_value);

// returns 1 if the bucket is found and removed from hashmap and deleted
int delete_entry(hashmap* hashmap_p, const void* key, const void** return_key, const void** return_value);

// the following function rehashes the hashmap pointed by hashmap_p, to a new size (probably larger)
// used to expand hashmap once the load factor is greater than 0.7 
void rehash_to_size(hashmap* hashmap_p, unsigned long long int new_bucket_size);

// print complete hashmap
void print_hashmap(const hashmap* hashmap_p, void (*print_key)(const void* key), void (*print_value)(const void* value));

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