#include<balancedbst.h>

balancedbst* get_balancedbst(tree_type balanced_tree_type, int (*data_compare)(const void* data0_p, const void* data1_p))
{
	balancedbst* balancedbst_p = (balancedbst*) calloc(1, sizeof(balancedbst));
	balancedbst_p->balanced_tree_type = balanced_tree_type;
	balancedbst_p->root = NULL;
	balancedbst_p->data_compare = data_compare;
	return balancedbst_p;
}

void insert_in_red_black_tree(balancedbst* blanacedbst_p, const void* data_p)
{

}

void insert_in_avl_tree(balancedbst* blanacedbst_p, const void* data_p)
{

}

void insert(balancedbst* blanacedbst_p, const void* data_p)
{
	switch(blanacedbst_p->balanced_tree_type)
	{
		case RED_BLACK_TREE :
		{
			insert_in_red_black_tree(blanacedbst_p, data_p);
			break;
		}
		case AVL_TREE :
		{
			insert_in_avl_tree(blanacedbst_p, data_p);
			break;
		}
	}
}

const void* find_node(balancedbst* blanacedbst_p, const node* root, const void* data_p)
{
	if(blanacedbst_p->data_compare(data_p, root->data_p) == 0)
	{
		return root->data_p;
	}
	else if(root->left_sub_tree != NULL && blanacedbst_p->data_compare(data_p, root->data_p) < 0)
	{
		return find_node(blanacedbst_p, root->left_sub_tree, data_p);
	}
	else if(root->right_sub_tree != NULL && blanacedbst_p->data_compare(data_p, root->data_p) > 0)
	{
		return find_node(blanacedbst_p, root->right_sub_tree, data_p);
	}
	else
	{
		return NULL;
	}
}

const void* find(balancedbst* blanacedbst_p, const void* data_p)
{
	if(blanacedbst_p->root == NULL)
	{
		return NULL;
	}
	else
	{
		return find_node(blanacedbst_p, blanacedbst_p->root, data_p);
	}
}

void remove_from_red_black_tree(balancedbst* blanacedbst_p, const void* data_p)
{

}

void remove_from_avl_tree(balancedbst* blanacedbst_p, const void* data_p)
{

}

void remove_node(balancedbst* blanacedbst_p, const void* data_p)
{
	switch(blanacedbst_p->balanced_tree_type)
	{
		case RED_BLACK_TREE :
		{
			remove_from_red_black_tree(blanacedbst_p, data_p);
			break;
		}
		case AVL_TREE :
		{
			remove_from_avl_tree(blanacedbst_p, data_p);
			break;
		}
	}
}

void delete_node(node* node_p)
{
	free(node_p);
}

void delete_nodes_from(node* node_p)
{
	if(node_p->left_sub_tree != NULL)
	{
		delete_nodes_from(node_p->left_sub_tree);
	}
	if(node_p->right_sub_tree != NULL)
	{
		delete_nodes_from(node_p->right_sub_tree);
	}
	delete_node(node_p);
}

void delete_balancedbst(balancedbst* balancedbst_p)
{
	delete_nodes_from(balancedbst_p->root);
	free(balancedbst_p);
}