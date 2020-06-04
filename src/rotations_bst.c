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
	/*
	** 		CACHE ORIGINAL VALUES OF A AND B 
	*/
		const bstnode A_ = *A;
		const bstnode B_ = *B;

	/*
	** 		POINT ALL RELATIVES OF A TO B
	*/

		// change the parent pointers of left and right children of A to point to B
		if(A_.left != NULL)
		{
			A_.left->parent = B;
		}
		if(A_.right != NULL)
		{
			A_.right->parent = B;
		}

		// change (left or right) child pointer of parent of A to point B
		if(A_.parent != NULL)
		{
			if(A_.parent->right == A)
			{
				A_.parent->right = B;
			}
			else if(A_.parent->left == A)
			{
				A_.parent->left = B;
			}
		}
		// if A is not left or right of its parent, i.e. it has no parent, then A is root of BST, so we update bst root to point to B
		else
		{
			bst_p->root = B;
		}

	/*
	** 		POINT ALL RELATIVES OF B TO A
	*/

		// change the parent pointers of left and right children of B to point to A
		if(B_.left != NULL)
		{
			B_.left->parent = A;
		}
		if(B_.right != NULL)
		{
			B_.right->parent = A;
		}

		// change (left or right) child pointer of parent of B to point A
		if(B_.parent != NULL)
		{
			if(B_.parent->right == B)
			{
				B_.parent->right = A;
			}
			else if(B_.parent->left == B)
			{
				B_.parent->left = A;
			}
		}
		// if B is not left or right of its parent, i.e. it has no parent, then B is root of BST, so we update bst root to point to A
		else
		{
			bst_p->root = A;
		}

	/*
	** 		POINT ALL POINTERS OF B TO where A points
	**		POINT ALL POINTERS OF A TO where B points
	*/

	// to do this task we will use the cached copies of A and B
	// because in the above process of notifying the relatives of each of them
	// we might have polluted their values

		*A = B_;
		*B = A_;
}