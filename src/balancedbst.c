#include<balancedbst.h>

balancedbst* get_balancedbst(tree_type balanced_tree_type, int (*data_compare)(const void* data0_p, const void* data1_p))
{
	balancedbst* balancedbst_p = (balancedbst*) calloc(1, sizeof(balancedbst));
	balancedbst->balanced_tree_type = balanced_tree_type;
	balancedbst_p->root = NULL;
	balancedbst_p->data_compare = data_compare;
}

void insert(balancedbst* blanacedbst_p, const void* data_p)
{

}

const void* find(balancedbst* blanacedbst_p, const void* data_p)
{

}

void remove(balancedbst* blanacedbst_p, const void* data_p)
{

}

void delete_node(node* node_p)
{
	free(node_p);
}

void delete_nodes_from(node* node_p)
{
	if(node_p->child_nodes[0] != NULL)
	{
		delete_nodes_from(node_p->child_nodes[0]);
	}
	if(node_p->child_nodes[1] != NULL)
	{
		delete_nodes_from(node_p->child_nodes[1]);
	}
	delete_node(node_p);
}

void delete_balancedbst(balancedbst* balancedbst_p)
{

}