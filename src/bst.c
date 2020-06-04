#include<bst.h>
#include<nonbalancing_bst.h>
#include<avl_bst.h>
#include<redblack_bst.h>
#include<bst_util.h>

void initialize_bst(bst* bst_p, bsttype type, unsigned long long int node_offset, int (*compare)(const void* data1, const void* data2))
{
	bst_p->type = type;
	bst_p->root = NULL;
	bst_p->node_offset = node_offset;
	bst_p->compare = compare;
	bst_p->node_count = 0;
}

#define is_greater(compare_A_with_B) 	(compare_A_with_B > 0)
#define is_lesser(compare_A_with_B)		(compare_A_with_B < 0)
#define is_equal(compare_A_with_B)		(compare_A_with_B == 0)

// recursively searches for bstnode that holds data equal to the given data
static const bstnode* find_node_recursively(const bst* bst_p, const bstnode* root, const void* data)
{
	int compared_data_with_root = bst_p->compare(data, get_data(root));
	if(is_equal(compared_data_with_root))
	{
		return ((bstnode*)root);
	}
	else if(root->left != NULL && is_lesser(compared_data_with_root))
	{
		return find_node_recursively(bst_p, root->left, data);
	}
	else if(root->right != NULL && is_greater(compared_data_with_root))
	{
		return find_node_recursively(bst_p, root->right, data);
	}
	else
	{
		return NULL;
	}
}

static const bstnode* find_node_preceding_or_equals_recursively(const bst* bst_p, const bstnode* root, const void* data)
{
	int compared_data_with_root = bst_p->compare(data, get_data(root));
	if(is_equal(compared_data_with_root))
	{
		return ((bstnode*)root);
	}
	else if(root->left != NULL && is_lesser(compared_data_with_root))
	{
		return find_node_preceding_or_equals_recursively(bst_p, root->left, data);
	}
	else if(is_greater(compared_data_with_root))
	{
		if(root->right != NULL)
		{
			return find_node_preceding_or_equals_recursively(bst_p, root->right, data);
		}
		return root;
	}
	else
	{
		return NULL;
	}
}

static const bstnode* find_node_succeeding_or_equals_recursively(const bst* bst_p, const bstnode* root, const void* data)
{
	int compared_data_with_root = bst_p->compare(data, get_data(root));
	if(is_equal(compared_data_with_root))
	{
		return ((bstnode*)root);
	}
	else if(is_lesser(compared_data_with_root))
	{
		if(root->left != NULL)
		{
			return find_node_succeeding_or_equals_recursively(bst_p, root->left, data);
		}
		return root;
	}
	else if(root->right != NULL && is_greater(compared_data_with_root))
	{
		return find_node_succeeding_or_equals_recursively(bst_p, root->right, data);
	}
	else
	{
		return NULL;
	}
}

const void* find_equals_in_bst(const bst* bst_p, const void* data)
{
	if(is_balancedbst_empty(bst_p))
	{
		return NULL;
	}
	else
	{
		const bstnode* node_p = find_node_recursively(bst_p, bst_p->root, data);
		return (node_p != NULL) ? get_data(node_p) : NULL;
	}
}

const void* find_preceding_or_equals(const bst* bst_p, const void* data)
{
	if(is_balancedbst_empty(bst_p))
	{
		return NULL;
	}
	else
	{
		const bstnode* node_p = find_node_preceding_or_equals_recursively(bst_p, bst_p->root, data);
		return (node_p != NULL) ? get_data(node_p) : NULL;
	}
}

const void* find_succeeding_or_equals(const bst* bst_p, const void* data)
{
	if(is_balancedbst_empty(bst_p))
	{
		return NULL;
	}
	else
	{
		const bstnode* node_p = find_node_succeeding_or_equals_recursively(bst_p, bst_p->root, data);
		return (node_p != NULL) ? get_data(node_p) : NULL;
	}
}

const void* find_smallest(const bst* bst_p)
{
	if(is_balancedbst_empty(bst_p))
	{
		return NULL;
	}
	else
	{
		const bstnode* node_p = get_smallest_node_from_node(bst_p->root);
		return (node_p != NULL) ? get_data(node_p) : NULL;
	}
}

const void* find_largest(const bst* bst_p)
{
	if(is_balancedbst_empty(bst_p))
	{
		return NULL;
	}
	else
	{
		const bstnode* node_p = get_largest_node_from_node(bst_p->root);
		return (node_p != NULL) ? get_data(node_p) : NULL;
	}
}

int exists_in_bst(bst* bst_p, const void* data)
{
	bstnode* node_p = get_node(data);
	return bstnode_exists_in_this_bst(node_p);
}

int insert_in_bst(bst* bst_p, const void* data)
{
	bstnode* node_p = get_node(data);

	if(bstnode_exists_in_any_bst(node_p) || (!is_new_bstnode(node_p)))
	{
		return 0;
	}

	// if the root of the tree is NULL, i.e. the tree is empty, add a new root to the tree
	if(is_balancedbst_empty(bst_p))
	{
		bst_p->root = node_p;

		// if avl => 1 : number of nodes to reach NULL node, if red-black => root is always black
		node_p->node_property = (bst_p->type == NON_SELF_BALANCING) ? 0 : 1;

		node_p->parent = NULL;

		node_p->belongs_to_bst = bst_p;
		bst_p->node_count = 1;
		return 1;
	}

	// else insert node as per the balancing that this tree uses
	switch(bst_p->type)
	{
		case NON_SELF_BALANCING:
		{
			insert_node_in_non_self_balancing_tree(bst_p, node_p);
			break;
		}
		case RED_BLACK_TREE :
		{
			insert_node_in_red_black_tree(bst_p, node_p);
			break;
		}
		case AVL_TREE :
		{
			insert_node_in_avl_tree(bst_p, node_p);
			break;
		}
	}

	node_p->belongs_to_bst = bst_p;
	bst_p->node_count++;

	return 1;
}

int remove_from_bst(bst* bst_p, const void* data)
{
	bstnode* node_p = get_node(data);

	if((!bstnode_exists_in_this_bst(node_p)) || is_new_bstnode(node_p))
	{
		return 0;
	}

	switch(bst_p->type)
	{
		case NON_SELF_BALANCING:
		{
			remove_node_from_non_self_balancing_tree(bst_p, node_p);
			break;
		}
		case RED_BLACK_TREE :
		{
			remove_node_from_red_black_tree(bst_p, node_p);
			break;
		}
		case AVL_TREE :
		{
			remove_node_from_avl_tree(bst_p, node_p);
			break;
		}
	}

	// NULL all references of the removed node
	initialize_bstnode(node_p);
	bst_p->node_count--;

	return 1;
}

static void for_each_node(const bst* bst_p, const bstnode* node_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params)
{
	if(node_p != NULL)
	{
		for_each_node(bst_p, node_p->left, operation, additional_params);
		operation(get_data(node_p), additional_params);
		for_each_node(bst_p, node_p->right, operation, additional_params);
	}
}

void for_each_in_bst(const bst* bst_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params)
{
	if(!is_balancedbst_empty(bst_p))
	{
		for_each_node(bst_p, bst_p->root, operation, additional_params);
	}
}

static void print_node(const bst* bst_p, const bstnode* node_p, void (*print_element)(const void* data))
{
	if(node_p != NULL)
	{
		if(is_root_node(node_p))
		{
			printf("\tROOT NODE     : (%d)", node_p->node_property);
		}
		else if(is_leaf_node(node_p))
		{
			printf("\tLEAF NODE     : (%d)", node_p->node_property);
		}
		else if(is_internal_node(node_p))
		{
			printf("\tINTERNAL NODE : (%d)", node_p->node_property);
		}

		printf("\t\t\t[%p] => ", node_p);
		print_element(get_data(node_p));
		printf("\n");

		if( !is_root_node(node_p) )
		{
			if(is_left_of_its_parent(node_p))
			{
				printf("\t\tis LEFT of ");
			}
			else if(is_right_of_its_parent(node_p))
			{
				printf("\t\tis RIGHT of ");
			}
			printf("\t\t\t[%p] =>", node_p->parent);
			print_element(get_data(node_p->parent));
			printf("\n");
		}

		if( (!is_leaf_node(node_p)) )
		{
			if(node_p->left != NULL)
			{
				printf("\t\thas a LEFT  child => \t\t[%p] => ", node_p->left);
				print_element(get_data(node_p->left));
				printf("\n");
			}
			if(node_p->right != NULL)
			{
				printf("\t\thas a RIGHT child => \t\t[%p] => ", node_p->right);
				print_element(get_data(node_p->right));
				printf("\n");
			}
		}
		printf("\n");
	}
}

static void print_tree(const bst* bst_p, const bstnode* node_p, void (*print_element)(const void* data))
{
	if(node_p != NULL)
	{
		print_tree(bst_p, node_p->left, print_element);
	}
	print_node(bst_p, node_p, print_element);
	printf("\n");
	if(node_p != NULL)
	{
		print_tree(bst_p, node_p->right, print_element);
	}
}

void print_bst(const bst* bst_p, void (*print_element)(const void* data))
{
	printf("TREE : %llu\n", bst_p->node_count);
	switch(bst_p->type)
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
	print_tree(bst_p, bst_p->root, print_element);
	printf("--\n\n");
}