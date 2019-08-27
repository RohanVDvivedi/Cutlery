#include<balancedbst.h>

balancedbst* get_balancedbst(tree_type balanced_tree_type, int (*key_compare)(const void* key0, const void* key1))
{
	balancedbst* balancedbst_p = (balancedbst*) calloc(1, sizeof(balancedbst));
	balancedbst_p->balanced_tree_type = balanced_tree_type;
	balancedbst_p->root = NULL;
	balancedbst_p->key_compare = key_compare;
	return balancedbst_p;
}

node* get_node(const void* key, const void* value)
{
	node* node_p = (node*) calloc(1, sizeof(node));
	node_p->bucket_p = get_bucket(key, value);
	return node_p;
}

/* functions to get node types -- start */

int is_leaf_node(node* node_p)
{
	// if the node does not have a left child and it does not have a right child, it is a leaf node
	return node_p->left_sub_tree == NULL && node_p->right_sub_tree == NULL;
}

int is_root_node(node* node_p)
{
	// if the node does not have a parent node, it is root node of the tree
	return node_p->parent == NULL;
}

int is_internal_node(node* node_p)
{
	// a node which is not leaf node or a root node is an internal node
	return !(is_leaf_node(node_p) || is_root_node(node_p));
}

// returns true if the node_p is the left node of its parent
int is_left_of_its_parent(node* node_p)
{
	return (!(is_root_node(node_p))) && (node_p->parent->left_sub_tree == node_p);
}

// returns true if the node_p is the right node of its parent
int is_right_of_its_parent(node* node_p)
{
	return (!(is_root_node(node_p))) && (node_p->parent->right_sub_tree == node_p);
}

/* functions to get node types -- end   */

/* utility functions for tree -- start */

int is_balancedbst_empty(balancedbst* blanacedbst_p)
{
	return blanacedbst_p->root == NULL;
}

/* utility functions for tree -- end   */

node* find_node_recursively(balancedbst* balancedbst_p, node* root, const void* key_p)
{
	if(balancedbst_p->key_compare(key_p, root->bucket_p->key) == 0)
	{
		return root;
	}
	else if(root->left_sub_tree != NULL && balancedbst_p->key_compare(key_p, root->bucket_p->key) < 0)
	{
		return find_node_recursively(balancedbst_p, root->left_sub_tree, key_p);
	}
	else if(root->right_sub_tree != NULL && balancedbst_p->key_compare(key_p, root->bucket_p->key) > 0)
	{
		return find_node_recursively(balancedbst_p, root->right_sub_tree, key_p);
	}
	else
	{
		return NULL;
	}
}

node* find_node(balancedbst* balancedbst_p, const void* key_p)
{
	// if the tree is empty, just return NULL
	if( is_balancedbst_empty(balancedbst_p) )
	{
		return NULL;
	}
	else
	{
		return find_node_recursively(balancedbst_p, balancedbst_p->root, key_p);
	}
}

// neither root nor node_p params are suppossed to be NULL in the function below
void insert_node_in_non_self_balancing_tree(balancedbst* balancedbst_p, node* root, node* node_p)
{
	if( balancedbst_p->key_compare(node_p->bucket_p->key, root->bucket_p->key) <= 0 )
	{
		if( root->left_sub_tree == NULL )
		{
			root->left_sub_tree = node_p;
			node_p->parent = root;
		}
		else
		{
			insert_node_in_non_self_balancing_tree(balancedbst_p, root->left_sub_tree, node_p);
		}
	}
	else if( balancedbst_p->key_compare(node_p->bucket_p->key, root->bucket_p->key) > 0 )
	{
		if( root->right_sub_tree == NULL )
		{
			root->right_sub_tree = node_p;
			node_p->parent = root;
		}
		else
		{
			insert_node_in_non_self_balancing_tree(balancedbst_p, root->right_sub_tree, node_p);
		}
	}
}

// neither root nor node_p params are suppossed to be NULL in the function below
void insert_node_in_red_black_tree(balancedbst* balancedbst_p, node* root, node* node_p)
{

}

// neither root nor node_p params are suppossed to be NULL in the function below
void insert_node_in_avl_tree(balancedbst* balancedbst_p, node* root, node* node_p)
{

}

// node_p param is not suppossed to be NULL in the function below
void insert_node_in_tree(balancedbst* balancedbst_p, node* node_p)
{
	// if the root of the tree is NULL, i.e. the tree is empty, add a new root to the tree
	if( is_balancedbst_empty(balancedbst_p) )
	{
		balancedbst_p->root = node_p;
		node_p->parent = NULL;
		return;
	}

	// else insert node as per the balancing that this tree uses
	switch( balancedbst_p->balanced_tree_type )
	{
		case NON_SELF_BALANCING:
		{
			insert_node_in_non_self_balancing_tree(balancedbst_p, balancedbst_p->root, node_p);
			break;
		}
		case RED_BLACK_TREE :
		{
			insert_node_in_red_black_tree(balancedbst_p, balancedbst_p->root, node_p);
			break;
		}
		case AVL_TREE :
		{
			insert_node_in_avl_tree(balancedbst_p, balancedbst_p->root, node_p);
			break;
		}
	}
}

void put_entry(balancedbst* balancedbst_p, const void* key_p, const void* value_p)
{
	// find a node in the tree that has key_p as its key (we do not want duplicate keys)
	node* node_p = find_node(balancedbst_p, key_p);

	// if we can not find such a node, create a new node and insert it in tree
	if(node_p == NULL)
	{
		node_p = get_node(key_p, value_p);
		insert_node_in_tree(balancedbst_p, node_p);
	}
	// else update the value for the node found
	else
	{
		bucket* bucket_p = ((bucket*)node_p->bucket_p);
		bucket_p->value = value_p;
	}
}

const void* find_value(balancedbst* blancedbst_p, const void* key_p)
{
	node* node_p = find_node(blancedbst_p, key_p);
	return node_p != NULL ? node_p->bucket_p->value : NULL;
}

int remove_node_from_non_self_balancing_tree(balancedbst* balancedbst_p, node* node_p)
{
	node* left_tree = node_p->left_sub_tree;
	node* right_tree = node_p->right_sub_tree;
	node* parent_node = node_p->parent;

	// do not update any references of node_p/we are anyway deleting it

	// remove references of the node_p from its children
	left_tree->parent = NULL;
	right_tree->parent = NULL;
	// if the node_p has parent we need to update the corresponding child reference of its parent to NULL
	// if the node_p is the left node of its parent
	if( is_left_of_its_parent(node_p) )
	{
		parent_node->left_sub_tree = NULL;
	}
	// if the node_p is the right node of its parent
	else if( is_right_of_its_parent(node_p) )
	{
		parent_node->right_sub_tree = NULL;
	}
	
	// if node_p does not have parents, then node_p is the root,
	// and hence root of the tree has to be updated
	if( is_root_node(node_p) )
	{
		balancedbst_p->root = left_tree;
		if(right_tree != NULL)
		{
			insert_node_in_tree(balancedbst_p, right_tree);
		}
	}
	else
	{
		// insert the left tree of the node_p back into the tree
		if(left_tree != NULL)
		{
			insert_node_in_non_self_balancing_tree(balancedbst_p, parent_node, left_tree);
		}
		// insert the right tree of the node_p back into the tree
		if(right_tree != NULL)
		{
			insert_node_in_non_self_balancing_tree(balancedbst_p, parent_node, right_tree);
		}
	}

	return 1;
}

int remove_node_from_red_black_tree(balancedbst* balancedbst_p, node* node_p)
{
	return 0;
}

int remove_node_from_avl_tree(balancedbst* balancedbst_p, node* node_p)
{
	return 0;
}

void delete_node(node* node_p)
{
	delete_bucket(((bucket*)node_p->bucket_p));
	free(node_p);
}

int remove_value(balancedbst* balancedbst_p, const void* key_p)
{
	int deleted_nodes_count = 0;
	node* node_p = find_node(balancedbst_p, key_p);
	if(node_p != NULL)
	{
		switch(balancedbst_p->balanced_tree_type)
		{
			case NON_SELF_BALANCING:
			{
				deleted_nodes_count = remove_node_from_non_self_balancing_tree(balancedbst_p, node_p);
				break;
			}
			case RED_BLACK_TREE :
			{
				deleted_nodes_count = remove_node_from_red_black_tree(balancedbst_p, node_p);
				break;
			}
			case AVL_TREE :
			{
				deleted_nodes_count = remove_node_from_avl_tree(balancedbst_p, node_p);
				break;
			}
		}
		delete_node(node_p);
	}
	return deleted_nodes_count;
}

void print_node(node* node_p, void (*print_key)(const void* key), void (*print_value)(const void* value))
{
	if(node_p != NULL)
	{
		if( is_leaf_node(node_p) )
		{
			printf("\tLEAF NODE     :");
		}
		else if( is_root_node(node_p) )
		{
			printf("\tROOT NODE     :");
		}
		else if( is_internal_node(node_p) )
		{
			printf("\tINTERNAL NODE :");
		}

		printf("\taddress => %d", ((int)node_p));
		printf("\tdata => ");
		print_bucket(node_p->bucket_p, print_key, print_value);

		if( !is_root_node(node_p) )
		{
			if(is_left_of_its_parent(node_p))
			{
				printf("\t\tis LEFT of  : \n");
			}
			else if(is_right_of_its_parent(node_p))
			{
				printf("\t\tis RIGHT of : \n");
			}
			printf("\t\t\taddress => %d", ((int)node_p->parent));
			printf("\tdata => ");
			print_bucket(node_p->parent->bucket_p, print_key, print_value);
		}

		if( (!is_leaf_node(node_p)) )
		{
			if(node_p->left_sub_tree != NULL)
			{
				printf("\t\thas a LEFT\n\t\t\tchild  => %d", ((int)node_p->left_sub_tree));
				printf("\tdata => ");
				print_bucket(node_p->left_sub_tree->bucket_p, print_key, print_value);
			}
			if(node_p->right_sub_tree != NULL)
			{
				printf("\t\thas a RIGHT\n\t\t\tchild => %d", ((int)node_p->right_sub_tree)); 
				printf("\tdata => ");
				print_bucket(node_p->right_sub_tree->bucket_p, print_key, print_value);
			}
		}
		printf("\n");
	}
}

void print_tree(node* node_p, void (*print_key)(const void* key), void (*print_value)(const void* value))
{
	if(node_p != NULL)
	{
		print_tree(node_p->left_sub_tree, print_key, print_value);
	}
	print_node(node_p, print_key, print_value);
	printf("\n");
	if(node_p != NULL)
	{
		print_tree(node_p->right_sub_tree, print_key, print_value);
	}
}

void print_balancedbst(const balancedbst* balancedbst_p, void (*print_key)(const void* key), void (*print_value)(const void* value))
{
	printf("TREE : ");
	switch(balancedbst_p->balanced_tree_type)
	{
		case NON_SELF_BALANCING :
		{
			printf("NON_SELF_BALANCING\n");
			break;
		}
		case AVL_TREE :
		{
			printf("AVL_TREE\n");
			break;
		}
		case RED_BLACK_TREE :
		{
			printf("RED_BLACK_TREE\n");
			break;
		}
	}
	print_tree(balancedbst_p->root, print_key, print_value);
	printf("--\n\n");
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