#ifndef REDBLACKBST_H
#define REDBLACKBST_H

#include<balancedbst.h>

void handle_imbalance_in_red_black_tree(balancedbst* balancedbst_p, bstnode* node_p);

void insert_node_in_red_black_tree(balancedbst* balancedbst_p, bstnode* root, bstnode* node_p);

void handle_black_height_imbalance(balancedbst* balancedbst_p, bstnode* double_black_node, bstnode* double_blacks_parent);

bstnode* remove_node_from_red_black_tree(balancedbst* balancedbst_p, bstnode* node_p);

#endif