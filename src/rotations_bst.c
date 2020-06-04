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

void exchange_positions_in_bst(bst* bst_p, bstnode* A, bstnode* B)
{
	if(A->parent == B)
	{
		exchange_positions_in_bst(bst_p, B, A);
		return;
	}

	bstnode A_ = *A;
	bstnode B_ = *B;

	if(B->parent == A)
	{
		A->left = B_.left;
		if(B_.left != NULL)
		{
			B_.left->parent = A;
		}

		A->right = B_.right;
		if(B_.right != NULL)
		{
			B_.right->parent = A;
		}

		A->parent = B;
		if(A_.left == B)
		{
			B->left = A;
			B->right = A_.right;
			if(A_.right != NULL)
			{
				A_.right->parent = B;
			}
		}
		else if(A_.right == B)
		{
			B->right = A;
			B->left = A_.left;
			if(A_.left != NULL)
			{
				A_.left->parent = B;
			}
		}

		B->parent = A_.parent;
		if(A_.parent != NULL)
		{
			if(A_.parent->left == A)
			{
				A_.parent->left = B;
			}
			else if(A_.parent->right == A)
			{
				A_.parent->right = B;
			}
		}
		else
		{
			bst_p->root = B;
		}
	}
	else
	{
		if(A_.left != NULL)
		{
			A_.left->parent = B;
		}
		if(A_.right != NULL)
		{
			A_.right->parent = B;
		}

		if(A_.parent != NULL)
		{
			if(A_.parent->left == A)
			{
				A_.parent->left = B;
			}
			else if(A_.parent->right == A)
			{
				A_.parent->right = B;
			}
		}
		else
		{
			bst_p->root = B;
		}


		if(B_.left != NULL)
		{
			B_.left->parent = A;
		}
		if(B_.right != NULL)
		{
			B_.right->parent = A;
		}

		if(B_.parent != NULL)
		{
			if(B_.parent->left == B)
			{
				B_.parent->left = A;
			}
			else if(B_.parent->right == B)
			{
				B_.parent->right = A;
			}
		}
		else
		{
			bst_p->root = A;
		}

		*A = B_;
		*B = A_;
	}
}