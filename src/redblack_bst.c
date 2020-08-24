#include<nonbalancing_bst.h>
#include<rotations_bst.h>
#include<redblack_bst.h>
#include<bst_util.h>

#include<cutlery_stds.h>

#define is_red_node(node_p)					\
	((node_p != NULL) && (node_p->node_property == 0))

#define is_black_node(node_p)				\
	((node_p == NULL) || (node_p->node_property == 1))

static void make_node_red(bstnode* node_p)
{
	if(node_p != NULL)
	{
		node_p->node_property = 0;
	}
}

static void make_node_black(bstnode* node_p)
{
	if(node_p != NULL)
	{
		node_p->node_property = 1;
	}
}

static void exchange_colours(bstnode* node_p1, bstnode* node_p2)
{
	int temp_color = node_p1->node_property;
	node_p1->node_property = node_p2->node_property;
	node_p2->node_property = temp_color;
}

// handle imbalance occuring in red black tree
// only a red node can be imbalanced in a red black tree
// i.e. the parameter node_p->node_property == 0, we dont check that in the function
static void handle_imbalance_in_red_black_tree(bst* bst_p, bstnode* node_p)
{
	if(is_root_node(node_p))
	{
		make_node_black(node_p); //root node is always black
	}
	else
	{
		if( is_red_node(node_p->parent) ) // the parent of the node is red
		{
			// get references to its parent and grand parent nodes
			bstnode* parent_node = node_p->parent;
			bstnode* grand_parent_node = parent_node->parent;

			// node_p's uncle is red
			if ( (is_right_of_its_parent(parent_node) && is_red_node(grand_parent_node->left)) ||
			(is_left_of_its_parent(parent_node) && is_red_node(grand_parent_node->right)) )
			{
				make_node_red(grand_parent_node);
				make_node_black(grand_parent_node->left);
				make_node_black(grand_parent_node->right);
				handle_imbalance_in_red_black_tree(bst_p, grand_parent_node);
			}
			// when node_p's uncle is black
			else
			{
				// i.e. if this is a right right case
				if( is_right_of_its_parent(node_p) && is_right_of_its_parent(parent_node) )
				{
					left_rotate_tree(bst_p, grand_parent_node);
					make_node_red(grand_parent_node);
					make_node_black(parent_node);
				}
				// i.e. if this is a left left case
				else if( is_left_of_its_parent(node_p) && is_left_of_its_parent(parent_node) )
				{
					right_rotate_tree(bst_p, grand_parent_node);
					make_node_red(grand_parent_node);
					make_node_black(parent_node);
				}
				// i.e. if this is right left case
				else if( is_right_of_its_parent(node_p) && is_left_of_its_parent(parent_node) )
				{
					left_rotate_tree(bst_p, parent_node);
					handle_imbalance_in_red_black_tree(bst_p, parent_node);
				}
				// i.e. if this is left right case
				else if( is_left_of_its_parent(node_p) && is_right_of_its_parent(parent_node) )
				{
					right_rotate_tree(bst_p, parent_node);
					handle_imbalance_in_red_black_tree(bst_p, parent_node);
				}
			}
		}
	}
}

// neither root nor node_p params are suppossed to be NULL in the function below
void insert_node_in_red_black_tree(bst* bst_p, bstnode* node_p)
{
	// this is a red node
	node_p->node_property = 0;

	// insert this node as if it is getting inserted in a non self balancing tree
	insert_node_in_non_self_balancing_tree(bst_p, node_p);

	// handle the imbalance in the red balck tree introduced by inserting the node
	handle_imbalance_in_red_black_tree(bst_p, node_p);
}

// here it is mandatory to pass both the imbalanced node and its parent
static void handle_black_height_imbalance(bst* bst_p, bstnode* double_black_node, bstnode* double_blacks_parent)
{
	//CASE1 :;
	if(double_black_node != NULL && is_root_node(double_black_node))
	{
		make_node_black(double_black_node);
		goto EXIT;// terminal case 1
	}

	// we calculate the sibling node's reference pointer here becauase, if the double black node is a root node (as in case 1), it will not have a sibling node
	bstnode* double_blacks_sibling = ((double_black_node == double_blacks_parent->right) ? double_blacks_parent->left : double_blacks_parent->right);

	//CASE2 :;
	if(is_black_node(double_blacks_parent) && is_red_node(double_blacks_sibling) && is_black_node(double_blacks_sibling->left) && is_black_node(double_blacks_sibling->right))
	{
		if(is_right_of_its_parent(double_blacks_sibling))
		{
			left_rotate_tree(bst_p, double_blacks_parent);
		}
		else if(is_left_of_its_parent(double_blacks_sibling))
		{
			right_rotate_tree(bst_p, double_blacks_parent);
		}
		else
		{
			goto CASE3; // could not identify, if it was case 2, check next case
		}
		make_node_red(double_blacks_parent);
		make_node_black(double_blacks_sibling);

		// update the sibline node, that changed because of rotation
		double_blacks_sibling = ((double_black_node == double_blacks_parent->right) ? double_blacks_parent->left : double_blacks_parent->right);
		goto CASE4;
		// case 2
	}

	CASE3 :;
	if(is_black_node(double_blacks_parent) && is_black_node(double_blacks_sibling) && is_black_node(double_blacks_sibling->left) && is_black_node(double_blacks_sibling->right))
	{
		make_node_red(double_blacks_sibling);
		handle_black_height_imbalance(bst_p, double_blacks_parent, double_blacks_parent->parent);
		goto EXIT;
		// case 3
	}

	CASE4 :;
	if(is_red_node(double_blacks_parent) && is_black_node(double_blacks_sibling) && is_black_node(double_blacks_sibling->left) && is_black_node(double_blacks_sibling->right))
	{
		make_node_red(double_blacks_sibling);
		make_node_black(double_blacks_parent);
		goto EXIT;// terminal case 4
	}

	//CASE5 :;
	if(is_black_node(double_blacks_sibling) && (is_red_node(double_blacks_sibling->left) || is_red_node(double_blacks_sibling->right)))
	{
		bstnode* red_child = NULL;
		if(is_right_of_its_parent(double_blacks_sibling) && is_black_node(double_blacks_sibling->right))
		{
			red_child = double_blacks_sibling->left;
			right_rotate_tree(bst_p, double_blacks_sibling);
		}
		else if(is_left_of_its_parent(double_blacks_sibling) && is_black_node(double_blacks_sibling->left))
		{
			red_child = double_blacks_sibling->right;
			left_rotate_tree(bst_p, double_blacks_sibling);
		}
		else
		{
			goto CASE6; // could not identify, if it was case 5, check next case
		}
		exchange_colours(double_blacks_sibling, red_child);

		// update the sibline node, that changed because of rotation
		double_blacks_sibling = ((double_black_node == double_blacks_parent->right) ? double_blacks_parent->left : double_blacks_parent->right);
		goto CASE6;
		// case 5
	}

	CASE6 :;
	if(is_black_node(double_blacks_sibling) && (is_red_node(double_blacks_sibling->left) || is_red_node(double_blacks_sibling->right)))
	{
		bstnode* red_child = NULL;
		if(is_right_of_its_parent(double_blacks_sibling) && is_red_node(double_blacks_sibling->right))
		{
			red_child = double_blacks_sibling->right;
			left_rotate_tree(bst_p, double_blacks_parent);
		}
		else if(is_left_of_its_parent(double_blacks_sibling) && is_red_node(double_blacks_sibling->left))
		{
			red_child = double_blacks_sibling->left;
			right_rotate_tree(bst_p, double_blacks_parent);
		}
		else
		{
			goto EXIT; // could not identify, if it was case 6, this is Error, but still exit
		}
		exchange_colours(double_blacks_parent, double_blacks_sibling);
		make_node_black(red_child);

		// update the sibline node, that changed because of rotation
		double_blacks_sibling = ((double_black_node == double_blacks_parent->right) ? double_blacks_parent->left : double_blacks_parent->right);
		goto EXIT;// terminal case 6
	}

	EXIT: return;
}

// the below function only detaches the node that has to be removed, it does not unintialize it
// you must make it NULL, so it is identified as not existing in any bst
void remove_node_from_red_black_tree(bst* bst_p, bstnode* node_p)
{
	// remove the node as if it is a normal tree
	remove_node_from_non_self_balancing_tree(bst_p, node_p);

	// we can not balance an empty tree
	// hence return if the tree is empty after deletion
	if(is_balancedbst_empty(bst_p))
	{
		return;
	}

	// only if the node that is removed is a black node
	// the black height of the tree reduces in one of its branch and we need imbalance handling
	if(is_black_node(node_p))
	{
		// handle height imbalance
		bstnode* imbalance_node = (node_p->right != NULL) ? node_p->right : node_p->left;
		bstnode* imbalance_parent = node_p->parent;
		if(is_red_node(imbalance_node))
		{
			make_node_black(imbalance_node);
		}
		else
		{
			handle_black_height_imbalance(bst_p, imbalance_node, imbalance_parent);
		}
	}
}