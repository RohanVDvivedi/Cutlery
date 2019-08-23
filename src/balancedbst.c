#include<balancedbst.h>

balancedbst* get_balancedbst(tree_type balanced_tree_type, int (*data_compare)(const void* key0, const void* key1))
{
	balancedbst* balancedbst_p = (balancedbst*) calloc(1, sizeof(balancedbst));
	balancedbst_p->balanced_tree_type = balanced_tree_type;
	balancedbst_p->root = NULL;
	balancedbst_p->data_compare = data_compare;
	return balancedbst_p;
}

node* get_node(const void* key, const void* value)
{
	node* node_p = (node*) calloc(1, sizeof(node));
	node_p->bucket_p = get_bucket(const void* key, const void* value);
	return node_p;
}

const node* find_node(balancedbst* blanacedbst_p, const node* root, const void* key_p)
{
	if(blanacedbst_p->data_compare(key_p, root->bucket_p->key) == 0)
	{
		return root;
	}
	else if(root->left_sub_tree != NULL && blanacedbst_p->data_compare(key_p, root->bucket_p->key) < 0)
	{
		return find_node(blanacedbst_p, root->left_sub_tree, data_p);
	}
	else if(root->right_sub_tree != NULL && blanacedbst_p->data_compare(key_p, root->bucket_p->key) > 0)
	{
		return find_node(blanacedbst_p, root->right_sub_tree, data_p);
	}
	else
	{
		return NULL;
	}
}

void insert_node_in_non_self_balancing_tree(balancedbst* blanacedbst_p, const node* node_p)
{

}

void insert_node_in_red_black_tree(balancedbst* blanacedbst_p, const node* node_p)
{

}

void insert_node_in_avl_tree(balancedbst* blanacedbst_p, const node* node_p)
{

}

void put_entry(balancedbst* blanacedbst_p, const void* key_p, const void* value_p)
{
	node* node_p = find_node(blanacedbst_p, balancedbst_p->root, key_p);
	if(node_p == NULL)
	{
		node_p = get_node(const void* key, const void* value);
		switch(blanacedbst_p->balanced_tree_type)
		{
			case NON_SELF_BALANCING:
			{
				insert_node_in_non_self_balancing_tree(blanacedbst_p, node_p);
				break;
			}
			case RED_BLACK_TREE :
			{
				insert_node_in_red_black_tree(blanacedbst_p, node_p);
				break;
			}
			case AVL_TREE :
			{
				insert_node_in_avl_tree(blanacedbst_p, node_p);
				break;
			}
		}
	}
	else
	{
		node_p->bucket_p->value = value_p;
	}
}

const void* find_value(balancedbst* blanacedbst_p, const void* data_p)
{
	if(blanacedbst_p->root == NULL)
	{
		return NULL;
	}
	else
	{
		node* node_p = find_node(blanacedbst_p, blanacedbst_p->root, data_p);
		return node_p != NULL ? node_p->bucket_p->value : NULL;
	}
}

void remove_node_from_non_self_balancing_tree(balancedbst* blanacedbst_p, const node* node_p)
{

}

void remove_node_from_red_black_tree(balancedbst* blanacedbst_p, const node* node_p)
{

}

void remove_node_from_avl_tree(balancedbst* blanacedbst_p, const node* node_p)
{

}

int remove_value(balancedbst* blanacedbst_p, const void* key_p)
{
	int deleted_nodes_count = 0;
	node* node_p = find_node(blanacedbst_p, balancedbst_p->root, key_p);
	if(node_p != NULL)
	{
		switch(blanacedbst_p->balanced_tree_type)
		{
			case NON_SELF_BALANCING:
			{
				deleted_nodes_count = remove_node_from_non_self_balancing_tree(blanacedbst_p, node_p);
				break;
			}
			case RED_BLACK_TREE :
			{
				deleted_nodes_count = remove_node_from_red_black_tree(blanacedbst_p, node_p);
				break;
			}
			case AVL_TREE :
			{
				deleted_nodes_count = remove_node_from_avl_tree(blanacedbst_p, node_p);
				break;
			}
		}
	}
	return deleted_nodes_count;
}

void delete_node(node* node_p)
{
	delete_bucket(node_p->bucket_p);
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