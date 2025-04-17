#ifndef BST_H
#define BST_H

#include<cutlery/dstring.h>
#include<cutlery/cutlery_stds.h>
#include<cutlery/comparator_interface.h>
#include<cutlery/notifier_interface.h>

/*
** 	BST is short for Binary Search Tree
*/

typedef enum bsttype bsttype;
enum bsttype
{
	NON_SELF_BALANCING,
	AVL_TREE,
	RED_BLACK_TREE
};

typedef struct bst bst;
struct bst;

typedef struct bstnode bstnode;
struct bstnode
{
	// the pointer to the parent node
	bstnode* parent;

	// all < nodes					// all >= nodes
	bstnode* left;					bstnode* right;

	// the property of the node that will be used to help balance the tree
	// a node_property == 0 -> implies that the given node does not exist in any bst
	// for a node that is in the bst,
	// bsttype == NON_SELF_BALANCING => node_property = 1 (always)
	// bsttype == AVL_TREE           => node_property = max( right_tree->node_property, left_tree->node_property) + 1 ( node_property of a NULL node is 0)
	// bsttype == RED_BLACK_TREE     => node_property = is_red_node? ? RED_NODE : BLACK_NODE; ( RED_NODE and BLACK_NODE are defined in the corresponsing header files and are not 0)
	cy_uint node_property;
};

typedef struct order_stat_bstnode order_stat_bstnode;
struct order_stat_bstnode
{
	// this represents the number of nodes in the bst that are contained in the tree rooted at this node
	cy_uint subtree_size;
};

// you can use bst, without embedded a order_stat_bstnode
// pass this as the order_stat_node_offset when you are not using the embedded node
#define NO_ORDER_STAT_BST_NODE_OFFSET INVALID_INDEX

struct bst
{
	// the type of balancing algorithm to use for the binary search tree
	bsttype type;

	// defines address to data with respect to bstnode
	// this is how we reach node addresses from provided user's structure data addresses and viceversa
	cy_uint node_offset;

	// the root node of the tree
	bstnode* root;

	// comparator for the elements
	// it returns 0 if they are same, >0 if data1 is greater than data2 else it must return <0 value
	comparator_interface comparator;

	// node_offset to store the offset of order_stat_bstnode inside order statistics bst
	// set it to NO_ORDER_STAT_BST_NODE_OFFSET, when using a non - order-statistics bst node
	cy_uint order_stat_node_offset;
};

/*
*	note : you can only insert a new node, and remove an existing node
*/

// initializes as if a new bst, may be to reuse
void initialize_bst(bst* bst_p, bsttype type, const comparator_interface* comparator, cy_uint node_offset);
void initialize_order_stat_bst(bst* bst_p, bsttype type, const comparator_interface* comparator, cy_uint node_offset, cy_uint order_stat_node_offset);

// always initialize your bstnode before using it
void initialize_bstnode(bstnode* node_p);

// a free floating bstnode is the one, that is not referenced in any bst, i.e a node that can be inserted to any bst
int is_free_floating_bstnode(const bstnode* node_p);

int is_empty_bst(const bst* bst_p);

/*
**		NOTE FOR USING ALL THE FIND FUNCTIONS BELOW
**
** find_* functions; return the data from the bst, that equals the data given as parameter "data",
** (when compared using the comparator function)
**
** the "data" you provide as parameter must have all the fields required for comparison 
** used in the given comparator function (as in "bst_p" struct).
** i.e. it must have all the fields/attributes of the struct that are used by the comparator i.e. "bst_p->comparator".
*/

// find data in bst_p (given in parameter), that compares equal to data (given in parameter)
// returns NULL, if not found
const void* find_equals_in_bst(const bst* bst_p, const void* data, search_occurence occurence_type);

// find data in bst_p (given in parameter), with the biggest value which is smaller than data (given in parameter)
// returns NULL, if data (given in parameter) is smaller than or equal to the smallest existing data in bst_p (given in parameter)
const void* find_preceding_in_bst(const bst* bst_p, const void* data);

// find data in bst_p (given in parameter), with the biggest value which is smaller than or equal to data (given in parameter)
// returns NULL, if data (given in parameter) is smaller than the smallest existing data in bst_p (given in parameter)
const void* find_preceding_or_equals_in_bst(const bst* bst_p, const void* data);

// find data in bst_p (given in parameter), with the smallest value which is bigger than data (given in parameter)
// returns NULL, if data (given in parameter) is bigger than or equla to the biggest existing data in bst_p (given in parameter)
const void* find_succeeding_in_bst(const bst* bst_p, const void* data);

// find data in bst_p (given in parameter), with the smallest value which is bigger than or equal to data (given in parameter)
// returns NULL, if data (given in parameter) is bigger than the biggest existing data in bst_p (given in parameter)
const void* find_succeeding_or_equals_in_bst(const bst* bst_p, const void* data);

// find data in bst_p (given in parameter), with the smallest value
const void* find_smallest_in_bst(const bst* bst_p);

// find data in bst_p (given in parameter), with the biggest value
const void* find_largest_in_bst(const bst* bst_p);

// get the element that comes inorder next of data_xist in the bst
const void* get_inorder_next_of_in_bst(const bst* bst_p, const void* data_xist);

// get the element that comes inorder previous of data_xist in the bst
const void* get_inorder_prev_of_in_bst(const bst* bst_p, const void* data_xist);

typedef enum sort_direction sort_direction;
enum sort_direction{ASCENDING_ORDERED, DESCENDING_ORDERED};
// find all (or atleast max_result_count number of) data pointers in the bst,
// which compare >= lower_bound and <= upper_bound,
// and accumulate them in the sort_direction order (either ASCENDING or DESCENDING order).
cy_uint find_all_in_range_in_bst(
						const bst* bst_p,				// binary search tree to search in
						
						const void* lower_bound,		// if(lower_bound != NULL), then find all data >= lower_bound, else find all ignoring the lower_bound
						const void* upper_bound,		// if(upper_bound != NULL), then find all data <= upper_bound, else find all ignoring the upper_bound

						sort_direction sort_dirctn,		// if sort_dirctn == ASCENDING_ORDERED,
														// 	then the result set is ordered from least to highest,
														//	stopping only if max_result_count condition is reached
														// vice-versa for DESCENDING_ORDERED
						
						cy_uint max_result_count,	// this is the maximum in-range results to find.
							// TO GET ALL THE RESULTS => max_result_count = CY_UINT_MAX

						// result_accumulator is a function that will be called for a maximum of max_result_count number of times, as long as it returns 1
						// it will be called once for each element found, with parameters; data and additional_params
						// once the result_accumulator returns 0, no further elements would be accumulated
						// i.e. to stop accumulating any more results, the result_accumulator must return a 0
						// and to continue receiving the next result the result_accumulator must return a 1 (or any non-zero value)
						int (*result_accumulator)(const void* data, const void* additional_params),
						const void* additional_params
					);
// The above function can also be used as a for_each_in_range function, to perform a give operation on all the data elements in a given range
// the return value of this function is equal to the number of times the result_accumulator was called (or the number of data elements found)

// inserts given data in the tree
// returns 0, and fails if bstnode of data is not a new node
int insert_in_bst(bst* bst_p, const void* data);

// remove data from the given tree
// returns 0, and fails if bstnode of data is a new node
int remove_from_bst(bst* bst_p, const void* data);

// removes all the elements from the bst and reinitializes their embedded bstnode
// after this if a notifier_interface if specified (i.e. not NULL), then it is notified
void remove_all_from_bst(bst* bst_p, const notifier_interface* ni_p);

// the three functions given below can only be used for an order statistics bst
int is_order_stat_bst(const bst* bst_p);

/*
	order statistics bst functions start

	below functions would fail if the bst is not an order stat bst
*/

// the below function would fail if the provided data is free_floating_bstnode or a new_node, does not exist in the bst_p
cy_uint get_index_of_element_in_bst(const bst* bst_p, const void* data);

const void* get_element_at_index_in_bst(const bst* bst_p, cy_uint index);

cy_uint get_element_count_bst(const bst* bst_p);

/*
	order statistics bst functions end
*/

// traversals possible in the tree
typedef enum bsttraversal bsttraversal;
enum bsttraversal{PRE_ORDER, IN_ORDER, POST_ORDER};

// perform given operation on all the elements of the binary search tree
void for_each_in_bst(const bst* bst_p, bsttraversal traversal, void (*operation)(const void* data, const void* additional_params), const void* additional_params);

// print complete binary search tree
void sprint_bst(dstring* append_str, const bst* bst_p, void (*sprint_element)(dstring* append_str, const void* data, unsigned int tabs), unsigned int tabs);

#endif