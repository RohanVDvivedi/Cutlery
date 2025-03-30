#include<cutlery/bst_core.h>
#include<cutlery/bst_nonbalancing.h>
#include<cutlery/bst_util.h>

#include<cutlery/cutlery_stds.h>

void insert_node_in_non_self_balancing_tree(bst* bst_p, bstnode* node_p)
{
	insert_node_in_bst(bst_p, node_p);

	// the new node now exists in bst, so set its node_property to 1
	node_p->node_property = 1;
}

void remove_node_from_non_self_balancing_tree(bst* bst_p, bstnode* node_p)
{
	remove_node_from_bst(bst_p, node_p);

	// the node now does not exists in bst, so set its node_property to 0
	node_p->node_property = 0;
}