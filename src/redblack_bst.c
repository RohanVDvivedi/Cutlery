#include<nonbalancing_bst.h>
#include<rotations_bst.h>
#include<redblack_bst.h>

// used to check if the node is red, in a red black tree
static int is_red_node(const bstnode* node_p)
{
	return node_p != NULL && node_p->node_property == 0;
}

// makes a given node red coloured
static void make_node_red(bstnode* node_p)
{
	if(node_p != NULL)
	{
		node_p->node_property = 0;
	}
}

// used to check if the node is black, in a red black tree
static int is_black_node(const bstnode* node_p)
{
	return node_p == NULL || node_p->node_property == 1;
}

// makes a given node black coloured
static void make_node_black(bstnode* node_p)
{
	if(node_p != NULL)
	{
		node_p->node_property = 1;
	}
}

static void exchange_colours(bstnode* node_p1, bstnode* node_p2)
{
	unsigned long long int temp_color = node_p1->node_property;
	node_p1->node_property = node_p2->node_property;
	node_p2->node_property = temp_color;
}

// handle imbalance occuring in red black tree
// only a red node can be imbalanced in a red black tree
// i.e. the parameter node_p->node_property == 0, we dont check that in the function
void handle_imbalance_in_red_black_tree(balancedbst* balancedbst_p, bstnode* node_p)
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
			if ( (is_right_of_its_parent(parent_node) && is_red_node(grand_parent_node->left_sub_tree)) ||
			(is_left_of_its_parent(parent_node) && is_red_node(grand_parent_node->right_sub_tree)) )
			{
				make_node_red(grand_parent_node);
				make_node_black(grand_parent_node->left_sub_tree);
				make_node_black(grand_parent_node->right_sub_tree);
				handle_imbalance_in_red_black_tree(balancedbst_p, grand_parent_node);
			}
			// when node_p's uncle is black
			else
			{
				// i.e. if this is a right right case
				if( is_right_of_its_parent(node_p) && is_right_of_its_parent(parent_node) )
				{
					left_rotate_tree(balancedbst_p, grand_parent_node);
					make_node_red(grand_parent_node);
					make_node_black(parent_node);
				}
				// i.e. if this is a left left case
				else if( is_left_of_its_parent(node_p) && is_left_of_its_parent(parent_node) )
				{
					right_rotate_tree(balancedbst_p, grand_parent_node);
					make_node_red(grand_parent_node);
					make_node_black(parent_node);
				}
				// i.e. if this is right left case
				else if( is_right_of_its_parent(node_p) && is_left_of_its_parent(parent_node) )
				{
					left_rotate_tree(balancedbst_p, parent_node);
					handle_imbalance_in_red_black_tree(balancedbst_p, parent_node);
				}
				// i.e. if this is left right case
				else if( is_left_of_its_parent(node_p) && is_right_of_its_parent(parent_node) )
				{
					right_rotate_tree(balancedbst_p, parent_node);
					handle_imbalance_in_red_black_tree(balancedbst_p, parent_node);
				}
			}
		}
	}
}

// neither root nor node_p params are suppossed to be NULL in the function below
void insert_node_in_red_black_tree(balancedbst* balancedbst_p, bstnode* root, bstnode* node_p)
{
	// this is a red node
	node_p->node_property = 0;

	// insert this node as if it is getting inserted in a non self balancing tree
	insert_node_in_non_self_balancing_tree(balancedbst_p, root, node_p);

	// handle the imbalance in the red balck tree introduced by inserting the node
	handle_imbalance_in_red_black_tree(balancedbst_p, node_p);
}

// here it is mandatory to pass both the imbalanced node and its parent
void handle_black_height_imbalance(balancedbst* balancedbst_p, bstnode* double_black_node, bstnode* double_blacks_parent)
{
	//CASE1 :;
	if(double_black_node != NULL && is_root_node(double_black_node))
	{
		make_node_black(double_black_node);
		goto EXIT;// terminal case 1
	}

	// we calculate the sibling node's reference pointer here becauase, if the double black node is a root node (as in case 1), it will not have a sibling node
	bstnode* double_blacks_sibling = ((double_black_node == double_blacks_parent->right_sub_tree) ? double_blacks_parent->left_sub_tree : double_blacks_parent->right_sub_tree);

	//CASE2 :;
	if(is_black_node(double_blacks_parent) && is_red_node(double_blacks_sibling) && is_black_node(double_blacks_sibling->left_sub_tree) && is_black_node(double_blacks_sibling->right_sub_tree))
	{
		if(is_right_of_its_parent(double_blacks_sibling))
		{
			left_rotate_tree(balancedbst_p, double_blacks_parent);
		}
		else if(is_left_of_its_parent(double_blacks_sibling))
		{
			right_rotate_tree(balancedbst_p, double_blacks_parent);
		}
		else
		{
			goto CASE3; // could not identify, if it was case 2, check next case
		}
		make_node_red(double_blacks_parent);
		make_node_black(double_blacks_sibling);

		// update the sibline node, that changed because of rotation
		double_blacks_sibling = ((double_black_node == double_blacks_parent->right_sub_tree) ? double_blacks_parent->left_sub_tree : double_blacks_parent->right_sub_tree);
		goto CASE4;
		// case 2
	}

	CASE3 :;
	if(is_black_node(double_blacks_parent) && is_black_node(double_blacks_sibling) && is_black_node(double_blacks_sibling->left_sub_tree) && is_black_node(double_blacks_sibling->right_sub_tree))
	{
		make_node_red(double_blacks_sibling);
		handle_black_height_imbalance(balancedbst_p, double_blacks_parent, double_blacks_parent->parent);
		goto EXIT;
		// case 3
	}

	CASE4 :;
	if(is_red_node(double_blacks_parent) && is_black_node(double_blacks_sibling) && is_black_node(double_blacks_sibling->left_sub_tree) && is_black_node(double_blacks_sibling->right_sub_tree))
	{
		make_node_red(double_blacks_sibling);
		make_node_black(double_blacks_parent);
		goto EXIT;// terminal case 4
	}

	//CASE5 :;
	if(is_black_node(double_blacks_sibling) && (is_red_node(double_blacks_sibling->left_sub_tree) || is_red_node(double_blacks_sibling->right_sub_tree)))
	{
		bstnode* red_child = NULL;
		if(is_right_of_its_parent(double_blacks_sibling) && is_black_node(double_blacks_sibling->right_sub_tree))
		{
			red_child = double_blacks_sibling->left_sub_tree;
			right_rotate_tree(balancedbst_p, double_blacks_sibling);
		}
		else if(is_left_of_its_parent(double_blacks_sibling) && is_black_node(double_blacks_sibling->left_sub_tree))
		{
			red_child = double_blacks_sibling->right_sub_tree;
			left_rotate_tree(balancedbst_p, double_blacks_sibling);
		}
		else
		{
			goto CASE6; // could not identify, if it was case 5, check next case
		}
		exchange_colours(double_blacks_sibling, red_child);

		// update the sibline node, that changed because of rotation
		double_blacks_sibling = ((double_black_node == double_blacks_parent->right_sub_tree) ? double_blacks_parent->left_sub_tree : double_blacks_parent->right_sub_tree);
		goto CASE6;
		// case 5
	}

	CASE6 :;
	if(is_black_node(double_blacks_sibling) && (is_red_node(double_blacks_sibling->left_sub_tree) || is_red_node(double_blacks_sibling->right_sub_tree)))
	{
		bstnode* red_child = NULL;
		if(is_right_of_its_parent(double_blacks_sibling) && is_red_node(double_blacks_sibling->right_sub_tree))
		{
			red_child = double_blacks_sibling->right_sub_tree;
			left_rotate_tree(balancedbst_p, double_blacks_parent);
		}
		else if(is_left_of_its_parent(double_blacks_sibling) && is_red_node(double_blacks_sibling->left_sub_tree))
		{
			red_child = double_blacks_sibling->left_sub_tree;
			right_rotate_tree(balancedbst_p, double_blacks_parent);
		}
		else
		{
			goto EXIT; // could not identify, if it was case 6, this is Error, but still exit
		}
		exchange_colours(double_blacks_parent, double_blacks_sibling);
		make_node_black(red_child);

		// update the sibline node, that changed because of rotation
		double_blacks_sibling = ((double_black_node == double_blacks_parent->right_sub_tree) ? double_blacks_parent->left_sub_tree : double_blacks_parent->right_sub_tree);
		goto EXIT;// terminal case 6
	}

	EXIT: return;
}

// the below function only detaches the node that has to be deleted
// returns pointer of the node that has to be deleted
// node_p can not be null in the parameters of the function

// the below function only detaches the node that has to be removed, it does not unintialize it
// you must make it NULL, so it is identified as not existing in any bst
bstnode* remove_node_from_red_black_tree(balancedbst* balancedbst_p, bstnode* node_p)
{
	// remove the node as if it is a normal tree, reacquire the pointer to the node that needs to be deleted
	node_p = remove_node_from_non_self_balancing_tree(balancedbst_p, node_p);

	// we can not balance an empty tree
	// hence return if the tree is empty after deletion
	if(is_balancedbst_empty(balancedbst_p))
	{
		// return the node that needs to be deleted
		return node_p;
	}

	// only if the node that is to be deleted is a black node
	// the black height of the tree reduces in one of its branch and we need imbalance handling
	if(is_black_node(node_p))
	{
		// handle height imbalance
		bstnode* imbalance_node = node_p->right_sub_tree != NULL ? node_p->right_sub_tree : node_p->left_sub_tree;
		bstnode* imbalance_parent = node_p->parent;
		if(is_red_node(imbalance_node))
		{
			make_node_black(imbalance_node);
		}
		else
		{
			handle_black_height_imbalance(balancedbst_p, imbalance_node, imbalance_parent);
		}
	}

	// return the node that needs to be deleted
	return node_p;
}