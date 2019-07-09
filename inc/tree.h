#ifndef TREE_H
#define TREE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// each tree branches on nodes
// tree maintains its own nodes
// you manage your data, tree will manage  its own data

typedef struct node node;
struct node
{
	// pointer to the parent node
	// parent of root node is NULL
	node* parent;

	// this is pointer to the data that the node will hold
	void* data_p;

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

	// total number of children buckets/capacity of any node must not be 0, if 1 this is doubly linked list
	unsigned long long int children_default_size;

	// this is the root node of the tree
	node* root_node;
};

// builds a tree with no nodes
tree* get_tree(unsigned long long int children_default_size, unsigned long long int size_of_data_element_on_node, const void* root_data_p);

// gets you a new node for the specified tree
node* get_node(const tree* tree_p, const void* data_p);

// adds a child to parent node, as per the description of the characteristics of tree_p at index child_index of the parent node of the tree
void add_child(const tree* tree_p, node* parent_p, const void* data_p, unsigned long long int child_index);

// builds a bond between parent node and a child node, placing the child at index child_index of the parent node of the tree
void connect(const tree* tree_p, node* parent_p, node* child_p, unsigned long long int child_index);

// deletes all of the tree
void delete_tree(tree* tree_p);

// deletes only the node pointed to by node_p
void delete_node(node* node_p);

// deletes the node and all its children aswell recursively
void delete_nodes_from(const tree* tree_p, node* node_p);

// print tree
void print_tree(const tree* tree_p, void (*print_data)(const void* node_p));

// prints only the node pointed to by node_p
void print_node(const node* node_p, void (*print_data)(const void* node_p), unsigned long long int tabs_count);

// prints node and all of its children aswell recursively
void print_nodes_from(const tree* tree_p, const node* node_p, void (*print_data)(const void* node_p), unsigned long long int tabs_count);

#endif