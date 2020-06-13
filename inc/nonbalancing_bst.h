#ifndef NONBALANCING_BST_H
#define NONBALANCING_BST_H

#include<bst.h>

bstnode* get_smallest_node_from_node(bstnode* node_p);

bstnode* get_largest_node_from_node(bstnode* node_p);

void insert_node_in_non_self_balancing_tree(bst* bst_p, bstnode* node_p);

void remove_node_from_non_self_balancing_tree(bst* bst_p, bstnode* node_p);

#endif