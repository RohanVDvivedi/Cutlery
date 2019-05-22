#ifndef TREE_H
#define TREE_H

#include<array.h>

typedef struct tree tree;
struct tree
{
	// size of data, on each node that each node of the tree holds
	unsigned long long int size_of_data_on_node;

	// total number of buckets, can not be 0, at least 1
	unsigned long long int children_default_size;

	// this is the factor, by which the size of node children increases
	unsigned long long int increment_factor;

	// this is the increment amount, by which the size of node children increases
	unsigned long long int increment_offset;

	// this is the root node of the tree
	node* root_node;

	// for any node
	// new_children_bucket_size = old_children_buycket_size * increment_factor + increment_offset
};

typedef struct node node;
struct node
{
	// this is pointer to the data that the node will hold
	void* data_p;

	// can be zero
	unsigned long long int children_size;

	// total number of buckets
	unsigned long long int children_max_size;

	// children nodes
	node** children;
};

tree* get_tree(unsigned long long int children_default_size, unsigned long long int size_of_data_element_on_node);

int is_leaf(node* node_p);


#endif