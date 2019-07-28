#ifndef BALANCEDBST_H
#define BALANCEDBST_H

#include<stdio.h>
#include<stdlib.h>

// the tree manages all of its data on its own
// you manage your own data
// all the nodes on the left sub tree of the node are smaller than or equal to the node itself,
// when compare using the data_compare function
// all the nodes on the right sub tree of the node are greater than the node itself,
// when compare using the data_compare function

typedef enum tree_type tree_type;
enum tree_type
{
	AVL_TREE,
	RED_BLACK_TREE
};

typedef struct node node;
struct node
{
	// the pointer to the parent node
	node* parent;

	// the data to store
	const void* data_p;

	// the property of the node that will be used to help balance the tree
	unsigned long long int node_property;

	// all <= nodes									// all > nodes
	node* left_sub_tree;							node* right_sub_tree;
};

typedef struct balancedbst balancedbst;
struct balancedbst
{
	// the type of balancing algorithm to use
	tree_type balanced_tree_type;

	// compare data pointers that we stored
	// it returns 0 if they are same, >0 if data0 is greater than data1 else it must return <0 value
	int (*data_compare)(const void* data0_p, const void* data1_p);

	// the root node of the tree
	node* root;
};

// get a new balancedbst
balancedbst* get_balancedbst(tree_type balanced_tree_type, int (*data_compare)(const void* data0_p, const void* data1_p));

// inserts a node in the tree whose data is data_p
void insert(balancedbst* blanacedbst_p, const void* data_p);

// find a data_p in tree
const void* find(balancedbst* blanacedbst_p, const void* data_p);

// remove the node if found, returns a list of nodes that match
void remove_node(balancedbst* blanacedbst_p, const void* data_p);

// delete the balancedbst and all its nodes
void delete_balancedbst(balancedbst* balancedbst_p);

#endif