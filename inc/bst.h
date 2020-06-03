#ifndef BST_H
#define BST_H

/*
** 	BST is short for Binary Search Tree
*/

#include<stdio.h>

typedef enum bsttype bsttype;
enum bsttype
{
	NON_SELF_BALANCING,
	AVL_TREE,
	RED_BLACK_TREE
};

typedef struct bstnode bstnode;
struct bstnode
{
	// the pointer to the parent node
	bstnode* parent;

	// all < nodes					// all >= nodes
	bstnode* left;					bstnode* right;

	// the property of the node that will be used to help balance the tree
	// if bsttype == NON_SELF_BALANCING then node property is ignored
	// else if bsttype == AVL_TREE then node_property = max( right_tree->node_property, left_tree->node_property)
	// else if bsttype == RED_BLACK_TREE then node_property = is_red_node? ? 0 : 1; (1 is black node) 
	// it must be a signed integer, do not change this datatype
	int node_property;
};

typedef struct bst bst;
struct bst
{
	// the type of balancing algorithm to use for the binary search tree
	bsttype type;

	// the root node of the tree
	bstnode* root;

	// this is number of nodes in the tree
	unsigned long long int node_count;

	// defines address to data with respect to bstnode
	unsigned long long int node_offset;

	// compares data
	// it returns 0 if they are same, >0 if data1 is greater than data2 else it must return <0 value
	int (*compare)(const void* data1, const void* data2);
};

/*
*	note : you can only insert a new node, and remove an existing node
*/

// initializes as if a new bst, may be to reuse
void initialize_bst(bst* bst_p, bsttype type, unsigned long long int node_offset, int (*compare)(const void* data1, const void* data2));

// inserts given data in the tree
// returns 0, and fails if bstnode of data is not a new node
int insert_in_bst(bst* bst_p, const void* data);

// find data in tree, whose data compares equal to data provided
const void* find_in_bst(const bst* bst_p, const void* data);

// find data in tree, with the biggest value which is smaller than or equal to data
// returns NULL, if data is smaller than the smallest data
const void* find_preceding_or_equals(const bst* bst_p, const void* data);

// find data in tree, with the smallest value which is bigger than or equal to data
// returns NULL, if data is bigger than the biggest data value in tree
const void* find_succeeding_or_equals(const bst* bst_p, const void* data);

// find data in tree, with the smallest value
const void* find_smallest(const bst* bst_p);

// find data in tree, with the biggest value
const void* find_largest(const bst* bst_p);

// remove data from the given tree
// returns 0, and fails if bstnode of data is a new node
int remove_from_bst(bst* bst_p, const void* data);

// perform given operation on all the elements of the binary search tree
void for_each_in_bst(const bst* bst_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params);

// print complete binary search tree
void print_balancedbst(const bst* bst_p, void (*print_element)(const void* data));

#endif