#ifndef AVL_BST_H
#define AVL_BST_H

#include<balancedbst.h>

#define node bstnode

// use generic api only as mentioned in balancedbst.h

void handle_imbalance_in_avl_tree(balancedbst* balancedbst_p, node* input_node_p);

void insert_node_in_avl_tree(balancedbst* balancedbst_p, node* root, node* node_p);

node* remove_node_from_avl_tree(balancedbst* balancedbst_p, node* node_p);

#undef node

#endif