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

	// each element of the hashmap is a linkedlist of buckets
	// each insert is done at the head of each of this linkedlist
	// worst case search is O(n)
	ELEMENTS_AS_LINKEDLIST_INSERT_AT_HEAD = 1,

	// each element of the hashmap is a linkedlist of buckets
	// each insert is done at the head of each of this linkedlist
	// worst case search is O(n)
	ELEMENTS_AS_LINKEDLIST_INSERT_AT_TAIL = 2,

	// each element of the hashmap is a red black binary search tree of buckets
	// worst case search is O(log(n))
	ELEMENTS_AS_RED_BLACK_BST = 3,

	// each element of the hashmap is a avl binary search tree of buckets
	// worst case search is O(log(n))
	ELEMENTS_AS_AVL_BST = 4
};

// inserting comparatively equal data is allowed
// inserting same data (same address) is not allowed
// hence hashmap does not gaurantee uniqueness for your data on its own
// two data* (pointers) pointing to data that compare equals on the given compartor may exist in the hashmap

// while designing the hash function please consider using all or some the attributes, that you are using in the comparator function
// the comparator function and the hashfunction must be based on using the same fields/attributes
// i.e. attributes of your data used in calculation of your hash_function must be a subset of the attributes you use in your compare function
// this ensures that all the data that compare equals reside/collide in the same bucket of the hashmap

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
	// this count is only valid for ROBINHOOD_HASHING OR if the actual element_count is lesser than UINT_MAX (which must be the case)
	unsigned int element_count;
};

// for collision_resolution_policy = ROBINHOOD_HASHING, use rbhnode as an embedded node
typedef struct rbhnode rbhnode;
struct rbhnode
{
	// position index is the actual index in the hashmap_holder of this element
	// while bucket_index is the index that this element hashes to
	unsigned int position_index;
};

// initializes hashmap and it will depend on initialize_array to give necessary memory to manage internal element contents
void initialize_hashmap(hashmap* hashmap_p, collision_resolution_policy hashmap_policy, unsigned int bucket_count, unsigned int (*hash_function)(const void* key), int (*compare)(const void* data1, const void* data2), unsigned int node_offset);
void initialize_hashmap_with_allocator(hashmap* hashmap_p, collision_resolution_policy hashmap_policy, unsigned int bucket_count, unsigned int (*hash_function)(const void* key), int (*compare)(const void* data1, const void* data2), unsigned int node_offset, memory_allocator mem_allocator);

// always initialize your rbhnode before using it
void initialize_rbhnode(rbhnode* node_p);

// a free floating rbhnode is the one, that is not referenced in any hashmap, i.e a node that can be inserted to any hashmap
int is_free_floating_rbhnode(const rbhnode* node_p);

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

#define FIRST_OF_HASHMAP INVALID_INDEX

// returns pointer to the first element of the hashmap in bucket at bucket_index
// if bucket_index parameter == FIRST_OF_HASHMAP, then the first element in iteration of hashmap is returned
const void* get_first_of_in_hashmap(const hashmap* hashmap_p, unsigned int bucket_index);

typedef enum hashmap_next_type hashmap_next_type;
enum hashmap_next_type
{
	ANY_IN_HASHMAP,
	ANY_IN_SAME_BUCKET,
	ANY_THAT_EQUALS
};

// this below function can be used to get the next element that occurs after data_xist in hashmap
// the next_type variable dictates what element will be returned it can be ANY_IN_HASHMAP, ANY_IN_SAME_BUCKET or ANY_THAT_EQUALS
const void* get_next_of_in_hashmap(const hashmap* hashmap_p, const void* data_xist, hashmap_next_type next_type);

// returns bucket_count of the hashmap
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
// it fails with return 0, when expand_factor <= 1.0
int expand_hashmap(hashmap* hashmap_p, float expand_factor);

// removes all the elements from the hashmap hashmap_p
// after a call to this function the hashmap contains 0 elements (get_element_count() == 0)
// it does not affect the bucket_count of the hashmap
void remove_all_from_hashmap(hashmap* hashmap_p);

// frees all the data being held by the hashmap
// the same hashmap can be reused by calling initialize_hashmap function, after it is deinitialized
void deinitialize_hashmap(hashmap* hashmap_p);

// serializes the hashmap, and appends the serialized form to the dstring
void sprint_hashmap(dstring* append_str, const hashmap* hashmap_p, void (*sprint_element)(dstring* append_str, const void* data, unsigned int tabs), unsigned int tabs);

// perform operation on all the elements of the hashmap
// the function is designed well, you may call free on your data, in the provided operation function
// if BST we use POST_ORDER traversal and for linkedlist we cache the next pointer,
// and hence we are able to iterate over the elements properly even if the current element gets freed
void for_each_in_hashmap(const hashmap* hashmap_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params);

#endif