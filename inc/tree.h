#ifndef TREE_H
#define TREE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// each tree branches on nodes
// tree maintains its own nodes
// you manage your data
// tree will manage its own data (the node structure)
// tree will not manage your data (the data pointed to by data_p in set_child)
// tree will not clone/touch/modify/free the memory pointed to by the pointer passed for referencing your data

typedef struct node node;
struct node
{
	// pointer to the parent node
	// parent of root node is NULL
	node* parent;

	// this is pointer to the data that the node will hold
	const void* data_p;

	// children nodes
	node** children;
};

// tree manages its own data
// if tree takes a pointer from you, it will be to copy your data

typedef struct tree tree;
struct tree
{
	// total number of children buckets/capacity of any node must not be 0, if 1 this is doubly linked list
	unsigned long long int children_default_size;

	// this is the root node of the tree
	node* root_node;
};

// builds a tree with no nodes
tree* get_tree(unsigned long long int children_default_size, const void* root_data_p);

// adds a child to parent node, as per the description of the characteristics of tree_p at index child_index of the parent node of the tree
// if this fails we return -1
int set_child(const tree* tree_p, node* parent_p, const void* data_p, unsigned long long int child_index);

// gets a pointer to the already existing child node
const node* get_child(const tree* tree_p, node* parent_p, unsigned long long int child_index);

// gets a pointer to the data pointer of already existing child node
const void* get_data(const tree* tree_p, node* parent_p, unsigned long long int child_index);

// builds a bond between parent node and a child node, placing the child at index child_index of the parent node of the tree
void connect(const tree* tree_p, node* parent_p, node* child_p, unsigned long long int child_index);

// deletes the child node, frees its node and data memory and deltes all of its children aswell
void remove_child(const tree* tree_p, node* parent, unsigned long long int child_index);

// deletes all of the tree
void delete_tree(tree* tree_p);

// print tree
void print_tree(const tree* tree_p, void (*print_data)(const void* data_p));

#endif