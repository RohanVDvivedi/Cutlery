#include<rotations_bst.h>


/*      A                                _B_
**     /  \                             /   \
**    W    B        left rotation      A     C
**        /  \        --------->>     / \   /  \
**       X    C                      W   X Y    Z
**           / \
**          Y   Z
** returns true if rotation was successfull*/
int left_rotate_tree(balancedbst* balancedbst_p, bstnode* A)
{
	bstnode* parent_of_tree = A->parent;
	bstnode* B = A->right_sub_tree;
	if( B == NULL )
	{
		return 0;
	}
	else
	{
		bstnode* X = B->left_sub_tree;

		if( is_root_node(A) )
		{
			balancedbst_p->root = B;
		}
		else
		{
			if( is_right_of_its_parent(A) )
			{
				parent_of_tree->right_sub_tree = B;
			}
			else if( is_left_of_its_parent(A) )
			{
				parent_of_tree->left_sub_tree = B;
				
			}
		}
		B->parent = parent_of_tree;

		A->right_sub_tree = X;
		if(X != NULL)
		{
			X->parent = A;
		}

		B->left_sub_tree = A;
		A->parent = B;
		return 1;
	}
}

/*           A                                 _B_
**         /   \                              /   \
**       B      W      right rotation        C     A
**     /   \             --------->>        / \   /  \
**   C      X                              Z   Y  X   W
**  / \
** Z   Y
** returns true if rotation was successfull*/
int right_rotate_tree(balancedbst* balancedbst_p, bstnode* A)
{
	bstnode* parent_of_tree = A->parent;
	bstnode* B = A->left_sub_tree;
	if( B == NULL )
	{
		return 0;
	}
	else
	{
		bstnode* X = B->right_sub_tree;

		if( is_root_node(A) )
		{
			balancedbst_p->root = B;
		}
		else
		{
			if( is_right_of_its_parent(A) )
			{
				parent_of_tree->right_sub_tree = B;
			}
			else if( is_left_of_its_parent(A) )
			{
				parent_of_tree->left_sub_tree = B;
			}
		}
		B->parent = parent_of_tree;

		A->left_sub_tree = X;
		if(X != NULL)
		{
			X->parent = A;
		}

		B->right_sub_tree = A;
		A->parent = B;
		return 1;
	}
}