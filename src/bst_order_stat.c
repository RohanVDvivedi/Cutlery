#include<cutlery/bst_order_stat.h>

#include<cutlery/cutlery_node.h>

static cy_uint get_subtree_size_internal(const bst* bst_p, const bstnode* node_p)
{
	const void* data = get_data(node_p, bst_p);
	const order_stat_bstnode* osb = data + bst_p->order_stat_node_offset;
	return osb->subtree_size;
}

static void set_subtree_size_internal(const bst* bst_p, bstnode* node_p, cy_uint subtree_size)
{
	const void* data = get_data(node_p, bst_p);
	order_stat_bstnode* osb = (order_stat_bstnode*)(data + bst_p->order_stat_node_offset);
	osb->subtree_size = subtree_size;
}

cy_uint get_subtree_size(const bst* bst_p, const bstnode* node_p)
{
	if(!is_order_stat_bst(bst_p))
		return INVALID_INDEX;

	// a NULL node has a sub_tree_size of 0
	if(node_p == NULL)
		return 0;

	return get_subtree_size_internal(bst_p, node_p);
}

cy_uint recompute_subtree_size(const bst* bst_p, bstnode* node_p)
{
	if(!is_order_stat_bst(bst_p))
		return INVALID_INDEX;

	// a NULL node has a sub_tree_size of 0
	if(node_p == NULL)
		return 0;

	// force recompute the subtree size at the node_p
	set_subtree_size_internal(bst_p, node_p, 0);

	bstnode* curr_p = node_p;
	while(curr_p != NULL)
	{
		cy_uint subtree_size = get_subtree_size_internal(bst_p, curr_p);
		cy_uint computed_subtree_size = get_subtree_size_internal(bst_p, curr_p) + 1 + get_subtree_size_internal(bst_p, curr_p);

		if(subtree_size == computed_subtree_size)
			break;

		set_subtree_size_internal(bst_p, curr_p, computed_subtree_size);

		curr_p = curr_p->parent;
	}

	return get_subtree_size_internal(bst_p, node_p);
}