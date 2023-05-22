#ifndef BINARY_TREE_ARRAY_UTIL_H
#define BINARY_TREE_ARRAY_UTIL_H

#include<cutlery_stds.h>

#include<nary_tree_as_array_util.h>

/*
	binary tree as array is a special case of N-ary tree as an array
	here each node has atmost 2 children
	and just like N-ary tree as array, binary tree as array is also completely filled
	refer to "nary_tree_as_array_util.h"

	standard binary tree as array can be represented as follows

	all indexes are unsigned ints
	array form (when indexed) :

	0 1 2 3 4 5 6 7 8 9 10 11 ...

	tree form (index to tree node position mapping) :
	             0
	           /   \
	         /       \
	        1         2
	       /  \     /   \
	      3   4     5    6
	     /\   /\   / \   /\
	    7 8  9 10 11 12 13 14
	   /
	  15

	as you can see in a binary tree, the first element of each of the row is always the number that is of the form 0000...000111...11
	i.e. all 0s and then all 1s
*/

#define get_parent_index(child_index) 			get_parent_index_N(child_index, 2)

// if the index is more than or equal to (UINT_MAX / 2), then calculating its child index is always an overflow
#define can_have_any_children(index)			can_have_any_children_N(parent_index, 2)
#define get_left_child_index(parent_index)		get_index_of_ith_child_N(parent_index, 0, 2)
#define get_right_child_index(parent_index)		get_index_of_ith_child_N(parent_index, 1, 2)

#define get_sibling_index(sibling_index)		((((sibling_index) + 1) ^ 1) - 1)

/*
	This file is used by following data structures
	* binary heap (current in cutlery as heap)
	* segment tree (future)
*/

#endif