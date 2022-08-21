#ifndef BST_CORE_H
#define BST_CORE_H

#include<bst.h>

bstnode* get_smallest_node_from_node(bstnode* node_p);

bstnode* get_largest_node_from_node(bstnode* node_p);

// inserts a node in the bst
// the node must be a new_node -> passing is_new_bstnode as true
// node_property of all the nodes remain untouched
void insert_node_in_bst(bst* bst_p, bstnode* node_p);

// the node must exist in the same bst
// the node that gets removed will still hold its parent, left and right pointers and its node property
// the removed node node must be reinitialized after this call using -> initialize_bstnode function
// node_property of all the nodes remain untouched (including the removed node)
void remove_node_from_bst(bst* bst_p, bstnode* node_p);

#endif