#ifndef BINARY_TREE_ARRAY_UTIL_H
#define BINARY_TREE_ARRAY_UTIL_H

#include<cutlery_stds.h>

#define is_root_index(index)					((index) == 0)

// a root in a binary tree will not have any parent
#define has_parent(index)						(! is_root_index((index)))
#define get_parent_index(child_index) 			(((child_index) - 1) / 2)

// if the index is more than or equal to (UINT_MAX / 2), then calculating its child index is always an overflow
#define can_have_any_children(index)			((index) < ((UINT_MAX) >> 1))
#define get_left_child_index(parent_index)		((2 * (parent_index)) + 1)
#define get_right_child_index(parent_index)		((2 * (parent_index)) + 2)

#define get_sibling_index(sibling_index)		((((sibling_index) + 1) ^ 1) - 1)

/*
	This file is used by following data structures
	* binary heap (current in cutlery as heap)
	* segment tree (future)
*/

#endif