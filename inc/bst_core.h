#ifndef BST_CORE_H
#define BST_CORE_H

#include<bst.h>

bstnode* get_smallest_node_from_node(bstnode* node_p);

bstnode* get_largest_node_from_node(bstnode* node_p);

void insert_node_in_bst(bst* bst_p, bstnode* node_p);

void remove_node_from_bst(bst* bst_p, bstnode* node_p);

#endif