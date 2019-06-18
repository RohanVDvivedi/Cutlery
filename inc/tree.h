#ifndef TREE_H
#define TREE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// each tree branches on nodes

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
	unsigned long long int increment_amount;

	// total number of nodes on the tree 
	unsigned long long int node_count;

	// height of tree, number of nodes between root_node and farthest node, including both nodes
	// helps one know deviation 
	unsigned long long int height_of_tree;

	// this is the root node of the tree
	node* root_node;

	// for any node
	// new_children_bucket_size = old_children_buycket_size * increment_factor + increment_offset
};

// builds a tree with no nodes
tree* get_tree(unsigned long long int children_default_size, unsigned long long int size_of_data_element_on_node);

// gets you a new node for the specified tree
node* get_node(tree* tree_p, const void* data_p);

// adds a child to parent node, as per the description of the characteristics of tree_p
void add_child(tree* tree_p, node* parent_p, const void* data_p);

// builds a bond between parent and a child
void connect(tree* tree_p, node* parent_p, node* child_p);

// deletes all of the tree
void delete_tree(tree* tree_p);

// deletes the node and all its children aswell recursively
void delete_nodes_from(node* node_p);

// print tree
void print_node(node* node_p);

// print tree
void print_tree(tree* tree_p, void (*print_data)(const void* node_p));

#endif