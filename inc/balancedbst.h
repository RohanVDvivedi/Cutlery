#ifndef BALANCEDBST_H
#define BALANCEDBST_H

#include<stdio.h>
#include<stdlib.h>
#include<bucket.h>

// the tree manages all of its data on its own
// you manage your own data
// all the nodes on the left sub tree of the node are smaller than or equal to the node itself,
// when compare using the data_compare function
// all the nodes on the right sub tree of the node are greater than the node itself,
// when compare using the data_compare function

typedef enum tree_type tree_type;
enum tree_type
{
	NON_SELF_BALANCING,
	AVL_TREE,
	RED_BLACK_TREE
};

typedef struct node node;
struct node
{
	// the pointer to the parent node
	node* parent;

	// the data to store
	const bucket* bucket_p;

	// the property of the node that will be used to help balance the tree
	// if balanced_tree_type == NON_SELF_BALANCING then node property is ignored
	// else if balanced_tree_type == AVL_TREE then node_property = left tree height - right tree height
	// else if balanced_tree_type == AVL_TREE then node_property = is_red_node? ? 0 : 1; (1 is black node) 
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
	// it returns 0 if they are same, >0 if key0 is greater than key1 else it must return <0 value
	int (*key_compare)(const void* key0, const void* key1);

	// the root node of the tree
	node* root;
};

// get a new balancedbst
balancedbst* get_balancedbst(tree_type balanced_tree_type, int (*key_compare)(const void* key0, const void* key1));

// inserts or updates a node in the tree whose data is a bucket with key as key_p and value as value_p
void put_entry(balancedbst* blanacedbst_p, const void* key_p, const void* value_p);

// find a value_p in tree, whose key is key_p
const void* find_value(balancedbst* blanacedbst_p, const void* key_p);

// remove the node if found, returns a list of nodes that match
int remove_value(balancedbst* blanacedbst_p, const void* key_p);

// print complete binary search tree
void print_balancedbst(const balancedbst* blanacedbst_p, void (*print_key)(const void* key), void (*print_value)(const void* value));

// delete the balancedbst and all its nodes
void delete_balancedbst(balancedbst* balancedbst_p);

#endif