#ifndef NONBALANCING_BST_H
#define NONBALANCING_BST_H

#include<balancedbst.h>

#define node bstnode

node* get_smallest_node_from_node(const node* node_p);

node* get_largest_node_from_node(const node* node_p);

// neither root nor node_p params are suppossed to be NULL in the function below
void insert_node_in_non_self_balancing_tree(balancedbst* balancedbst_p, node* root, node* node_p);

// the below function only detaches the node that has to be deleted
// returns pointer of the node that has to be deleted
// node_p can not be null in the parameters of the function
node* remove_node_from_non_self_balancing_tree(balancedbst* balancedbst_p, node* node_p);

#undef node

#endif