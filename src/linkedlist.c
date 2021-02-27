#include<linkedlist.h>

#include<stdio.h>

// get data from llnode
#define get_data(node_p) 	(((const void*)(node_p)) - ll->node_offset)
// get llnode from data
#define get_node(data_p) 	(((void*)(data_p)) + ll->node_offset)

// utility
// to check if a node is new
int is_new_llnode(const linkedlist* ll, const llnode* node_p)
{
	return ((node_p->next == NULL) && (node_p->prev == NULL) && (ll->head != node_p));
}

void initialize_linkedlist(linkedlist* ll, unsigned int node_offset)
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

const void* get_head(const linkedlist* ll)
{
	return ((ll->head == NULL) ? NULL : get_data(ll->head));
}

const void* get_tail(const linkedlist* ll)
{
	return ((ll->head == NULL) ? NULL : get_data(ll->head->prev));
}

const void* get_nth_from_head(const linkedlist* ll, unsigned int n)
{
	if(is_empty_linkedlist(ll))
		return NULL;
	llnode* head = ll->head;
	if(n == 0)
		return head;
	llnode* node_p = head;
	do
	{
		n--;
		node_p = node_p->next;
	}
	while(node_p != head && n > 0);
	return (node_p == head) ? NULL : get_data(node_p);
}

const void* get_nth_from_tail(const linkedlist* ll, unsigned int n)
{
	if(is_empty_linkedlist(ll))
		return NULL;
	llnode* tail = ll->head->prev;
	if(n == 0)
		return tail;
	llnode* node_p = tail;
	do
	{
		n--;
		node_p = node_p->prev;
	}
	while(node_p != tail && n > 0);
	return (node_p == tail) ? NULL : get_data(node_p);
}

const void* get_next_of(const linkedlist* ll, const void* data_xist)
{
	return get_data(((llnode*)(get_node(data_xist)))->next);
}

const void* get_prev_of(const linkedlist* ll, const void* data_xist)
{
	return get_data(((llnode*)(get_node(data_xist)))->prev);
}

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

int insert_head(linkedlist* ll, const void* data_p)
{
	llnode* new_node = get_node(data_p);

	if(!is_new_llnode(ll, new_node))	// insert only a new node
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

int insert_tail(linkedlist* ll, const void* data_p)
{
	llnode* new_node = get_node(data_p);

	if(!is_new_llnode(ll, new_node))	// insert only a new node
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

int insert_before(linkedlist* ll, const void* data_xist, const void* data)
{
	llnode* node_xist = get_node(data_xist);
	llnode* new_node = get_node(data);

	// insert only a new node, before a node that exists
	if(is_new_llnode(ll, node_xist) || !is_new_llnode(ll, new_node))
		return 0;

	insert_node_before(ll, node_xist, new_node);

	return 1;
}

int insert_after(linkedlist* ll, const void* data_xist, const void* data)
{
	llnode* node_xist = get_node(data_xist);
	llnode* new_node = get_node(data);

	// insert only a new node, after a node that exists
	if(is_new_llnode(ll, node_xist) || !is_new_llnode(ll, new_node))
		return 0;

	insert_node_after(ll, node_xist, new_node);

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

int remove_head(linkedlist* ll)
{
	if(is_empty_linkedlist(ll))	// remove fails with 0, if linkedlist is empty
		return 0;
	llnode* node_p = ll->head;
	remove_node(ll, node_p);
	initialize_llnode(node_p);	// re-initialize the node as soon as it is removed
	return 1;
}

int remove_tail(linkedlist* ll)
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
	llnode* node_p = get_node(data);

	if(is_new_llnode(ll, node_p))	// a new node does not need to be removed
		return 0;

	remove_node(ll, node_p);
	initialize_llnode(node_p);	// re-initialize the node as soon as it is removed
	return 1;
}

const void* find_equals_in_linkedlist(const linkedlist* ll, const void* data, int (*compare)(const void* ll_data, const void* data))
{
	if(is_empty_linkedlist(ll))
		return NULL;
	llnode* node_p = ll->head;
	do
	{
		const void* data_at_node = get_data(node_p);
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
	llnode* node_p = ll->head;
	do
	{
		llnode* next = node_p->next;
		operation(get_data(node_p), additional_params);
		node_p = next;
	}
	while(node_p != ll->head);
}

static void print_linkedlist_wrapper(const linkedlist* ll, const llnode* node_p, void (*print_element)(const void* data_p))
{
	printf("\tprev => %p\n", node_p->prev);
	printf("\t\tnode => %p\n", node_p);
	printf("\t\tdata => ");print_element(get_data(node_p));printf("\n");
	printf("\tnext => %p\n", node_p->next);
	printf("\n");
}

void print_linkedlist(const linkedlist* ll, void (*print_element)(const void* data_p))
{
	printf("LINKED LIST\n");
	printf("node_offset : [%u]\n", ll->node_offset);
	printf("head : [%p]\n", ll->head);
	if(!is_empty_linkedlist(ll))
	{
		llnode* node_p = ll->head;
		do
		{
			print_linkedlist_wrapper(ll, node_p, print_element);
			node_p = node_p->next;
		}
		while(node_p != ll->head);
	}
	printf("--\n\n\n");
}