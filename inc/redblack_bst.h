#ifndef REDBLACKBST_H
#define REDBLACKBST_H

#include<balancedbst.h>

#define node bstnode

// use generic api only as mentioned in balancedbst.h

void handle_imbalance_in_red_black_tree(balancedbst* balancedbst_p, node* node_p);

void insert_node_in_red_black_tree(balancedbst* balancedbst_p, node* root, node* node_p);

void handle_black_height_imbalance(balancedbst* balancedbst_p, node* double_black_node, node* double_blacks_parent);

node* remove_node_from_red_black_tree(balancedbst* balancedbst_p, node* node_p);

#undef node

#endif