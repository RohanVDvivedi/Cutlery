#include<nonbalancing_bst.h>
#include<rotations_bst.h>		// used only for interchange positions of nodes, no rotations performed
#include<bst_util.h>

#include<stdio.h>

bstnode* get_smallest_node_from_node(bstnode* node_p)
{
	bstnode* prev = NULL;
	bstnode* curr = node_p;
	while(curr != NULL)
	{
		prev = curr;
		curr = curr->left;
	}
	return prev;
}

bstnode* get_largest_node_from_node(bstnode* node_p)
{
	bstnode* prev = NULL;
	bstnode* curr = node_p;
	while(curr != NULL)
	{
		prev = curr;
		curr = curr->right;
	}
	return prev;
}

void insert_node_in_non_self_balancing_tree(bst* bst_p, bstnode* node_p)
{
	bstnode* curr = bst_p->root;
	while(curr != NULL)
	{
		int compare_result = bst_p->compare(get_data(node_p), get_data(curr));
		if(compare_result < 0)
		{
			if(curr->left == NULL)
			{
				curr->left = node_p;
				node_p->parent = curr;
				return;
			}
			else
				curr = curr->left;
		}
		else if(compare_result >= 0)
		{
			if(curr->right == NULL)
			{
				curr->right = node_p;
				node_p->parent = curr;
				return;
			}
			else
				curr = curr->right;
		}
	}
}

// the below function only detaches the node that has to be removed, it does not unintialize it
// you must make it NULL, so it is identified as not existing in any bst
void remove_node_from_non_self_balancing_tree(bst* bst_p, bstnode* node_p)
{
	// we can only remove the node if atleast one of the node child is NULL
	if(node_p->left != NULL && node_p->right != NULL)
	{
		// find a smallest node that is greater than node_p
		bstnode* succeeding_node_p = get_smallest_node_from_node(node_p->right);

		// interchange their positions in the tree, to bring the removal to previously seen easy cases
		exchange_positions_in_bst(bst_p, node_p, succeeding_node_p);
	}
	// the node_p now is either a leaf node or has only 1 child
	
	// the parent reference of the node to be removed
	bstnode* parent_node = node_p->parent;

	// the node_p will have one or no child nodes, find any one existing node
	bstnode* none_or_1_child = has_only_right_sub_tree(node_p) ? node_p->right : node_p->left;

	if(parent_node != NULL)
	{
		if(is_left_of_its_parent(node_p))
			parent_node->left = none_or_1_child;
		else if(is_right_of_its_parent(node_p))
			parent_node->right = none_or_1_child;
	}
	else	// node_p was a root node
		bst_p->root = none_or_1_child;

	if(none_or_1_child != NULL)
		none_or_1_child->parent = parent_node;
}