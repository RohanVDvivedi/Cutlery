#ifndef HASHMAP_H
#define HASHMAP_H

#include<array.h>

typedef enum collision_resolution_policy collision_resolution_policy;
enum collision_resolution_policy
{
	// each element of the hashmap, is itself a bucket
	// collision is handled using Robinhood hashing,
	// worst case search is O(n)
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
// hence hashmap does not gaurantee uniqueness for your data on its own
// two data* (pointers) pointing to data that compare equals on the given compartor may exist in the hashmap

// While designing the hashfunction please consider using all the attributes, that you are using in the comparator function
// the comparator function and the hashfunction must be based on using the same fields

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
	array hashmap_holder;

	// element_count represents the number of elements in the hashmap
	unsigned int element_count;
};

// initializes hashmap and it will depend on initialize_array to give necessary memory to manage internal element contents
void initialize_hashmap(hashmap* hashmap_p, collision_resolution_policy hashmap_policy, unsigned int bucket_count, unsigned int (*hash_function)(const void* key), int (*compare)(const void* data1, const void* data2), unsigned int node_offset);

// place a new data in the hashmap, fails with return 0, if the element already exists in the hashmap
// or fails with 0, if the hashmap does not have enough space to hold the new data element
int insert_in_hashmap(hashmap* hashmap_p, const void* data);

/*
**		NOTE FOR USING THE FIND FUNCTION BELOW
**
** find_* function; returns the data from the hashmap, that equals the data given as parameter "data",
** (when compared using the comparator function)
**
** the "data" you provide as parameter must have all the fields required for comparison 
** used in the given comparator function (as in "hashmap_p" struct).
** i.e. it must have all the fields/attributes of the struct that are used in "hashmap_p->compare(,)".
*/
// the parameter data provided must hash to the same value and must be comparatively equal to the data that you want to find
const void* find_equals_in_hashmap(const hashmap* hashmap_p, const void* data);

// returns 1 if the element exists in the hashmap and is removed
// fails with 0, if the data provided does not exist in this hashmap
int remove_from_hashmap(hashmap* hashmap_p, const void* data);

// returns bucket_count of the hashmap;;; i.e. total_size of the hashmap_holder
unsigned int get_bucket_count_hashmap(const hashmap* hashmap_p);
// returns the number of elements inside the hashmap
unsigned int get_element_count_hashmap(const hashmap* hashmap_p);
// returns 1 if the hashmap is empty (i.e. element_count == 0), else returns 0
int is_empty_hashmap(const hashmap* hashmap_p);

// incresase the number of buckets in the hashmap
// this is O(n) operation, since it involves rehashing all the elements of the hashmap
// On failure it returns 0, when new_bucket_count < old_element_count (for ROBINHOOD_HASHING) || new_bucket_count == 0
// else it returns 1 on successfully resizing
int resize_hashmap(hashmap* hashmap_p, unsigned int new_bucket_count);
// below function works similarly to resize_hashmap
// it fails with return 0, when expand_factor < 1.0
int expand_hashmap(hashmap* hashmap_p, float expand_factor);

// frees all the data being held by the hashmap
// the same hashmap can be reused by calling initialize_hashmap function, after it is deinitialized
void deinitialize_hashmap(hashmap* hashmap_p);

// print complete hashmap
void print_hashmap(const hashmap* hashmap_p, void (*print_element)(const void* data));

// perform operation on all the elements of the hashmap
// the function is designed well, you may call free on your data, in the provided operation function
// if BST we use POST_ORDER traversal and for linkedlist we cache the next pointer,
// and hence we are able to iterate over the elements properly even if the current element gets freed
void for_each_in_hashmap(const hashmap* hashmap_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params);

#endif