#include<bst.h>
#include<nonbalancing_bst.h>
#include<avl_bst.h>
#include<redblack_bst.h>

// to avoid name collision with node of linkedlist
#define delete_node delete_bstnode
#define find_node 	find_bstnode

// to avoid name collision with functions of hashmap
#define insert_entry	insert_entry_in_bst
#define find_value		find_value_from_bst 
#define update_value	update_value_in_bst
#define delete_entry 	delete_entry_from_bst
#define for_each_entry 	for_each_entry_in_bst

balancedbst* get_balancedbst(tree_type balanced_tree_type, int (*key_compare)(const void* key0, const void* key1))
{
	balancedbst* balancedbst_p = calloc(1, sizeof(balancedbst));
	initialize_balancedbst(balancedbst_p, balanced_tree_type, key_compare);
	return balancedbst_p;
}

void initialize_balancedbst(balancedbst* balancedbst_p, tree_type balanced_tree_type, int (*key_compare)(const void* key0, const void* key1))
{
	balancedbst_p->balanced_tree_type = balanced_tree_type;
	balancedbst_p->root = NULL;
	balancedbst_p->key_compare = key_compare;
	balancedbst_p->bucket_count = 0;
}

bstnode* get_node(const void* key, const void* value)
{
	bstnode* node_p = calloc(1, sizeof(bstnode));
	initialize_bucket(&(node_p->data_entry), key, value);
	return node_p;
}

static bstnode* find_node_recursively(const balancedbst* balancedbst_p, const bstnode* root, const void* key_p)
{
	if(balancedbst_p->key_compare(key_p, root->data_entry.key) == 0)
	{
		return ((bstnode*)root);
	}
	else if(root->left_sub_tree != NULL && balancedbst_p->key_compare(key_p, root->data_entry.key) < 0)
	{
		return find_node_recursively(balancedbst_p, root->left_sub_tree, key_p);
	}
	else if(root->right_sub_tree != NULL && balancedbst_p->key_compare(key_p, root->data_entry.key) > 0)
	{
		return find_node_recursively(balancedbst_p, root->right_sub_tree, key_p);
	}
	else
	{
		return NULL;
	}
}

static bstnode* find_node(const balancedbst* balancedbst_p, const void* key_p)
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

static bstnode* find_node_preceding_or_equals_recursively(const balancedbst* balancedbst_p, const bstnode* root, const void* key_p)
{
	if(balancedbst_p->key_compare(key_p, root->data_entry.key) == 0)
	{
		return ((bstnode*)root);
	}
	else if(root->left_sub_tree != NULL && balancedbst_p->key_compare(key_p, root->data_entry.key) < 0)
	{
		return find_node_preceding_or_equals_recursively(balancedbst_p, root->left_sub_tree, key_p);
	}
	else if(balancedbst_p->key_compare(key_p, root->data_entry.key) > 0)
	{
		bstnode* result = NULL;
		if(root->right_sub_tree != NULL)
		{
			result = find_node_preceding_or_equals_recursively(balancedbst_p, root->right_sub_tree, key_p);
		}
		return result == NULL ? ((bstnode*)root) : result;
	}
	else
	{
		return NULL;
	}
}

static bstnode* find_node_preceding_or_equals(const balancedbst* balancedbst_p, const void* key_p)
{
	// if the tree is empty, just return NULL
	if( is_balancedbst_empty(balancedbst_p) )
	{
		return NULL;
	}
	else
	{
		return find_node_preceding_or_equals_recursively(balancedbst_p, balancedbst_p->root, key_p);
	}
}

static bstnode* find_node_succeeding_or_equals_recursively(const balancedbst* balancedbst_p, const bstnode* root, const void* key_p)
{
	if(balancedbst_p->key_compare(key_p, root->data_entry.key) == 0)
	{
		return ((bstnode*)root);
	}
	else if(balancedbst_p->key_compare(key_p, root->data_entry.key) < 0)
	{
		bstnode* result = NULL;
		if(root->left_sub_tree != NULL)
		{
			result = find_node_succeeding_or_equals_recursively(balancedbst_p, root->left_sub_tree, key_p);
		}
		return result == NULL ? ((bstnode*)root) : result;
	}
	else if(root->right_sub_tree != NULL && balancedbst_p->key_compare(key_p, root->data_entry.key) > 0)
	{
		return find_node_succeeding_or_equals_recursively(balancedbst_p, root->right_sub_tree, key_p);
	}
	else
	{
		return NULL;
	}
}

static bstnode* find_node_succeeding_or_equals(const balancedbst* balancedbst_p, const void* key_p)
{
	// if the tree is empty, just return NULL
	if( is_balancedbst_empty(balancedbst_p) )
	{
		return NULL;
	}
	else
	{
		return find_node_succeeding_or_equals_recursively(balancedbst_p, balancedbst_p->root, key_p);
	}
}

static bstnode* find_node_with_smallest_key(const balancedbst* balancedbst_p)
{
	// if the tree is empty, just return NULL
	if( is_balancedbst_empty(balancedbst_p) )
	{
		return NULL;
	}
	else
	{
		return get_smallest_node_from_node(balancedbst_p->root);
	}
}

static bstnode* find_node_with_largest_key(const balancedbst* balancedbst_p)
{
	// if the tree is empty, just return NULL
	if( is_balancedbst_empty(balancedbst_p) )
	{
		return NULL;
	}
	else
	{
		return get_largest_node_from_node(balancedbst_p->root);
	}
}

const void* find_value(const balancedbst* balancedbst_p, const void* key_p)
{
	bstnode* node_p = find_node(balancedbst_p, key_p);
	return node_p != NULL ? node_p->data_entry.value : NULL;
}

const void* find_value_with_smallest_key(const balancedbst* balancedbst_p)
{
	bstnode* node_p = find_node_with_smallest_key(balancedbst_p);
	return node_p != NULL ? node_p->data_entry.value : NULL;
}

const void* find_value_with_largest_key(const balancedbst* balancedbst_p)
{
	bstnode* node_p = find_node_with_largest_key(balancedbst_p);
	return node_p != NULL ? node_p->data_entry.value : NULL;
}

const void* find_value_preceding_or_equals(const balancedbst* balancedbst_p, const void* key_p)
{
	bstnode* node_p = find_node_preceding_or_equals(balancedbst_p, key_p);
	return node_p != NULL ? node_p->data_entry.value : NULL;
}

const void* find_value_succeeding_or_equals(const balancedbst* balancedbst_p, const void* key_p)
{
	bstnode* node_p = find_node_succeeding_or_equals(balancedbst_p, key_p);
	return node_p != NULL ? node_p->data_entry.value : NULL;
}

// node_p param is not suppossed to be NULL in the function below
static void insert_node_in_tree(balancedbst* balancedbst_p, bstnode* node_p)
{
	// if the root of the tree is NULL, i.e. the tree is empty, add a new root to the tree
	if( is_balancedbst_empty(balancedbst_p) )
	{
		balancedbst_p->root = node_p;
		node_p->node_property = balancedbst_p->balanced_tree_type == NON_SELF_BALANCING ? 0 : 1; // if avl => 1 : number of nodes to reach NULL node, if red-black => root is always black
		node_p->parent = NULL;
		balancedbst_p->bucket_count = 1;
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

void insert_entry(balancedbst* balancedbst_p, const void* key_p, const void* value_p)
{
	// create a new node for the entry to be inserted
	bstnode* node_p = get_node(key_p, value_p);

	// insert that node in the tree
	insert_node_in_tree(balancedbst_p, node_p);
}

int update_value(balancedbst* balancedbst_p, const void* key_p, const void* value_p, const void** return_value)
{
	// find a node in the tree that has key_p as its key
	bstnode* node_p = find_node(balancedbst_p, key_p);

	// if we can not find such a node, return 0
	if(node_p == NULL)
	{
		return 0;
	}
	// else update the value for the node found
	else
	{
		if(return_value != NULL)
		{
			(*return_value) = node_p->data_entry.value;
		}
		node_p->data_entry.value = value_p;
		return 1;
	}
}

void delete_node(bstnode* node_p)
{
	free(node_p);
}

int delete_entry(balancedbst* balancedbst_p, const void* key_p,const void** return_key,const void** return_value)
{
	int deleted_nodes_count = 0;
	bstnode* node_p = find_node(balancedbst_p, key_p);
	if(node_p != NULL)
	{
		switch(balancedbst_p->balanced_tree_type)
		{
			case NON_SELF_BALANCING:
			{
				node_p = remove_node_from_non_self_balancing_tree(balancedbst_p, node_p);
				break;
			}
			case RED_BLACK_TREE :
			{
				node_p = remove_node_from_red_black_tree(balancedbst_p, node_p);
				break;
			}
			case AVL_TREE :
			{
				node_p = remove_node_from_avl_tree(balancedbst_p, node_p);
				break;
			}
		}
		if(node_p != NULL)
		{
			if(return_key != NULL)
			{
				(*(return_key)) = node_p->data_entry.key;
			}
			if(return_value != NULL)
			{
				(*(return_value)) = node_p->data_entry.value;
			}
			delete_node(node_p);
			deleted_nodes_count++;
		}
	}
	return deleted_nodes_count;
}

static void for_each_node(const bstnode* node_p, void (*operation)(const void* key, const void* value, const void* additional_params), const void* additional_params)
{
	if(node_p != NULL)
	{
		for_each_node(node_p->left_sub_tree, operation, additional_params);
		operation(node_p->data_entry.key, node_p->data_entry.value, additional_params);
		for_each_node(node_p->right_sub_tree, operation, additional_params);
	}
}

void for_each_entry(const balancedbst* balancedbst_p, void (*operation)(const void* key, const void* value, const void* additional_params), const void* additional_params)
{
	if(!is_balancedbst_empty((balancedbst*)balancedbst_p))
	{
		for_each_node(balancedbst_p->root, operation, additional_params);
	}
}

static void delete_nodes_from(bstnode* node_p)
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

void deinitialize_balancedbst(balancedbst* balancedbst_p)
{
	if(!is_balancedbst_empty(balancedbst_p))
	{
		delete_nodes_from(balancedbst_p->root);
	}
}

void delete_balancedbst(balancedbst* balancedbst_p)
{
	deinitialize_balancedbst(balancedbst_p);
	free(balancedbst_p);
}

static void print_node(bstnode* node_p, void (*print_key)(const void* key), void (*print_value)(const void* value))
{
	if(node_p != NULL)
	{
		if( is_root_node(node_p) )
		{
			printf("\tROOT NODE     :");
		}
		else if( is_leaf_node(node_p) )
		{
			printf("\tLEAF NODE     :");
		}
		else if( is_internal_node(node_p) )
		{
			printf("\tINTERNAL NODE :");
		}

		printf("\taddress => %p", node_p);
		printf("\tdata => ");
		print_bucket(&(node_p->data_entry), print_key, print_value);
		printf("\t\twith property = %d\n", node_p->node_property);

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
			printf("\t\t\taddress => %p", node_p->parent);
			printf("\tdata => ");
			print_bucket(&(node_p->parent->data_entry), print_key, print_value);
		}

		if( (!is_leaf_node(node_p)) )
		{
			if(node_p->left_sub_tree != NULL)
			{
				printf("\t\thas a LEFT\n\t\t\tchild  => %p", node_p->left_sub_tree);
				printf("\tdata => ");
				print_bucket(&(node_p->left_sub_tree->data_entry), print_key, print_value);
			}
			if(node_p->right_sub_tree != NULL)
			{
				printf("\t\thas a RIGHT\n\t\t\tchild => %p", node_p->right_sub_tree); 
				printf("\tdata => ");
				print_bucket(&(node_p->right_sub_tree->data_entry), print_key, print_value);
			}
		}
		printf("\n");
	}
}

static void print_tree(bstnode* node_p, void (*print_key)(const void* key), void (*print_value)(const void* value))
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
	printf("bucket_count : %llu\n", balancedbst_p->bucket_count);
	print_tree(balancedbst_p->root, print_key, print_value);
	printf("--\n\n");
}

#undef insert_entry
#undef find_value
#undef update_value
#undef delete_entry
#undef for_each_entry
#undef delete_node
#undef find_node