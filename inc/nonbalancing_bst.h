#ifndef NONBALANCING_BST_H
#define NONBALANCING_BST_H

#include<balancedbst.h>

bstnode* get_smallest_node_from_node(const bstnode* node_p);

bstnode* get_largest_node_from_node(const bstnode* node_p);

void insert_node_in_non_self_balancing_tree(balancedbst* balancedbst_p, bstnode* root, bstnode* node_p);

bstnode* remove_node_from_non_self_balancing_tree(balancedbst* balancedbst_p, bstnode* node_p);

#endif