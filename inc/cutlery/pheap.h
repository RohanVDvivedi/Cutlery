#ifndef PHEAP_H
#define PHEAP_H

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
	// the type of merge algorithm to use for the heap
	pheaptype type;

	// stores whether, type = MIN_HEAP or MAX_HEAP AND the comparator to be used
	heap_info info;

	// defines address to data with respect to bstnode
	// this is how we reach node addresses from provided user's structure data addresses and viceversa
	cy_uint node_offset;

	// the root node of the tree
	phpnode* root;
};

#endif