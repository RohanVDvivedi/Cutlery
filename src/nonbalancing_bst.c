#include<nonbalancing_bst.h>
#include<rotations_bst.h>		// used only for interchange positions of nodes, no rotations performed
#include<bst_util.h>

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
	// if atleast one of its children are NULL, the node itself can be removed easily
	if(node_p->left == NULL || node_p->right == NULL)
	{
		// find the parent reference of the node
		bstnode* parent_node = node_p->parent;

		// the child will have one or no child nodes
		// find any one existing node else if it has no child nodes, either left or right are fine
		bstnode* none_or_1_child = has_only_right_sub_tree(node_p) ? node_p->right : node_p->left;

		// since we are removing node_p we update the child references kept by parents 
		// parents = (parent_node if node_p is internal or leaf node, root pointer if node_p is root node)
		// of node_p to the one and only (or none) child of node_p
		if(is_left_of_its_parent(node_p))
		{
			parent_node->left = none_or_1_child;
		}
		else if(is_right_of_its_parent(node_p))
		{
			parent_node->right = none_or_1_child;
		}
		// if a node is neither left or right of its parent, it does not have a parent, hence is a root node by definition
		else
		{
			bst_p->root = none_or_1_child;
		}

		// since the node_p is removed, and that is has any children
		// the new parent of child of node_p is the parent node of node_p
		if(!is_leaf_node(node_p)) // just to ensure that none_or_1_child is not NULL
		{
			none_or_1_child->parent = parent_node;
		}
	}
	else
	{
		// find a smallest node that is greater than node_p
		bstnode* smallest_node_greater_than_node_p = (bstnode*) get_smallest_node_from_node(node_p->right);

		// interchange their positions in the tree, to bring the removal to previously seen easy cases
		exchange_positions_in_bst(bst_p, node_p, smallest_node_greater_than_node_p);

		// the node_p now is either a leaf node or has only 1 child (right child)
		// and will be handled by the if case in the next recursion, iteration
		remove_node_from_non_self_balancing_tree(bst_p, node_p);
	}
}