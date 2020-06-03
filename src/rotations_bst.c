#include<rotations_bst.h>
#include<bst_util.h>

/*      A                                _B_
**     /  \                             /   \
**    W    B        left rotation      A     C
**        /  \        --------->>     / \   /  \
**       X    C                      W   X Y    Z
**           / \
**          Y   Z
** returns true if rotation was successfull*/
int left_rotate_tree(bst* bst_p, bstnode* A)
{
	bstnode* parent_of_tree = A->parent;
	bstnode* B = A->right;
	if( B == NULL )
	{
		return 0;
	}
	else
	{
		bstnode* X = B->left;

		if( is_root_node(A) )
		{
			bst_p->root = B;
		}
		else
		{
			if( is_right_of_its_parent(A) )
			{
				parent_of_tree->right = B;
			}
			else if( is_left_of_its_parent(A) )
			{
				parent_of_tree->left = B;
				
			}
		}
		B->parent = parent_of_tree;

		A->right = X;
		if(X != NULL)
		{
			X->parent = A;
		}

		B->left = A;
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
int right_rotate_tree(bst* bst_p, bstnode* A)
{
	bstnode* parent_of_tree = A->parent;
	bstnode* B = A->left;
	if( B == NULL )
	{
		return 0;
	}
	else
	{
		bstnode* X = B->right;

		if( is_root_node(A) )
		{
			bst_p->root = B;
		}
		else
		{
			if( is_right_of_its_parent(A) )
			{
				parent_of_tree->right = B;
			}
			else if( is_left_of_its_parent(A) )
			{
				parent_of_tree->left = B;
			}
		}
		B->parent = parent_of_tree;

		A->left = X;
		if(X != NULL)
		{
			X->parent = A;
		}

		B->right = A;
		A->parent = B;
		return 1;
	}
}