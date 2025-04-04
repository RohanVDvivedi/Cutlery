#ifndef BST_ORDER_STAT_H
#define BST_ORDER_STAT_H

#include<cutlery/bst.h>

// below two functions take bstnode as input but actually work with order_stat_bstnode

// returns the sub_tree_size as present in the order_stat_bstnode of the node_p
cy_uint get_subtree_size(const bst* bst_p, const bstnode* node_p);

// below functions recomputes the sub_tree_size of all nodes from node_p to the most parent node that is wrong/incorrect
// it force recomputes the sub_tree_size at node_p
cy_uint recompute_subtree_size(const bst* bst_p, bstnode* node_p);

#endif