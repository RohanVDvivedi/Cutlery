#include<singlylist.h>

#include<cutlery_stds.h>

void initialize_singlylist(singlylist* sl, unsigned int node_offset)
{
	sl->node_offset = node_offset;
	sl->head = NULL;
	sl->tail = NULL;
}

void initialize_slnode(slnode* node_p)
{
	node_p->next = NULL;
}

static int is_new_slnode(const singlylist* sl, const slnode* node_p)
{
	return node_p->next == NULL && sl->tail != node_p;
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

const void* get_nth_from_head_of_singlylist(const singlylist* sl, unsigned int n)
{
	const slnode* node_p = sl->head;
	while(node_p != NULL && n > 0)
	{
		node_p = node_p->next;
		n--;
	}
	return (node_p == NULL) ? NULL : get_data(node_p, sl);
}

const void* get_next_of_in_singlylist(const singlylist* sl, const void* data_xist)
{
	const slnode* node_xist = get_node(data_xist, sl);

	// the data_xist must not be a new node
	if(is_new_slnode(sl, node_xist))
		return NULL;
	
	return (node_xist->next == NULL) ? NULL : get_data(node_xist, sl);
}

int insert_head_in_singlylist(singlylist* sl, const void* data)
{
	slnode* node_p = get_node(data, sl);

	// insert only a new node to the singlylist
	if(!is_new_slnode(sl, node_p))
		return 0;

	if(is_empty_singlylist(sl))
	{
		sl->head = node_p;
		sl->tail = node_p;
	}
	else
	{
		node_p->next = sl->head;
		sl->head = node_p;
	}

	return 1;
}

int insert_tail_in_singlylist(singlylist* sl, const void* data)
{
	slnode* node_p = get_node(data, sl);

	// insert only a new node to the singlylist
	if(!is_new_slnode(sl, node_p))
		return 0;

	if(is_empty_singlylist(sl))
	{
		sl->head = node_p;
		sl->tail = node_p;
	}
	else
	{
		sl->tail->next = node_p;
		sl->tail = node_p;
	}

	return 1;
}

int insert_after_in_singlylist(singlylist* sl, const void* data_xist, const void* data)
{
	slnode* node_xist = get_node(data_xist, sl);
	slnode* node_p = get_node(data, sl);

	// node_xist must not be a new node and node_p must be a new node
	if(is_new_slnode(sl, node_xist) || !is_new_slnode(sl, node_p))
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
	// if the node_offsets are different OR if insert_from_sl is empty then we fail
	if(sl->node_offset != insert_from_sl->node_offset || is_empty_singlylist(insert_from_sl) || sl == insert_from_sl)
		return 0;

	if(is_empty_singlylist(sl))
	{
		sl->head = insert_from_sl->head;
		sl->tail = insert_from_sl->tail;
	}
	else
	{
		insert_from_sl->tail->next = sl->head;
		sl->head = insert_from_sl->head;
	}

	insert_from_sl->head = NULL;
	insert_from_sl->tail = NULL;

	return 1;
}

int insert_all_at_tail_in_singlylist(singlylist* sl, singlylist* insert_from_sl)
{
	// if the node_offsets are different OR if insert_from_sl is empty then we fail
	if(sl->node_offset != insert_from_sl->node_offset || is_empty_singlylist(insert_from_sl) || sl == insert_from_sl)
		return 0;

	if(is_empty_singlylist(sl))
	{
		sl->head = insert_from_sl->head;
		sl->tail = insert_from_sl->tail;
	}
	else
	{
		sl->tail->next = insert_from_sl->head;
		sl->tail = insert_from_sl->tail;
	}

	insert_from_sl->head = NULL;
	insert_from_sl->tail = NULL;

	return 1;
}

int insert_all_after_in_singlylist(singlylist* sl, const void* data_xist, singlylist* insert_from_sl)
{
	// if the node_offsets are different OR if insert_from_sl is empty then we fail
	if(sl->node_offset != insert_from_sl->node_offset || is_empty_singlylist(insert_from_sl) || sl == insert_from_sl)
		return 0;

	slnode* node_xist = get_node(data_xist, sl);

	// insert only after a node that exists
	if(is_new_slnode(sl, node_xist))
		return 0;

	insert_from_sl->tail->next = node_xist->next;
	node_xist->next = insert_from_sl->head;

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

	sl->head = to_be_removed->next;
	if(sl->head == NULL)
		sl->tail = NULL;

	initialize_slnode(to_be_removed);

	return 1;
}

const void* remove_next_of_from_singlylist(singlylist* sl, const void* data_xist)
{
	if(is_empty_singlylist(sl))
		return NULL;

	slnode* node_xist = get_node(data_xist, sl);

	// node_xist must not be a new node
	if(is_new_slnode(sl, node_xist))
		return NULL;

	slnode* to_be_removed = node_xist->next;

	node_xist->next = to_be_removed->next;

	// if the node to be removed was a tail node then update the tail node to the node_xist
	if(sl->tail == to_be_removed)
		sl->tail = node_xist;

	initialize_slnode(to_be_removed);

	return get_data(to_be_removed, sl);
}

void remove_all_from_singlylist(singlylist* sl)
{
	slnode* node_p = sl->head;
	while(node_p != NULL)
	{
		slnode* node_p_next = node_p->next;
		initialize_slnode(node_p);
		node_p = node_p_next;
	}
	
	sl->head = NULL;
	sl->tail = NULL;
}

const void* find_equals_in_singlylist(const singlylist* sl, const void* data, int (*compare)(const void* data1, const void* data2))
{
	const slnode* node_p = sl->head;
	while(node_p != NULL)
	{
		if(compare(data, get_data(node_p, sl)) == 0)
			return get_data(node_p, sl);
		node_p = node_p->next;
	}
	return NULL;
}

void for_each_in_singlylist(const singlylist* sl, void (*operation)(const void* data_p, const void* additional_params), const void* additional_params)
{
	const slnode* node_p = sl->head;
	while(node_p != NULL)
	{
		operation(get_data(node_p, sl), additional_params);
		node_p = node_p->next;
	}
}

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
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "node_offset : [%u]\n", sl->node_offset);
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