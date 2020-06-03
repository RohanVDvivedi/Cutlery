#include<nonbalancing_bst.h>

#define node bstnode

node* get_smallest_node_from_node(const node* node_p)
{
	return node_p->left_sub_tree == NULL ? ((node*)node_p) : get_smallest_node_from_node(node_p->left_sub_tree);
}

node* get_largest_node_from_node(const node* node_p)
{
	return node_p->right_sub_tree == NULL ? ((node*)node_p) : get_largest_node_from_node(node_p->right_sub_tree);
}

// neither root nor node_p params are suppossed to be NULL in the function below
void insert_node_in_non_self_balancing_tree(balancedbst* balancedbst_p, node* root, node* node_p)
{
	if( balancedbst_p->key_compare(node_p->data_entry.key, root->data_entry.key) < 0 )
	{
		if( root->left_sub_tree == NULL )
		{
			root->left_sub_tree = node_p;
			node_p->parent = root;
			balancedbst_p->bucket_count++;
		}
		else
		{
			insert_node_in_non_self_balancing_tree(balancedbst_p, root->left_sub_tree, node_p);
		}
	}
	else if( balancedbst_p->key_compare(node_p->data_entry.key, root->data_entry.key) >= 0 )
	{
		if( root->right_sub_tree == NULL )
		{
			root->right_sub_tree = node_p;
			node_p->parent = root;
			balancedbst_p->bucket_count++;
		}
		else
		{
			insert_node_in_non_self_balancing_tree(balancedbst_p, root->right_sub_tree, node_p);
		}
	}
}

// the below function only detaches the node that has to be deleted
// returns pointer of the node that has to be deleted
// node_p can not be null in the parameters of the function
node* remove_node_from_non_self_balancing_tree(balancedbst* balancedbst_p, node* node_p)
{
	// if atleast one of its children are NULL, the node itself can be removed easily
	if(node_p->left_sub_tree == NULL || node_p->right_sub_tree == NULL)
	{
		// find the parent reference of the node
		node* parent_node = node_p->parent;

		// the child will have one or no child nodes
		// find any one existing node else if it has no child nodes, either left or right are fine
		node* child_sub_tree = has_only_right_sub_tree(node_p) ? node_p->right_sub_tree : node_p->left_sub_tree;

		// since we are removing node_p we update the child references kept by parents 
		// parents = (parent_node if node_p is internal or leaf node, root pointer if node_p is root node)
		// of node_p to the one and only (or none) child of node_p
		if(is_left_of_its_parent(node_p))
		{
			parent_node->left_sub_tree = child_sub_tree;
		}
		else if(is_right_of_its_parent(node_p))
		{
			parent_node->right_sub_tree = child_sub_tree;
		}
		// if a node is neither left or right of its parent, it does not have a parent, hence is a root node 
		else
		{
			balancedbst_p->root = child_sub_tree;
		}

		// since the node_p is removed, and that is has any children
		// the new parent of child of node_p is the parent node of node_p
		if(!is_leaf_node(node_p)) // just to ensure that child_sub_tree is not NULL
		{
			child_sub_tree->parent = parent_node;
		}

		// decrement the bucket count for the tree
		balancedbst_p->bucket_count--;

		// this new node can not be safely deleted
		return node_p;
	}
	else
	{
		// find a smallest node that is grester than node_p
		node* smallest_node_greater_than_node_p = get_smallest_node_from_node(node_p->right_sub_tree);

		// interchange their data, to bring the removal to any of previously seen easy cases
		bucket bucket_p = node_p->data_entry;
		node_p->data_entry = smallest_node_greater_than_node_p->data_entry;
		smallest_node_greater_than_node_p->data_entry = bucket_p;

		// since we did not remove any node, and now we have duplicate nodes
		// we ask to recursively remove, the duplicate node
		return remove_node_from_non_self_balancing_tree(balancedbst_p, smallest_node_greater_than_node_p);
	}
}

#undef node