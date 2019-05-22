#ifndef TREE_H
#define TREE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// tree manages its own data
// if tree takes a pointer from you, it will be to copy your data

typedef struct tree tree;
struct tree
{
	// size of data, on each node that each node of the tree holds
	unsigned long long int size_of_data_on_node;

	// total number of children buckets/capacity at initialization
	unsigned long long int children_default_size;

	// this is the factor, by which the size of children of any node increases
	unsigned long long int increment_factor;

	// this is the increment amount, by which the size of node children increases, over the factor
	unsigned long long int increment_offset;

	// this is the root node of the tree
	node* root_node;

	// for any node
	// new_children_bucket_size = old_children_buycket_size * increment_factor + increment_offset
};

typedef struct node node;
struct node
{
	// pointer to the parent node
	// parent of root node is NULL
	node* parent;

	// this is pointer to the data that the node will hold
	void* data_p;

	// can be zero
	unsigned long long int children_occupied_size;

	// total number of buckets
	unsigned long long int children_total_size;

	// children nodes
	node** children;
};

// builds a tree with no nodes
tree* get_tree(unsigned long long int children_default_size, unsigned long long int size_of_data_element_on_node);

// gives a node with data, copied to itself from data_p
node* get_node(tree* tree_p, const void* data_p);

// adds a child to parent node, as per the description of the characteristics of tree_p
void* add_child(tree* tree_p ,node* parent_p);

// builds a bond between parent and a child
void connect(tree* tree_p, node* parent_p, node* child_p);

// deletes all of the tree
void delete_tree(tree* tree_p);

// deletes all its children aswell
// recursive
void delete_node(node* node_p);

// tells us if the node is a leaf node
int is_leaf(node* node_p);

// tells us if the node is a root node
int is_root(node* node_p);

#endif