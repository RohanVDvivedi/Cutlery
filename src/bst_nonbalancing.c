#include<bst_nonbalancing.h>
#include<bst_rotations.h>		// used only for interchange positions of nodes, no rotations performed
#include<bst_util.h>

#include<cutlery_stds.h>

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
	// pointer to the node that will be node_p's parent
	bstnode* node_p_parent = NULL;

	// find the pointer to the position, where the node_p should be installed
	// start this search from the root
	bstnode** insertion_point = &(bst_p->root);

	// insertion_point points to the left or right pointer of the node_p_parent
	// unless insertion_point points to the position of the root

	while((*insertion_point) != NULL)
	{
		// if insertion point is not empty/NULL
		// then it could be new possible parent of the node_p
		node_p_parent = *insertion_point;

		// compare the bstnode* at the insertion point
		int compare_result = bst_p->compare(get_data(node_p), get_data((*insertion_point)));

		// new insertion point would be at the left or right of the current insertion point
		if(compare_result < 0)
			insertion_point = &((*insertion_point)->left);
		else if(compare_result >= 0)
			insertion_point = &((*insertion_point)->right);
	}

	// install the new node_p
	*insertion_point = node_p;
	node_p->parent = node_p_parent;
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