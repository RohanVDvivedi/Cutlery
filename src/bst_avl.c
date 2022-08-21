#include<bst_core.h>
#include<bst_rotations.h>
#include<bst_avl.h>
#include<bst_util.h>

#include<cutlery_stds.h>
#include<cutlery_math.h>

// gets max height including all nodes from itself to leaf, including itself and the leaf node, in an avl tree
static int get_max_height(bstnode* node_p)
{
	if(node_p == NULL)
		return 0;
	if(node_p->node_property == 0)
		node_p->node_property = max(get_max_height(node_p->left), get_max_height(node_p->right)) + 1;
	return node_p->node_property;
}

// force update max height of the node, in an avl tree
static void update_max_height(bstnode* node_p)
{
	node_p->node_property = 0;
	get_max_height(node_p);
}

// handle imbalance occuring in avl tree, starting at given input_node_p
static void handle_imbalance_in_avl_tree(bst* bst_p, bstnode* input_node_p)
{
	// start iterationf from the input unbalanced node until you reach the root
	bstnode* unbalanced_node = input_node_p;

	// loop untill you reach the root
	while(unbalanced_node != NULL)
	{
		update_max_height(unbalanced_node);
		int left_tree_max_height = get_max_height(unbalanced_node->left);
		int right_tree_max_height = get_max_height(unbalanced_node->right);

		// if left tree height is more, do right rotate
		if(left_tree_max_height - right_tree_max_height >= 2)
		{
			bstnode* unbalanced_node_child = unbalanced_node->left;

			// LR rotation case, else R rotation case
			if(get_max_height(unbalanced_node_child->left) < get_max_height(unbalanced_node_child->right))
			{
				// after the rotation, sub tree heights can not be trusted
				unbalanced_node_child->node_property = 0;
				unbalanced_node_child->right->node_property = 0;

				left_rotate_tree(bst_p, unbalanced_node_child);
			}

			unbalanced_node_child = unbalanced_node->left;

			// after the rotation, sub tree heights can not be trusted
			unbalanced_node->node_property = 0;
			unbalanced_node_child->node_property = 0;

			// for a left left case, just right rotate
			right_rotate_tree(bst_p, unbalanced_node);
		}
		// if right tree height is more, do left rotate
		else if(right_tree_max_height - left_tree_max_height >= 2)
		{
			bstnode* unbalanced_node_child = unbalanced_node->right;

			// RL rotation case else L rotation
			if(get_max_height(unbalanced_node_child->right) < get_max_height(unbalanced_node_child->left))
			{
				// after the rotation, sub tree heights can not be trusted
				unbalanced_node_child->node_property = 0;
				unbalanced_node_child->left->node_property = 0;

				right_rotate_tree(bst_p, unbalanced_node_child);
			}

			unbalanced_node_child = unbalanced_node->right;

			// after the rotation, sub tree heights can not be trusted
			unbalanced_node->node_property = 0;
			unbalanced_node_child->node_property = 0;

			// for a right right case, just right rotate
			left_rotate_tree(bst_p, unbalanced_node);
		}

		// update the unbalanced node
		unbalanced_node = unbalanced_node->parent;
	}
}

void insert_node_in_avl_tree(bst* bst_p, bstnode* node_p)
{
	// the new node will be inserted any where, hence we mark it's node_property for recalculation
	node_p->node_property = 0;

	// insert this node as if it is getting inserted in a non self balancing tree
	insert_node_in_bst(bst_p, node_p);

	// handle the imbalance in the avl tree introduced by inserting the node
	handle_imbalance_in_avl_tree(bst_p, node_p);
}

// the below function only detaches the node that has to be removed, it does not unintialize it
// you must make it NULL, so it is identified as not existing in any bst
void remove_node_from_avl_tree(bst* bst_p, bstnode* node_p)
{
	// remove the node as if it is a normal tree
	remove_node_from_bst(bst_p, node_p);

	// we can not balance an empty tree
	// hence return if the tree is empty after deletion
	if(is_empty_bst(bst_p))
		return;

	// the imbalance is always in the parent node of the node currently going to be deleted
	bstnode* imbalance_node_p = node_p->parent;

	// remove the imbalance and remove it
	handle_imbalance_in_avl_tree(bst_p, imbalance_node_p);
}