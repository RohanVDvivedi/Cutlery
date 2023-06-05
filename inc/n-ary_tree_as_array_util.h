#ifndef N_ARY_TREE_AS_ARRAY_UTIL_H
#define N_ARY_TREE_AS_ARRAY_UTIL_H

/*
	standard N-ary tree as array, (with N=3) can be represented as follows

	all indexes are cy_uint-s
	array form (when indexed) :

	0 1 2 3 4 5 6 7 8 9 10 11 ...

	tree form (index to tree node position mapping) :
	              0
	           /  |   \
	         /    |     \
	        1     2      3
	       /|\   /|\   /  | \
	      3 4 5 6 7 8  9 10 11
	     /| 
	   12 13

	There can be atmost CY_UINT_MAX elements in the tree,
	hence the last element in the tree is at index (CY_UINT_MAX - 1)
*/

#define is_root_index_N(index)								((index) == 0)

// a root in the N-ary tree will not have any parent
#define has_parent_N(index)									(! is_root_index_N((index)))

#define get_parent_index_N(child_index, N) 					(((child_index) - 1) / N)

// the parent_index has any children, only if the parent_index is lesser than or equal to the parent of the last index (CY_UINT_MAX - 1) of the tree
#define can_have_any_children_N(parent_index, N)			(parent_index <= get_parent_index_N((CY_UINT_MAX - 1), N))

// here i must be in range [0, N) (0 inclusive, N exclusive)
#define get_index_of_ith_child_N(parent_index, i, N)		((N * (parent_index)) + 1 + i)

// below macro is only valid for binary trees
// it can you used to get index of the sibling node (from left's index to right's index and viceversa)
#define get_sibling_index_2(sibling_index)		((((sibling_index) + 1) ^ 1) - 1)

#endif