#ifndef BALANCEDBST_H
#define BALANCEDBST_H

typedef enum tree_type tree_type;
enum tree_type
{
	AVL_TREE,
	RED_BLACK_TREE
};

typedef struct node node;
struct node
{
	// the pointer to the parent node
	node* parent;

	// the data to store
	const void* data_p;

	// there can be only two child nodes,
	// we store here pointer to both of them here
	node* child_nodes[2];
};

typedef struct balancedbst balancedbst;
struct balancedbst
{
	// the type of balancing algorithm to use
	const tree_type balanced_tree_type;

	// compare data pointers that we stored
	// it returns 0 if they are same, 1 if data0 is greater than data1 else it must return -1
	int (*data_compare)(const void* data0_p, const void* data1_p);

	// the root node of the tree
	node* root;
};

// get a new balancedbst
balancedbst* get_balancedbst();

// inserts a node in the tree whose data is data_p
void insert(balancedbst* blanacedbst_p, const void* data_p);

// find a data_p in tree
const void* find(balancedbst* blanacedbst_p, const void* data_p);

// remove the node if found, returns a list of nodes that match
void remove(balancedbst* blanacedbst_p, const void* data_p);

// delete the balancedbst and all its nodes
void delete_balancedbst(balancedbst* balancedbst_p);

#endif