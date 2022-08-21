#ifndef UNION_FIND_H
#define UNION_FIND_H

typedef struct ufnode ufnode;
struct ufnode
{
	ufnode* parent; // == NULL, if it is the root of the group, else it points to its parent

	unsigned int rank;
};

typedef struct union_find union_find;
struct union_find
{
	// defines address to data with respect to uf_node
	// this is how we reach node addresses from provided user's structure data addresses and viceversa
	unsigned int node_offset;
};

// initializes a new bst
void initialize_union_find(union_find* uf_p, unsigned int node_offset);

// always initialize your ufnode before using it
void initialize_ufnode(ufnode* node_p);

const void* find_root_in_union_find(const union_find* uf_p, const void* data);

// return 0, (i.e. fails) if data1 and data2 are already in the same group
int merge_groups_in_union_find(const union_find* uf_p, const void* data1, const void* data2);

#endif