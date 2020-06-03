#include<linkedlist.h>

void initialize_linkedlist(linkedlist* ll, unsigned long long int node_offset, int (*compare)(const void* data1, const void* data2))
{
	ll->head = NULL;
	ll->tail = NULL;
	ll->node_count = 0;
	ll->node_offset = node_offset;
	ll->compare = compare;
}

#define get_data(node_p) 	(((void*)(node_p)) - ll->node_offset)

#define get_node(data_p) 	(((void*)(data_p)) + ll->node_offset)

const void* get_head_data(linkedlist* ll)
{
	return ((ll->head == NULL) ? NULL : get_data(ll->head));
}

const void* get_tail_data(linkedlist* ll)
{
	return ((ll->tail == NULL) ? NULL : get_data(ll->tail));
}

static void insert_node_before(linkedlist* ll, llnode* node_p, llnode* new_node)
{	
	// if there is no node before node_p
	// we are basically adding a node to the head
	if(node_p->prev == NULL)
	{
		// the prev of new node is NULL
		// and its next is node_p
		// since it is going to be the last
		new_node->prev = NULL;
		new_node->next = node_p;

		// the new node is before node_p (next of the new_node)
		new_node->next->prev = new_node;

		// since this is the new head node
		// we update the reference to the head node in the linkedlist struct
		ll->head = new_node;
	}
	else
	{
		// first update the new_node's prev and next references
		new_node->prev = node_p->prev;
		new_node->next = node_p;

		// then update the next of prev of new_node to itself and the prev of next of new_node to itself
		new_node->prev->next = new_node;
		new_node->next->prev = new_node;
	}

	ll->node_count++;
}

static void insert_node_after(linkedlist* ll, llnode* node_p, llnode* new_node)
{
	// if there is no node after node_p
	// we are basically adding a node to the tail
	if(node_p->next == NULL)
	{
		// the prev of new node is node_p
		// and its next is NULL
		// since it is going to be the last
		new_node->prev = node_p;
		new_node->next = NULL;

		// the new node is after node_p (prev of new_node)
		new_node->prev->next = new_node;

		// since this is the new tail node
		// we update the reference to the tail node in the linkedlist struct
		ll->tail = new_node;
	}
	else
	{
		// first update the new_node's prev and next references
		new_node->prev = node_p;
		new_node->next = node_p->next;

		// then update the next of prev of new_node to itself and the prev of next of new_node to itself
		new_node->prev->next = new_node;
		new_node->next->prev = new_node;
	}

	ll->node_count++;
}

int insert_head(linkedlist* ll, const void* data_p)
{
	llnode* new_node = get_node(data_p);

	if(!is_new_node(new_node))
	{
		return 0;
	}

	// case when the linkedlist is empty
	if(ll->head == NULL)
	{
		ll->head = new_node;
		ll->tail = ll->head;
		ll->node_count++;
	}
	else
	{
		insert_node_before(ll, ll->head, new_node);
	}

	return 1;
}

int insert_tail(linkedlist* ll, const void* data_p)
{
	llnode* new_node = get_node(data_p);

	if(!is_new_node(new_node))
	{
		return 0;
	}

	// case when the linkedlist is empty
	if(ll->tail == NULL)
	{
		ll->tail = new_node;
		ll->head = ll->tail;
		ll->node_count++;
	}
	else
	{
		insert_node_after(ll, ll->tail, new_node);
	}

	return 1;
}

int insert_before(linkedlist* ll, const void* data_xist, const void* data)
{
	llnode* node_xist = get_node(data_xist);
	llnode* new_node = get_node(data);

	if((!is_new_node(new_node)) || is_new_node(node_xist))
	{
		return 0;
	}

	insert_node_before(ll, node_xist, new_node);
	return 1;
}

int insert_after(linkedlist* ll, const void* data_xist, const void* data)
{
	llnode* node_xist = get_node(data_xist);
	llnode* new_node = get_node(data);

	if((!is_new_node(new_node)) || is_new_node(node_xist))
	{
		return 0;
	}

	insert_node_after(ll, node_xist, new_node);
	return 1;
}

static void remove_node(linkedlist* ll, llnode* node_p)
{
	// if the node to be removed is not the last node
	// we have to update the prev pointer of "the node next to node_p" to the "previous node of node_p" 
	if(node_p->next != NULL)
	{
		node_p->next->prev = node_p->prev;
	}

	// if the node to be removed is not the first node
	// we have to update the next pointer of "the node previous to node_p" to the "next node of node_p" 
	if(node_p->prev != NULL)
	{
		node_p->prev->next = node_p->next;
	}

	// if node_p is the head node
	// update the head reference of the linkedlist to next of the node_p before deletion
	if(node_p == ll->head)
	{
		ll->head = node_p->next;
	}

	// if node_p is the head node
	// update the tail reference of the linkedlist to prev of the node_p before deletion
	if(node_p == ll->tail)
	{
		ll->tail = node_p->prev;
	}

	node_p->next = NULL;
	node_p->prev = NULL;

	ll->node_count--;
}

int remove_head(linkedlist* ll)
{
	// if there is a head node remove the head node
	if(ll->head != NULL)
	{
		remove_node(ll, ll->head);
		return 1;
	}

	return 0;
}

int remove_tail(linkedlist* ll)
{
	// if there is a tail node remove the tail node
	if(ll->tail != NULL)
	{
		remove_node(ll, ll->tail);
		return 1;
	}

	return 0;
}

// remove the given element from the linked list
int remove_from_list(linkedlist* ll, const void* data)
{
	llnode* node_p = get_node(data);

	if(is_new_node(node_p))
	{
		return 0;
	}

	remove_node(ll, node_p);

	return 1;
}

const void* get_nth_from_head(linkedlist* ll, unsigned long long int n)
{
	llnode* node_p = ll->head;
	while(node_p != NULL && n > 0)
	{
		n--;
		node_p = node_p->next;
	}
	return (node_p == NULL) ? NULL : get_data(node_p);
}

const void* get_nth_from_tail(linkedlist* ll, unsigned long long int n)
{
	llnode* node_p = ll->tail;
	while(node_p != NULL && n > 0)
	{
		n--;
		node_p = node_p->prev;
	}
	return (node_p == NULL) ? NULL : get_data(node_p);
}

const void* find_in_list(const linkedlist* ll, const void* data)
{
	llnode* node_p = ll->head;
	while(node_p != NULL)
	{
		const void* data_at_node = get_data(node_p);
		if(ll->compare(data_at_node, data) == 0)
		{
			return data_at_node;
		}
		node_p = node_p->next;
	}
	return NULL;
}

void for_each_in_list(const linkedlist* ll, void (*operation)(const void* data_p, const void* additional_params), const void* additional_params)
{
	llnode* node_p = ll->head;
	while(node_p != NULL)
	{
		operation(get_data(node_p), additional_params);
		node_p = node_p->next;
	}
}

static void print_linkedlist_wrapper(linkedlist* ll, const llnode* node_p, void (*print_element)(const void* data_p))
{
	printf("\tprev => %p\n", node_p->prev);
	printf("\t\tnode => %p\n", node_p);
	printf("\t\tdata => ");print_element(get_data(node_p));printf("\n");
	printf("\tnext => %p\n", node_p->next);
	printf("\n");
}

void print_linkedlist(linkedlist* ll, void (*print_element)(const void* data_p))
{
	printf("Linkedlist : %llu nodes\n", ll->node_count);
	printf("head : %p\n", ll->head);
	printf("tail : %p\n", ll->tail);
	
	llnode* node_p = ll->head;
	while(node_p != NULL)
	{
		print_linkedlist_wrapper(ll, node_p, print_element);
		node_p = node_p->next;
	}

	printf("\n");
}