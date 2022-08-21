#include<bst_core.h>
#include<bst_nonbalancing.h>
#include<bst_util.h>

#include<cutlery_stds.h>

void insert_node_in_non_self_balancing_tree(bst* bst_p, bstnode* node_p)
{
	insert_node_in_bst(bst_p, node_p);
}

void remove_node_from_non_self_balancing_tree(bst* bst_p, bstnode* node_p)
{
	remove_node_from_bst(bst_p, node_p);
}