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

typedef struct bst bst;
struct bst;

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

struct bst
{
	// the type of balancing algorithm to use for the binary search tree
	bsttype type;

	// compares data
	// it returns 0 if they are same, >0 if data1 is greater than data2 else it must return <0 value
	int (*compare)(const void* data1, const void* data2);

	// defines address to data with respect to bstnode
	// this is how we reach node addresses from provided user's structure data addresses and viceversa
	unsigned int node_offset;

	// the root node of the tree
	bstnode* root;
};

/*
*	note : you can only insert a new node, and remove an existing node
*/

// initializes as if a new bst, may be to reuse
void initialize_bst(bst* bst_p, bsttype type, unsigned int node_offset, int (*compare)(const void* data1, const void* data2));

// always initialize your bstnode before using it
void initialize_bstnode(bstnode* node_p);

// inserts given data in the tree
// returns 0, and fails if bstnode of data is not a new node
int insert_in_bst(bst* bst_p, const void* data);

/*
**		NOTE FOR USING ALL THE FIND FUNCTIONS BELOW
**
** find_* functions get the data from the bst, that equals (when compared using the comparator function) the data given as parameter data
**
** the data you provide as parameter must have all the fields required for comparison, 
** this completely depends on how you design your comparator function, and what some few or all fields you use to do comparison, it is up to you
** i.e. it must have all the fields/attrubutes of the struct that are used in comparison function that you provided
*/

// find data in tree, whose data compares equal to data provided
const void* find_equals_in_bst(const bst* bst_p, const void* data);

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

// This function is tricky to explain, it returns true 2 types of nodes
// 1. a node that does not exist in any bst
// 2. a node that is a only single element of any other bst (other than the bst_p provided)
// a true of this function signifies that this node can be inserted into bst_p without any bad consequences
// a true signifies that this node_p does not exist in any other bst having more than one element
int is_new_bstnode(bst* bst_p, bstnode* node_p);

// traversals possible in the tree
typedef enum bsttraversal bsttraversal;
enum bsttraversal {PRE_ORDER, IN_ORDER, POST_ORDER};

// perform given operation on all the elements of the binary search tree
void for_each_in_bst(const bst* bst_p, bsttraversal traversal, void (*operation)(const void* data, const void* additional_params), const void* additional_params);

// print complete binary search tree
void print_bst(const bst* bst_p, void (*print_element)(const void* data));

// PRINT NODE REFERENCES TO DEBUG YOUR APP
void print_bstnode_debug(bstnode* node_p);

#endif