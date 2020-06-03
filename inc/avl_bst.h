#ifndef AVL_BST_H
#define AVL_BST_H

#include<balancedbst.h>

// use generic api only as mentioned in balancedbst.h

void handle_imbalance_in_avl_tree(balancedbst* balancedbst_p, bstnode* input_node_p);

void insert_node_in_avl_tree(balancedbst* balancedbst_p, bstnode* root, bstnode* node_p);

bstnode* remove_node_from_avl_tree(balancedbst* balancedbst_p, bstnode* node_p);

#endif