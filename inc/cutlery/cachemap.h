#ifndef CACHEMAP_H
#define CACHEMAP_H

#include<cutlery/hashmap.h>
#include<cutlery/linkedlist.h>

/*
	This is an LRU cache, why not LFU, MRU or some custom priority based cache, you say
	MRU -> too specific use case, only works for multiple scan over scan of objects
	LFU -> saturating counters, O(log(n)) access due to use of heap, and inabilit to evict highly used objects from past
	priority based -> too much complexity, and no temporal locality used

	This is a very simple cachemap implementation, which is also a directive on how this library can be used to make more complex datastructures

	UNLIKE OTHER MAP LIKE CUTLERY DATA STRUCTURES INSERTS WILL FAIL ON DUPLICATE INSERTIONS
*/

typedef struct cchnde cchnode;
struct cchnode
{
	bstnode map_embed_node; // the map used is a ELEMENTS_AS_RED_BLACK_BST based hashmap
	llnode lru_embed_node; // for LRU linkedlist
};

typedef struct cachemap cachemap;
struct cachemap
{
	// callback to check if a element of the cachemap is pinned
	// a pinned element can not be selected for eviction
	const void* pinning_context;
	int (*is_pinned)(const void* pinning_context, const void* data);

	// actual map, storing all elements
	hashmap map;

	// lru linkedlist for tracking the least recently used element and evicting the un-pinned elements
	linkedlist lru;
};

// intialize cachemap
int initialize_cachemap(cachemap* cachemap_p, const void* pinning_context, int (*is_pinned)(const void* pinning_context, const void* data), cy_uint bucket_count, const hasher_interface* hasher, const comparator_interface* comparator, cy_uint node_offset);
int initialize_cachemap_with_allocator(cachemap* cachemap_p, const void* pinning_context, int (*is_pinned)(const void* pinning_context, const void* data), cy_uint bucket_count, const hasher_interface* hasher, const comparator_interface* comparator, cy_uint node_offset, memory_allocator mem_allocator);
int initialize_cachemap_with_memory(cachemap* cachemap_p, const void* pinning_context, int (*is_pinned)(const void* pinning_context, const void* data), cy_uint bucket_count, const hasher_interface* hasher, const comparator_interface* comparator, cy_uint node_offset, const void* bucket_memory[]);

// always initialize your cchnode before using it
void initialize_cchnode(cchnode* node_p);

// a free floating rbhnode is the one, that is not referenced in any cachemap, i.e a node that can be inserted to any cachemap
int is_free_floating_cchnode(const cchnode* node_p);

// place a new data in the cachemap, fails with return 0, if the element already exists in the cachemap
// it will also fail insertion if an element that compares equal is present in the cache
int insert_in_cachemap(cachemap* cachemap_p, const void* data);

// the parameter data provided must hash to the same value and must be comparatively equal to the data that you want to find
// if you want this element of the cachemap, to stay in the cache longer and avoid being an eviction victim, bump it, after a non-NULL return from this function
const void* find_equals_in_cachemap(const cachemap* cachemap_p, const void* data);

// returns 1 if the element exists in the cachemap and is removed
// fails with 0, if the data provided does not exist in this cachemap
int remove_from_cachemap(cachemap* cachemap_p, const void* data);

// returns pointer to the least recently used unpinned element that can be evicted to possibly insert a new one
// make sure to remove it first, this is a read only function, it does not actually evict the element
const void* get_evictable_element_from_cachemap(const cachemap* cachemap_p);

// bump an element in the cachemap, it will position the element back in the lru to avoid it from being the least recently used element
// the insert in lru may mot occur if the element is pinned
// this function must also be called after un-pinning the element, to insert it back in the lru
int bump_element_in_cachemap(cachemap* cachemap_p, const void* data);

// returns bucket_count of the cachemap
cy_uint get_bucket_count_cachemap(const cachemap* cachemap_p);
// returns the number of elements inside the cachemap
cy_uint get_element_count_cachemap(const cachemap* cachemap_p);
// returns 1 if the cachemap is empty (i.e. element_count == 0), else returns 0
int is_empty_cachemap(const cachemap* cachemap_p);

// resize the bucket_count of the cache
int resize_cachemap(cachemap* cachemap_p, cy_uint new_bucket_count);

// removes all the elements from the cachemap and reinitializes their embedded nodes
void remove_all_from_cachemap(cachemap* cachemap_p, const notifier_interface* ni_p);

// frees all the data being held by the cachemap
// the same cachemap can be reused by calling initialize_cachemap function, after it is deinitialized
void deinitialize_cachemap(cachemap* cachemap_p);

// serializes the cachemap, and appends the serialized form to the dstring
void sprint_cachemap(dstring* append_str, const cachemap* cachemap_p, void (*sprint_element)(dstring* append_str, const void* data, unsigned int tabs), unsigned int tabs);

// for each for cachemap
void for_each_in_cachemap(const cachemap* cachemap_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params);

#endif