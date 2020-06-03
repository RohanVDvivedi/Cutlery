#include<nonbalancing_bst.h>
#include<rotations_bst.h>

#include<avl_bst.h>

#define node bstnode

// gets max nodes from itself to any NULL, including itself, in an avl tree
static unsigned long long int get_max_height(node* node_p)
{
	if(node_p == NULL)
	{
		return 0;
	}
	if(node_p->node_property > 0)
	{
		return node_p->node_property;
	}
	unsigned long long int left_tree_max_height = get_max_height(node_p->left_sub_tree);
	unsigned long long int right_tree_max_height = get_max_height(node_p->right_sub_tree);
	node_p->node_property = (left_tree_max_height > right_tree_max_height ? left_tree_max_height : right_tree_max_height) + 1;
	return node_p->node_property;
}

// updates max height of the node, in an avl tree
static void update_max_height(node* node_p)
{
	node_p->node_property = 0;
	get_max_height(node_p);
}

// handle imbalance occuring in avl tree, starting at input_node_p
void handle_imbalance_in_avl_tree(balancedbst* balancedbst_p, node* input_node_p)
{
	// maintain a reference that traces
	// all the nodes from  node_p to root
	node* unbalanced_node = input_node_p;
	node* prev_unbalanced_node = input_node_p;
	node* prev_prev_unbalanced_node = input_node_p;

	// loop untill you reach the root
	while(unbalanced_node != NULL)
	{
		update_max_height(unbalanced_node);
		unsigned long long int left_tree_max_height = get_max_height(unbalanced_node->left_sub_tree);
		unsigned long long int right_tree_max_height = get_max_height(unbalanced_node->right_sub_tree);

		// if left tree height is more, do right rotate
		if(left_tree_max_height >= 2 + right_tree_max_height)
		{
			// if it is not a left left case, make it a left left case
			// below logic resolves the left right case with one more rotation in the child of unbalanced node
			if(is_right_of_its_parent(prev_prev_unbalanced_node))
			{
				left_rotate_tree(balancedbst_p, prev_unbalanced_node);

				// after above rotation, left left sub tree height can not be trusted
				prev_unbalanced_node->node_property = 0;
				prev_prev_unbalanced_node->node_property = 0;
			}

			// for a left left case, just right rotate
			right_rotate_tree(balancedbst_p, unbalanced_node);

			// after rotation the height of the unbalanced_node or
			// any of its new current parent can not be trusted
			unbalanced_node->node_property = 0;
			unbalanced_node->parent->node_property = 0;
		}
		// if right tree height is more, do left rotate
		else if(right_tree_max_height >= 2 + left_tree_max_height)
		{
			// if it is not a right right case, make it a right right case
			// below logic resolves the right left case with one more rotation in the child of unbalanced node
			if(is_left_of_its_parent(prev_prev_unbalanced_node))
			{
				right_rotate_tree(balancedbst_p, prev_unbalanced_node);

				// after above rotation, right right sub tree height can not be trusted
				prev_unbalanced_node->node_property = 0;
				prev_prev_unbalanced_node->node_property = 0;
			}

			// for a right right case, just right rotate
			left_rotate_tree(balancedbst_p, unbalanced_node);

			// after rotation the height of the unbalanced_node or
			// any of its new current parent can not be trusted
			unbalanced_node->node_property = 0;
			unbalanced_node->parent->node_property = 0;
		}

		// update the references
		prev_prev_unbalanced_node = prev_unbalanced_node;
		prev_unbalanced_node = unbalanced_node;
		unbalanced_node = unbalanced_node->parent;
	}
}

// neither root nor node_p params are suppossed to be NULL in the function below
void insert_node_in_avl_tree(balancedbst* balancedbst_p, node* root, node* node_p)
{
	// the new node will be inserted any where, hence we mark it's node_property for recalculation
	node_p->node_property = 0;

	// insert this node as if it is getting inserted in a non self balancing tree
	insert_node_in_non_self_balancing_tree(balancedbst_p, root, node_p);

	// handle the imbalance in the red balck tree introduced by inserting the node
	handle_imbalance_in_avl_tree(balancedbst_p, node_p);
}

// the below function only detaches the node thatr has to be deleted
// returns pointer of the node that has to be deleted
// node_p can not be null in the parameters of the function
node* remove_node_from_avl_tree(balancedbst* balancedbst_p, node* node_p)
{
	// remove the node as if it is a normal tree, reacquire the pointer to the node that needs to be deleted
	node_p = remove_node_from_non_self_balancing_tree(balancedbst_p, node_p);

	// we can not balance an empty tree
	if(!is_balancedbst_empty(balancedbst_p))
	{
		// the imbalance is always in the parent node of the node currently going to be deleted
		node* imbalance_node_p = node_p->parent;

		// remove the imbalance and remove it
		handle_imbalance_in_avl_tree(balancedbst_p, imbalance_node_p);
	}

	// return the node that needs to be deleted
	return node_p;
}

#undef node