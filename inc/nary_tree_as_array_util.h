#ifndef NARY_TREE_ARRAY_UTIL_H
#define NARY_TREE_ARRAY_UTIL_H

/*
	standard N-ary tree as array, (with N=3) can be represented as follows

	all indexes are unsigned ints
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

#define is_root_index(index)								((index) == 0)

// a root in the N-ary tree will not have any parent
#define has_parent(index)									(! is_root_index((index)))

#define get_parent_index_N(child_index, N) 					(((child_index) - 1) / N)

// the parent_index has any children, only if the parent_index is lesser than or equal to the parent of the last index (CY_UINT_MAX - 1) of the tree
#define can_have_any_children_N(parent_index, N)			(parent_index <= get_parent_index_N((CY_UINT_MAX - 1), N))

// here i must be in range [0, N) (0 inclusive, N exclusive)
#define get_index_of_ith_child_N(parent_index, i, N)		((N * (parent_index)) + 1 + i)

#endif