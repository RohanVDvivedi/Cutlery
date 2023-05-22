#include<linkedlist.h>

#include<cutlery_stds.h>

// to check if a node is a free floating node
int is_free_floating_llnode(const llnode* node_p)
{
	return ((node_p->next == NULL) && (node_p->prev == NULL));
}

void initialize_linkedlist(linkedlist* ll, cy_uint node_offset)
{
	ll->node_offset = node_offset;
	ll->head = NULL;
}

void initialize_llnode(llnode* node_p)
{
	node_p->next = NULL;
	node_p->prev = NULL;
}

int is_empty_linkedlist(const linkedlist* ll)
{
	return (ll->head == NULL);
}

const void* get_head_of_linkedlist(const linkedlist* ll)
{
	return ((ll->head == NULL) ? NULL : get_data(ll->head, ll));
}

const void* get_tail_of_linkedlist(const linkedlist* ll)
{
	return ((ll->head == NULL) ? NULL : get_data(ll->head->prev, ll));
}

const void* get_nth_from_head_of_linkedlist(const linkedlist* ll, cy_uint n)
{
	if(is_empty_linkedlist(ll))
		return NULL;
	
	llnode* node_p = ll->head;
	do
	{
		if(n == 0)
			return get_data(node_p, ll);
		node_p = node_p->next;
		n--;
	}
	while(node_p != ll->head);

	// if not found in the loop return NULL
	return NULL;
}

const void* get_nth_from_tail_of_linkedlist(const linkedlist* ll, cy_uint n)
{
	if(is_empty_linkedlist(ll))
		return NULL;

	llnode* tail = ll->head->prev;

	llnode* node_p = tail;
	do
	{
		if(n == 0)
			return get_data(node_p, ll);
		node_p = node_p->prev;
		n--;
	}
	while(node_p != tail);

	// if not found in the loop return NULL
	return NULL;
}

const void* get_next_of_in_linkedlist(const linkedlist* ll, const void* data_xist)
{
	return get_data(((llnode*)(get_node(data_xist, ll)))->next, ll);
}

const void* get_prev_of_in_linkedlist(const linkedlist* ll, const void* data_xist)
{
	return get_data(((llnode*)(get_node(data_xist, ll)))->prev, ll);
}

// for the functions insert_node_*, node_p must be in ll linkedlist and new_node must not be part of any linkedlist (i.e. is_new_llnode(new_node) == 1)
static void insert_node_before(linkedlist* ll, llnode* node_p, llnode* new_node)
{
	// first update the new_node's prev and next references
	new_node->prev = node_p->prev;
	new_node->next = node_p;

	// then update the next of prev of new_node to itself and the prev of next of new_node to itself
	new_node->prev->next = new_node;
	new_node->next->prev = new_node;

	if(node_p == ll->head)
		ll->head = new_node;
}

static void insert_node_after(linkedlist* ll, llnode* node_p, llnode* new_node)
{
	// first update the new_node's prev and next references
	new_node->prev = node_p;
	new_node->next = node_p->next;

	// then update the next of prev of new_node to itself and the prev of next of new_node to itself
	new_node->prev->next = new_node;
	new_node->next->prev = new_node;
}

int insert_head_in_linkedlist(linkedlist* ll, const void* data_p)
{
	llnode* new_node = get_node(data_p, ll);

	if(!is_free_floating_llnode(new_node))	// insert only a free floating node, i.e. a node that does not exist in any linkedlist
		return 0;

	// case when the linkedlist is empty
	if(is_empty_linkedlist(ll))
	{
		ll->head = new_node;
		ll->head->next = new_node;
		ll->head->prev = new_node;
	}
	else
		insert_node_before(ll, ll->head, new_node);

	return 1;
}

int insert_tail_in_linkedlist(linkedlist* ll, const void* data_p)
{
	llnode* new_node = get_node(data_p, ll);

	if(!is_free_floating_llnode(new_node))	// insert only a free floating node, i.e. a node that does not exist in any linkedlist
		return 0;

	// case when the linkedlist is empty
	if(is_empty_linkedlist(ll))
	{
		ll->head = new_node;
		ll->head->next = new_node;
		ll->head->prev = new_node;
	}
	else
		insert_node_after(ll, ll->head->prev, new_node);

	return 1;
}

int insert_before_in_linkedlist(linkedlist* ll, const void* data_xist, const void* data)
{
	llnode* node_xist = get_node(data_xist, ll);
	llnode* new_node = get_node(data, ll);

	// insert only a free floating node, before a node that exists (i.e. that must not be a free floating node)
	if(is_free_floating_llnode(node_xist) || !is_free_floating_llnode(new_node))
		return 0;

	insert_node_before(ll, node_xist, new_node);

	return 1;
}

int insert_after_in_linkedlist(linkedlist* ll, const void* data_xist, const void* data)
{
	llnode* node_xist = get_node(data_xist, ll);
	llnode* new_node = get_node(data, ll);

	// insert only a free floating node, before a node that exists (i.e. that must not be a free floating node)
	if(is_free_floating_llnode(node_xist) || !is_free_floating_llnode(new_node))
		return 0;

	insert_node_after(ll, node_xist, new_node);

	return 1;
}

// for the functions insert_all_nodes_*, node_p must be in ll linkedlist and new_nodes_head must be the head of the doubly circular linkedlist (different linkedlist than the one provided i.e. different from ll)
static void insert_all_nodes_before(linkedlist* ll, llnode* node_p, llnode* new_nodes_head)
{
	llnode* new_nodes_tail = new_nodes_head->prev;

	// fix pointers of the new_nodes first
	new_nodes_head->prev = node_p->prev;
	new_nodes_tail->next = node_p;

	// self explanatory
	new_nodes_head->prev->next = new_nodes_head;
	new_nodes_tail->next->prev = new_nodes_tail;

	if(node_p == ll->head)
		ll->head = new_nodes_head;
}

static void insert_all_nodes_after(linkedlist* ll, llnode* node_p, llnode* new_nodes_head)
{
	llnode* new_nodes_tail = new_nodes_head->prev;

	// fix pointers of the new_nodes first
	new_nodes_head->prev = node_p;
	new_nodes_tail->next = node_p->next;

	// self explanatory
	new_nodes_head->prev->next = new_nodes_head;
	new_nodes_tail->next->prev = new_nodes_tail;
}

int insert_all_at_head_in_linkedlist(linkedlist* ll, linkedlist* insert_from_ll)
{
	// if the node_offsets are different OR if insert_from_ll is empty then we fail
	if(ll->node_offset != insert_from_ll->node_offset || is_empty_linkedlist(insert_from_ll) || ll == insert_from_ll)
		return 0;

	if(is_empty_linkedlist(ll))	// steal its head pointer
		ll->head = insert_from_ll->head;
	else
		insert_all_nodes_before(ll, ll->head, insert_from_ll->head);

	// mark the insert_from_ll linkedlist as empty
	insert_from_ll->head = NULL;

	return 1;
}

int insert_all_at_tail_in_linkedlist(linkedlist* ll, linkedlist* insert_from_ll)
{
	// if the node_offsets are different OR if insert_from_ll is empty OR if both the linkedlists are the same, then we fail
	if(ll->node_offset != insert_from_ll->node_offset || is_empty_linkedlist(insert_from_ll) || ll == insert_from_ll)
		return 0;

	if(is_empty_linkedlist(ll))	// steal its head pointer
		ll->head = insert_from_ll->head;
	else
		insert_all_nodes_after(ll, ll->head->prev, insert_from_ll->head);

	// mark the insert_from_ll linkedlist as empty
	insert_from_ll->head = NULL;

	return 1;
}

int insert_all_before_in_linkedlist(linkedlist* ll, const void* data_xist, linkedlist* insert_from_ll)
{
	// if the node_offsets are different OR if insert_from_ll is empty OR if both the linkedlists are the same, then we fail
	if(ll->node_offset != insert_from_ll->node_offset || is_empty_linkedlist(insert_from_ll) || ll == insert_from_ll)
		return 0;

	llnode* node_xist = get_node(data_xist, ll);

	// insert only before a node that exists (i.e. that is not a free floating node)
	if(is_free_floating_llnode(node_xist))
		return 0;

	insert_all_nodes_before(ll, node_xist, insert_from_ll->head);

	// mark the insert_from_ll linkedlist as empty
	insert_from_ll->head = NULL;

	return 1;
}

int insert_all_after_in_linkedlist(linkedlist* ll, const void* data_xist, linkedlist* insert_from_ll)
{
	// if the node_offsets are different OR if insert_from_ll is empty OR if both the linkedlists are the same, then we fail
	if(ll->node_offset != insert_from_ll->node_offset || is_empty_linkedlist(insert_from_ll) || ll == insert_from_ll)
		return 0;

	llnode* node_xist = get_node(data_xist, ll);

	// insert only after a node that exists (i.e. that is not a free floating node)
	if(is_free_floating_llnode(node_xist))
		return 0;

	insert_all_nodes_after(ll, node_xist, insert_from_ll->head);

	// mark the insert_from_ll linkedlist as empty
	insert_from_ll->head = NULL;

	return 1;
}

static void remove_node(linkedlist* ll, llnode* node_p)
{
	// first update next and prev nodes of node_p
	node_p->next->prev = node_p->prev;
	node_p->prev->next = node_p->next;

	// if node_p is the head node
	// update the head reference of the linkedlist to next of the node_p before deletion
	if(ll->head == node_p)
		ll->head = node_p->next;
	if(ll->head == node_p)	// if there is only one node, then the next of head is also head, hence make head NULL
		ll->head = NULL;

	node_p->next = NULL;
	node_p->prev = NULL;
}

int remove_head_from_linkedlist(linkedlist* ll)
{
	if(is_empty_linkedlist(ll))	// remove fails with 0, if linkedlist is empty
		return 0;
	llnode* node_p = ll->head;
	remove_node(ll, node_p);
	initialize_llnode(node_p);	// re-initialize the node as soon as it is removed
	return 1;
}

int remove_tail_from_linkedlist(linkedlist* ll)
{
	if(is_empty_linkedlist(ll))	// remove fails with 0, if linkedlist is empty
		return 0;
	llnode* node_p = ll->head->prev;
	remove_node(ll, node_p);
	initialize_llnode(node_p);	// re-initialize the node as soon as it is removed
	return 1;
}

// remove the given element from the linked list
int remove_from_linkedlist(linkedlist* ll, const void* data)
{
	llnode* node_p = get_node(data, ll);

	if(is_free_floating_llnode(node_p))	// a free floating node can not be removed
		return 0;

	remove_node(ll, node_p);
	initialize_llnode(node_p);	// re-initialize the node as soon as it is removed
	return 1;
}

void remove_all_from_linkedlist(linkedlist* ll)
{
	if(is_empty_linkedlist(ll))
		return;

	// initialize all nodes in linkedlist
	llnode* temp = ll->head;
	do
	{
		llnode* temp_next = temp->next;
		initialize_llnode(temp);
		temp = temp_next;
	}
	while(temp != ll->head);

	// set linkedlist head to NULL
	ll->head = NULL;
}

const void* find_equals_in_linkedlist(const linkedlist* ll, const void* data, int (*compare)(const void* data1, const void* data2))
{
	if(is_empty_linkedlist(ll))
		return NULL;
	const llnode* node_p = ll->head;
	do
	{
		const void* data_at_node = get_data(node_p, ll);
		if(compare(data_at_node, data) == 0)
			return data_at_node;
		node_p = node_p->next;
	}
	while(node_p != ll->head);
	return NULL;
}

void for_each_in_linkedlist(const linkedlist* ll, void (*operation)(const void* data_p, const void* additional_params), const void* additional_params)
{
	if(is_empty_linkedlist(ll))
		return;
	const llnode* node_p = ll->head;
	do
	{
		llnode* next = node_p->next;
		operation(get_data(node_p, ll), additional_params);
		node_p = next;
	}
	while(node_p != ll->head);
}

static void sprint_linkedlist_wrapper(dstring* append_str, const linkedlist* ll, const llnode* node_p, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs++); snprintf_dstring(append_str, "node : [%p]\n", node_p);
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "prev : [%p]\n", node_p->prev);
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "data :\n");
	sprint_element(append_str, get_data(node_p, ll), tabs + 1); snprintf_dstring(append_str, "\n");
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "next : [%p]\n\n", node_p->next);
}

void sprint_linkedlist(dstring* append_str, const linkedlist* ll, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs++); snprintf_dstring(append_str, "linkedlist :\n");
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "node_offset : [%u]\n", ll->node_offset);
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "head : [%p]\n", ll->head);
	if(!is_empty_linkedlist(ll))
	{
		const llnode* node_p = ll->head;
		do
		{
			sprint_linkedlist_wrapper(append_str, ll, node_p, sprint_element, tabs + 1);
			node_p = node_p->next;
		}
		while(node_p != ll->head);
	}
}