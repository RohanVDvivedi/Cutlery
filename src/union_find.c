#include<union_find.h>

#include<cutlery_stds.h>
#include<cutlery_math.h>

void initialize_union_find(union_find* uf_p, unsigned int node_offset)
{
	uf_p->node_offset = node_offset;
}

void initialize_ufnode(ufnode* node_p)
{
	node_p->parent = node_p; // make it point to self
	node_p->rank = 0;
}

static int is_root(const ufnode* node_p)
{
	return (node_p->parent == node_p);
}

static ufnode* get_root(ufnode* node_p)
{
	// find root node for the node_p
	ufnode* root_p = node_p;
	while(!is_root(root_p))
		root_p = root_p->parent;
	
	// make all the nodes from node_p to the root_p (except root_p) to point to root_p
	ufnode* t = node_p;
	while(t != root_p)
	{
		ufnode* t_next = t->parent;
		t->parent = root_p;
		t = t_next;
	}

	return root_p;
}

const void* find_root_in_union_find(const union_find* uf_p, const void* data)
{
	ufnode* node_p = get_node(data, uf_p);
	ufnode* root_p = get_root(node_p);
	return get_data(root_p, uf_p);
}

int are_in_same_group(const union_find* uf_p, const void* data1, const void* data2)
{
	ufnode* node1_p = get_node(data1, uf_p);
	ufnode* node2_p = get_node(data2, uf_p);

	ufnode* root1_p = get_root(node1_p);
	ufnode* root2_p = get_root(node2_p);

	// make sure that both the nodes point to the same root
	return root1_p == root2_p;
}

int merge_groups_in_union_find(const union_find* uf_p, const void* data1, const void* data2)
{
	ufnode* node1_p = get_node(data1, uf_p);
	ufnode* node2_p = get_node(data2, uf_p);

	ufnode* root1_p = get_root(node1_p);
	ufnode* root2_p = get_root(node2_p);

	if(root1_p == root2_p)
		return 0;

	if(root1_p->rank >= root2_p->rank)
	{
		root2_p->parent = root1_p;
		node2_p->parent = root1_p;
		root1_p->rank = max(root1_p->rank, root2_p->rank + 1);
	}
	else
	{
		root1_p->parent = root2_p;
		node1_p->parent = root2_p;
		root2_p->rank = max(root2_p->rank, root1_p->rank + 1);
	}

	return 1;
}