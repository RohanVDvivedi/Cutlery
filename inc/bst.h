#ifndef BST_H
#define BST_H

#include<dstring.h>

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
	// if bsttype == NON_SELF_BALANCING then node property is ignored
	// else if bsttype == AVL_TREE then node_property = max( right_tree->node_property, left_tree->node_property)
	// else if bsttype == RED_BLACK_TREE then node_property = is_red_node? ? 0 : 1; (1 is black node) 
	// it must be a signed integer, do not change this datatype
	int node_property;
};

struct bst
{
	// the type of balancing algorithm to use for the binary search tree
	bsttype type;

	// defines address to data with respect to bstnode
	// this is how we reach node addresses from provided user's structure data addresses and viceversa
	unsigned int node_offset;

	// the root node of the tree
	bstnode* root;

	// compares data
	// it returns 0 if they are same, >0 if data1 is greater than data2 else it must return <0 value
	int (*compare)(const void* data1, const void* data2);
};

/*
*	note : you can only insert a new node, and remove an existing node
*/

// initializes as if a new bst, may be to reuse
void initialize_bst(bst* bst_p, bsttype type, unsigned int node_offset, int (*compare)(const void* data1, const void* data2));

// always initialize your bstnode before using it
void initialize_bstnode(bstnode* node_p);

int is_empty_bst(const bst* bst_p);

/*
**		NOTE FOR USING ALL THE FIND FUNCTIONS BELOW
**
** find_* functions; return the data from the bst, that equals the data given as parameter "data",
** (when compared using the comparator function)
**
** the "data" you provide as parameter must have all the fields required for comparison 
** used in the given comparator function (as in "bst_p" struct).
** i.e. it must have all the fields/attributes of the struct that are used in "bst_p->compare(,)".
*/

// find data in bst_p (given in parameter), that compares equal to data (given in parameter)
// returns NULL, if not found
const void* find_equals_in_bst(const bst* bst_p, const void* data);

// find data in bst_p (given in parameter), with the biggest value which is smaller than data (given in parameter)
// returns NULL, if data (given in parameter) is smaller than or equal to the smallest existing data in bst_p (given in parameter)
const void* find_preceding(const bst* bst_p, const void* data);

// find data in bst_p (given in parameter), with the biggest value which is smaller than or equal to data (given in parameter)
// returns NULL, if data (given in parameter) is smaller than the smallest existing data in bst_p (given in parameter)
const void* find_preceding_or_equals(const bst* bst_p, const void* data);

// find data in bst_p (given in parameter), with the smallest value which is bigger than data (given in parameter)
// returns NULL, if data (given in parameter) is bigger than or equla to the biggest existing data in bst_p (given in parameter)
const void* find_succeeding(const bst* bst_p, const void* data);

// find data in bst_p (given in parameter), with the smallest value which is bigger than or equal to data (given in parameter)
// returns NULL, if data (given in parameter) is bigger than the biggest existing data in bst_p (given in parameter)
const void* find_succeeding_or_equals(const bst* bst_p, const void* data);

// find data in bst_p (given in parameter), with the smallest value
const void* find_smallest(const bst* bst_p);

// find data in bst_p (given in parameter), with the biggest value
const void* find_largest(const bst* bst_p);

typedef enum sort_direction sort_direction;
enum sort_direction{ASCENDING_ORDERED, DESCENDING_ORDERED};
// find all (or atleast max_result_count number of) data pointers in the bst,
// which compare >= lower_bound and <= upper_bound,
// and accumulate them in the sort_direction order (either ASCENDING or DESCENDING order).
unsigned int find_all_in_range(	
						const bst* bst_p,				// binary search tree to search in
						
						const void* lower_bound,		// if(lower_bound != NULL), then find all data >= lower_bound, else find all ignoring the lower_bound
						const void* upper_bound,		// if(upper_bound != NULL), then find all data <= upper_bound, else find all ignoring the upper_bound

						sort_direction sort_dirctn,		// if sort_dirctn == ASCENDING_ORDERED,
														// 	then the result set is ordered from least to highest,
														//	stopping only if max_result_count condition is reached
														// vice-versa for DESCENDING_ORDERED
						
						unsigned int max_result_count,	// this is the maximum in-range results to find.
							// TO GET ALL THE RESULTS => max_result_count = 0xffffffff (maximum unsigned int)

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

// This function returns true(1),
// if a bstnode is insertable in the given bst bst_p
// i.e. left, right and parent pointers are NULL and it is not at the root of bst_p bst
// please try and avoid using this function in user application
int is_new_bstnode(const bst* bst_p, const bstnode* node_p);

// traversals possible in the tree
typedef enum bsttraversal bsttraversal;
enum bsttraversal{PRE_ORDER, IN_ORDER, POST_ORDER};

// perform given operation on all the elements of the binary search tree
void for_each_in_bst(const bst* bst_p, bsttraversal traversal, void (*operation)(const void* data, const void* additional_params), const void* additional_params);

// print complete binary search tree
void sprint_bst(dstring* append_str, const bst* bst_p, void (*sprint_element)(dstring* append_str, const void* data, unsigned int tabs), unsigned int tabs);

#endif