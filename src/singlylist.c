#include<singlylist.h>

#include<cutlery_node.h>
#include<cutlery_stds.h>

void initialize_singlylist(singlylist* sl, cy_uint node_offset)
{
	sl->node_offset = node_offset;
	sl->head = NULL;
	sl->tail = NULL;
}

void initialize_slnode(slnode* node_p)
{
	node_p->next = NULL;
}

int is_free_floating_slnode(const slnode* node_p)
{
	return node_p->next == NULL;
}

int is_empty_singlylist(const singlylist* sl)
{
	return sl->head == NULL;
}

const void* get_head_of_singlylist(const singlylist* sl)
{
	return (sl->head == NULL) ? NULL : get_data(sl->head, sl);
}

const void* get_tail_of_singlylist(const singlylist* sl)
{
	return (sl->tail == NULL) ? NULL : get_data(sl->tail, sl);
}

const void* get_from_head_of_singlylist(const singlylist* sl, cy_uint index)
{
	if(is_empty_singlylist(sl))
		return NULL;

	const slnode* node_p = sl->head;
	do
	{
		if(index == 0)
			return get_data(node_p, sl);
		node_p = node_p->next;
		index--;
	}
	while(node_p == sl->head);

	// if not found in the loop return NULL
	return NULL;
}

const void* get_next_of_in_singlylist(const singlylist* sl, const void* data_xist)
{
	const slnode* node_xist = get_node(data_xist, sl);

	// the data_xist must not be a free floating node
	if(is_free_floating_slnode(node_xist))
		return NULL;

	return get_data(node_xist->next, sl);
}

int insert_head_in_singlylist(singlylist* sl, const void* data)
{
	slnode* node_p = get_node(data, sl);

	// insert only a free floating slnode to the singlylist
	if(!is_free_floating_slnode(node_p))
		return 0;

	if(is_empty_singlylist(sl))
	{
		sl->head = node_p;
		sl->tail = node_p;
		node_p->next = node_p;
	}
	else
	{
		node_p->next = sl->head;
		sl->tail->next = node_p;
		sl->head = node_p;
	}

	return 1;
}

int insert_tail_in_singlylist(singlylist* sl, const void* data)
{
	slnode* node_p = get_node(data, sl);

	// insert only a free floating node to the singlylist
	if(!is_free_floating_slnode(node_p))
		return 0;

	if(is_empty_singlylist(sl))
	{
		sl->head = node_p;
		sl->tail = node_p;
		node_p->next = node_p;
	}
	else
	{
		node_p->next = sl->tail->next;
		sl->tail->next = node_p;
		sl->tail = node_p;
	}

	return 1;
}

int insert_after_in_singlylist(singlylist* sl, const void* data_xist, const void* data)
{
	slnode* node_xist = get_node(data_xist, sl);
	slnode* node_p = get_node(data, sl);

	// node_xist must not be a free floating node and node_p must be a free floating node
	if(is_free_floating_slnode(node_xist) || !is_free_floating_slnode(node_p))
		return 0;

	// insert the new node in the singlylist after the node_xist
	node_p->next = node_xist->next;
	node_xist->next = node_p;

	// if node_xist was the tail node then, now node_p is the tail node
	if(sl->tail == node_xist)
		sl->tail = node_p;

	return 1;
}

int insert_all_at_head_in_singlylist(singlylist* sl, singlylist* insert_from_sl)
{
	// if the node_offsets are different OR if insert_from_sl is same as sl, then we fail
	if(sl->node_offset != insert_from_sl->node_offset || sl == insert_from_sl)
		return 0;

	// nothing to be inserted
	if(is_empty_singlylist(insert_from_sl))
		return 1;

	if(is_empty_singlylist(sl)) // if sl is empty copy as it is
		*sl = *insert_from_sl;
	else
	{
		insert_from_sl->tail->next = sl->head;
		sl->tail->next = insert_from_sl->head;
		sl->head = insert_from_sl->head;
	}

	insert_from_sl->head = NULL;
	insert_from_sl->tail = NULL;

	return 1;
}

int insert_all_at_tail_in_singlylist(singlylist* sl, singlylist* insert_from_sl)
{
	// if the node_offsets are different OR if insert_from_sl is same as sl, then we fail
	if(sl->node_offset != insert_from_sl->node_offset || sl == insert_from_sl)
		return 0;

	// nothing to be inserted
	if(is_empty_singlylist(insert_from_sl))
		return 1;

	if(is_empty_singlylist(sl)) // if sl is empty copy as it is
		*sl = *insert_from_sl;
	else
	{
		insert_from_sl->tail->next = sl->head;
		sl->tail->next = insert_from_sl->head;
		sl->tail = insert_from_sl->tail;
	}

	insert_from_sl->head = NULL;
	insert_from_sl->tail = NULL;

	return 1;
}

int insert_all_after_in_singlylist(singlylist* sl, const void* data_xist, singlylist* insert_from_sl)
{
	// if the node_offsets are different OR if insert_from_sl is same as sl, then we fail
	if(sl->node_offset != insert_from_sl->node_offset || sl == insert_from_sl)
		return 0;

	// nothing to be inserted
	if(is_empty_singlylist(insert_from_sl))
		return 1;

	slnode* node_xist = get_node(data_xist, sl);

	// insert only after a node that may exists in the singlylist (i.e. is not free floating)
	if(is_free_floating_slnode(node_xist))
		return 0;

	insert_from_sl->tail->next = node_xist->next;
	node_xist->next = insert_from_sl->head;

	// if inserting after a tail, then update tail
	if(sl->tail == node_xist)
		sl->tail = insert_from_sl->tail;

	insert_from_sl->head = NULL;
	insert_from_sl->tail = NULL;

	return 1;
}

int remove_head_from_singlylist(singlylist* sl)
{
	if(is_empty_singlylist(sl))
		return 0;

	slnode* to_be_removed = sl->head;

	// there is only 1 node in the linkedlist, i.e the head and we are removing it
	if(sl->head == sl->tail)
	{
		sl->head = NULL;
		sl->tail = NULL;
	}
	else
	{
		sl->tail->next = sl->head->next;
		sl->head = sl->head->next;
	}

	initialize_slnode(to_be_removed);

	return 1;
}

const void* remove_next_of_from_singlylist(singlylist* sl, const void* data_xist)
{
	if(is_empty_singlylist(sl))
		return NULL;

	slnode* node_xist = get_node(data_xist, sl);

	// node_xist must not be a free floating node
	if(is_free_floating_slnode(node_xist))
		return NULL;

	slnode* to_be_removed = node_xist->next;

	if(sl->head == sl->tail) // there is only 1 element in singlylist and it better be node_xist
	{
		if(sl->head != node_xist) // the node_xist provided is not in the sl, we fail
			return NULL;

		// since there is only 1 element in singlylist
		// and we are require to remove the next of it, which must be itself

		// so here just make the sl empty
		sl->head = NULL;
		sl->tail = NULL;
	}
	else
	{
		// unlink to be to_be_removed from the chain
		node_xist->next = to_be_removed->next;

		// if to be removed was the tail, then
		// removing it makes node_xist (which is rightly the previous to it) the tail node
		if(sl->tail == to_be_removed)
			sl->tail = node_xist;
	}

	initialize_slnode(to_be_removed);

	return get_data(to_be_removed, sl);
}

void remove_all_from_singlylist(singlylist* sl)
{
	// nothing to remove
	if(is_empty_singlylist(sl))
		return;

	slnode * const HEAD = sl->head;

	slnode* node_p = sl->head;
	do
	{
		slnode* node_p_next = node_p->next;
		initialize_slnode(node_p);
		node_p = node_p_next;
	}
	while(node_p != HEAD);
	
	sl->head = NULL;
	sl->tail = NULL;
}

const void* find_equals_in_singlylist(const singlylist* sl, const void* data, int (*compare)(const void* data1, const void* data2))
{
	if(is_empty_singlylist(sl))
		return NULL;

	slnode const * const HEAD = sl->head;

	const slnode* node_p = sl->head;
	do
	{
		if(compare(data, get_data(node_p, sl)) == 0)
			return get_data(node_p, sl);
		node_p = node_p->next;
	}
	while(node_p != HEAD);

	return NULL;
}

void radix_sort_singlylist(singlylist* sl, unsigned long long int (*get_sort_attribute)(const void* data))
{
	// return, if the singlylist has 0 or 1 nodes only
	if(is_empty_singlylist(sl) || get_head_of_singlylist(sl) == get_tail_of_singlylist(sl))
		return;

	singlylist sort_buckets[2];
	initialize_singlylist(&(sort_buckets[0]), sl->node_offset);
	initialize_singlylist(&(sort_buckets[1]), sl->node_offset);

	for(unsigned int i = 0; i < sizeof(unsigned long long int) * CHAR_BIT; i++)
	{
		// sort all elements of ll, based on the ith bit of the sort attribute
		while(!is_empty_singlylist(sl))
		{
			const void* data_p = get_head_of_singlylist(sl);
			unsigned int sort_buckets_index = (get_sort_attribute(data_p) >> i) & 1ULL;
			remove_head_from_singlylist(sl);
			insert_tail_in_singlylist(&(sort_buckets[sort_buckets_index]), data_p);
		}

		// concatenate both the sort buckets, one after the other in order
		insert_all_at_tail_in_singlylist(sl, &(sort_buckets[0]));
		insert_all_at_tail_in_singlylist(sl, &(sort_buckets[1]));
	}
}

// TODO from here --

static void swap_nodes_singlylist(slnode** node1_p_p, slnode** node2_p_p)
{
	if((*node1_p_p) == (*node2_p_p))
		return;

	slnode* temp;

	temp = (*node1_p_p);
	(*node1_p_p) = (*node2_p_p);
	(*node2_p_p) = temp;

	temp = (*node1_p_p)->next;
	(*node1_p_p)->next = (*node2_p_p)->next;
	(*node2_p_p)->next = temp;
}

void bubble_sort_singlylist(singlylist* sl, int (*compare)(const void* data1, const void* data2))
{
	// return, if the singlylist has 0 or 1 nodes only
	if(is_empty_singlylist(sl) || get_head_of_singlylist(sl) == get_tail_of_singlylist(sl))
		return;

	// perform loops for bubble sort as long as there are any swaps in previous iteration
	cy_uint swaps_performed = 0;
	do
	{
		swaps_performed = 0;

		slnode** curr = &(sl->head);
		slnode** next = &((*curr)->next);
		while((*next))
		{
			if(compare(get_data(*curr, sl), get_data(*next, sl)) > 0)
			{
				swaps_performed++;
				swap_nodes_singlylist(curr, next);
				curr = &((*curr)->next);
			}
			else
				curr = next;
			next = &((*curr)->next);
		}

		sl->tail = *curr;
	}while(swaps_performed);
}

// TODO from here -end-

void for_each_in_singlylist(const singlylist* sl, void (*operation)(const void* data_p, const void* additional_params), const void* additional_params)
{
	// no element to iterate over
	if(is_empty_singlylist(sl))
		return;

	slnode const * const HEAD = sl->head;
	const slnode* node_p = sl->head;
	do
	{
		operation(get_data(node_p, sl), additional_params);
		node_p = node_p->next;
	}
	while(node_p != HEAD);
}

// TODO from here --

static void sprint_singlylist_wrapper(dstring* append_str, const singlylist* sl, const slnode* node_p, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs++); snprintf_dstring(append_str, "node : [%p]\n", node_p);
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "data :\n");
	sprint_element(append_str, get_data(node_p, sl), tabs + 1); snprintf_dstring(append_str, "\n");
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "next : [%p]\n\n", node_p->next);
}

void sprint_singlylist(dstring* append_str, const singlylist* sl, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs++); snprintf_dstring(append_str, "singlylist :\n");
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "node_offset : [%" PRIu_cy_uint "]\n", sl->node_offset);
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "head : [%p]\n", sl->head);
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "tail : [%p]\n", sl->tail);
	if(!is_empty_singlylist(sl))
	{
		const slnode* node_p = sl->head;
		while(node_p != NULL)
		{
			sprint_singlylist_wrapper(append_str, sl, node_p, sprint_element, tabs + 1);
			node_p = node_p->next;
		}
	}
}