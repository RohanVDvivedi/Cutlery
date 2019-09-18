#include<linkedlist.h>

// to avoid name collision with node of balancedbst
#define node llnode

linkedlist* get_linkedlist()
{
	linkedlist* ll = (linkedlist*) calloc(1, sizeof(linkedlist));
	ll->head = NULL;
	ll->tail = NULL;
	return ll;
}

node* get_new_node(const void* data_p)
{
	node* node_p = (node*) calloc(1, sizeof(node));
	node_p->data_p = data_p;
	return node_p;
}

void delete_node(node* node_p)
{
	free(node_p);
}

void insert_head(linkedlist* ll, const void* data_p)
{
	// case when the linkedlist is empty
	if(ll->head == NULL)
	{
		ll->head = get_new_node(data_p);
		ll->tail = ll->head;
	}
	// incase when there is atleast 1 node
	// we place the new node before the current head node
	else
	{
		insert_node_before(ll, ll->head, data_p);
	}
}

void insert_tail(linkedlist* ll, const void* data_p)
{
	// case when the linkedlist is empty
	if(ll->tail == NULL)
	{
		ll->tail = get_new_node(data_p);
		ll->head = ll->tail;
	}
	// incase when there is atleast 1 node
	// we place the new node after the current tail node
	else
	{
		insert_node_after(ll, ll->tail, data_p);
	}
}

void insert_node_before(linkedlist* ll, node* node_p, const void* data_p)
{
	node* new_node = get_new_node(data_p);
	
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
}

void insert_node_after(linkedlist* ll, node* node_p, const void* data_p)
{
	node* new_node = get_new_node(data_p);

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
}

void remove_head(linkedlist* ll)
{
	// if there is a head node remove the head node
	if(ll->head != NULL)
	{
		remove_node(ll, ll->head);
	}
}

void remove_tail(linkedlist* ll)
{
	// if there is a tail node remove the tail node
	if(ll->tail != NULL)
	{
		remove_node(ll, ll->tail);
	}
}

void remove_node(linkedlist* ll, node* node_p)
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

	// delete node_p
	delete_node(node_p);
}

const void* get_head_data(linkedlist* ll)
{
	return ll == NULL ? NULL : (ll->head == NULL ? NULL : ll->head->data_p) ;
}

const void* get_tail_data(linkedlist* ll)
{
	return ll == NULL ? NULL : (ll->tail == NULL ? NULL : ll->tail->data_p) ;
}

const node* get_nth_node_from_head(linkedlist* ll, unsigned long long int n)
{
	node* node_p = ll->head;
	while(node_p != NULL && n > 0)
	{
		n--;
		node_p = node_p->next;
	}
	return node_p;
}

const node* get_nth_node_from_tail(linkedlist* ll, unsigned long long int n)
{
	node* node_p = ll->tail;
	while(node_p != NULL && n > 0)
	{
		n--;
		node_p = node_p->prev;
	}
	return node_p;
}

void delete_linkedlist(linkedlist* ll)
{
	while(ll->head != NULL)
	{
		remove_head(ll);
	}
	free(ll);
}

void for_each_node_in_list(const linkedlist* ll, void (*operation)(node* node_p, const void* additional_params), const void* additional_params)
{
	node* node_p = ll->head;
	while(node_p != NULL)
	{
		operation(node_p, additional_params);
		node_p = node_p->next;
	}
}

void for_each_in_list(const linkedlist* ll, void (*operation)(void* data_p, const void* additional_params), const void* additional_params)
{
	node* node_p = ll->head;
	while(node_p != NULL)
	{
		operation(((void*)node_p->data_p), additional_params);
		node_p = node_p->next;
	}
}

node* find_first_node_in_list(const linkedlist* ll, void* data_p, int (*compare)(const void* data_p1, const void* data_p2, const void* additional_params), const void* additional_params)
{
	node* node_p = ll->head;
	while(node_p != NULL)
	{
		if(compare(node_p->data_p, data_p, additional_params) == 0)
		{
			return node_p;
		}
		node_p = node_p->next;
	}
	return NULL;
}

const void* find_first_in_list(const linkedlist* ll, void* data_p, int (*compare)(const void* data_p1, const void* data_p2, const void* additional_params), const void* additional_params)
{
	node* node_p = find_first_node_in_list(ll, data_p, compare, additional_params);
	return node_p == NULL ? NULL : node_p->data_p;
}

int remove_from_list(const linkedlist* ll, void* data_p, int (*compare)(const void* data_p1, const void* data_p2, const void* additional_params), const void* additional_params, const void** return_data)
{
	node* node_to_be_deleted = find_first_node_in_list(ll, data_p, compare, additional_params);
	if(node_to_be_deleted == NULL)
	{
		return 0;
	}
	else
	{
		if(return_data != NULL)
		{
			(*(return_data)) = node_to_be_deleted->data_p;
		}
		remove_node((linkedlist*)ll, node_to_be_deleted);
		return 1;
	}
}

void print_linkedlist_element_wrapper(node* node_p, const void* print_element)
{
	printf("\tprev => %d\n", ((int)node_p->prev));
	printf("\t\tnode => %d\n", ((int)node_p));
	printf("\t\tdata => ");((void (*)(const void* data_p))print_element)(node_p->data_p);printf("\n");
	printf("\tnext => %d\n", ((int)node_p->next));
	printf("\n");
}

void print_linkedlist(linkedlist* ll, void (*print_element)(const void* data_p))
{
	printf("linkedlist : \n");
	printf("head : %d\n", ((int)ll->head));
	printf("tail : %d\n", ((int)ll->tail));
	for_each_node_in_list(ll, print_linkedlist_element_wrapper, print_element);
	printf("\n");
}

#undef node