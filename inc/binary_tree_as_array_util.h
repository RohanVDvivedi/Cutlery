#ifndef BINARY_TREE_ARRAY_UTIL_H
#define BINARY_TREE_ARRAY_UTIL_H

#define get_parent_index(child_index) 			(((child_index) - 1) / 2)

#define get_left_child_index(parent_index)		((2 * (parent_index)) + 1)

#define get_right_child_index(parent_index)		((2 * (parent_index)) + 2)

#define get_sibling_index(sibling_index)		((((sibling_index) - 1) ^ 1) + 1)

#endif