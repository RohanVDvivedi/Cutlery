#ifndef PHEAP_H
#define PHEAP_H

#include<cutlery/heap_info.h>
#include<cutlery/dstring.h>
#include<cutlery/notifier_interface.h>

/*
** 	pheap is short for pointer based binary heap, it is an intrusive data structure, like bst, supporting policies like SHEW heap and LEFTIST heap for balancing
*/

typedef enum pheaptype pheaptype;
enum pheaptype
{
	SKEW,
	LEFTIST,
};

typedef struct phpnode phpnode;
struct phpnode
{
	// the pointer to the parent node
	phpnode* parent;

	// it is a binary tree, with 2 children
	// both it's children are comparably <= the node itself for MIN_HEAP, and >= for MAX_HEAP
	phpnode* left;
	phpnode* right;

	// helps in keeing the tree balanced depends on the pheaptype for its value
	// pheaptype == SKEW, node_property = 1, always
	// pheaptype == LEFTIST, node_property = minimum number of nodes to reach the closest NULL child, includes itself in the path, i.e. for NULL node it is 0, and for a leaf node it is 1
	cy_uint node_property;
};

typedef struct pheap pheap;
struct pheap
{
	// stores whether, type = MIN_HEAP or MAX_HEAP AND the comparator to be used
	heap_info info;

	// the type of merge algorithm to use for the heap, possible values SKEW and LEFTIST
	pheaptype type;

	// defines address to data with respect to phpnode
	// this is how we reach node addresses from provided user's structure data addresses and viceversa
	cy_uint node_offset;

	// the root node of the tree
	phpnode* root;
};

/*
*	note : you can only insert a new node, and remove an existing node
*/

// initializes as if a new pheap, may be to reuse
void initialize_pheap(pheap* pheap_p, heap_type type, pheaptype policy, const comparator_interface* comparator, cy_uint node_offset);

// always initialize your phpnode before using it
void initialize_phpnode(phpnode* node_p);

// a free floating phpnode is the one, that is not referenced in any pheap, i.e a node that can be inserted to any pheap
int is_free_floating_phpnode(const phpnode* node_p);

int is_empty_pheap(const pheap* pheap_p);

// merged pheaps to make the dest pheap contain all elements of dest and src, as of prior to this call
// after this call dest contains all the elements and src is empty
// this call fails if the pheaps are not of same type (SKEW or LEFTIST) and (MIN_HEAP or MAX_HEAP)
int merge_pheaps(pheap* dest, pheap* src);

// insert a new element in pheap
// returns 0, and fails if phpnode of data is not a new node
int push_to_pheap(pheap* pheap_p, const void* data);

// pop the top element from pheap
// returns 0 only if the pheap_p is empty
int pop_from_pheap(pheap* pheap_p);

// returns the top of the pheap
const void* get_top_of_pheap(const pheap* pheap_p);

// if you modify the value of data, such that the heap property is violated then this functions fixes it's position in the tree, pushing it up or down the tree
// i.e. if you changed the attributes of the data, which changed its comparison ourput with respect to other elements in the heap, then you need to call this function immediately
// fails with 0, only if you provided data that is a free_floating node
int heapify_for_in_pheap(pheap* pheap_p, const void* data);

// removes a specific element from the heap
// returns 0, and fails if phpnode of data is a new node
int remove_from_pheap(pheap* pheap_p, const void* data);

// removes all the elements from the pheap and reinitializes their embedded phpnode
// after this if a notifier_interface if specified (i.e. not NULL), then it is notified
void remove_all_from_pheap(pheap* pheap_p, const notifier_interface* ni_p);

// perform given operation on all the elements of the pheap
void for_each_in_pheap(const pheap* pheap_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params);

// print complete pheap
void sprint_pheap(dstring* append_str, const pheap* pheap_p, void (*sprint_element)(dstring* append_str, const void* data, unsigned int tabs), unsigned int tabs);

#endif