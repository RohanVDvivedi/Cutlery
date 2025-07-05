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
	int is_pinned(const void* pinning_context, const void* data);

	// actual map, storing all elements
	hashmap map;

	// lru linkedlist for tracking the least recently used element and evicting the un-pinned elements
	linkedlist lru;
};

#endif