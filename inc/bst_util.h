#ifndef BST_UTIL_H
#define BST_UTIL_H

/*
*	MACROS TO DEFINE THE TYPE OF NODE
*/

#define is_leaf_node(node_p)									\
	((node_p)->left == NULL && (node_p)->right == NULL)

#define is_root_node(node_p)									\
	((node_p)->parent == NULL)

#define is_internal_node(node_p)								\
	(!(is_leaf_node((node_p)) || is_root_node((node_p))))


/*
*	MACROS TO DEFINE SURROUNDINGS OF THE NODE
*/

#define has_only_right_sub_tree(node_p)							\
	((node_p)->left == NULL && (node_p)->right != NULL)

#define has_only_left_sub_tree(node_p)							\
	((node_p)->left != NULL && (node_p)->right == NULL)

#define is_left_of_its_parent(node_p)							\
	((!(is_root_node((node_p)))) && ((node_p)->parent->left == (node_p)))

#define is_right_of_its_parent(node_p)							\
	((!(is_root_node((node_p)))) && ((node_p)->parent->right == (node_p)))

#endif