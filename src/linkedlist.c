#include<linkedlist.h>

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
	return node_p;
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

}

void insert_node_after(linkedlist* ll, node* node_p, const void* data_p)
{

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

void delete_node(node* node_p)
{
	free(node_p);
}

void delete_linkedlist(linkedlist* ll)
{
	while(ll->head != NULL)
	{
		remove_head(ll);
	}
	free(ll);
}

void print_linkedlist(linkedlist* ll, void (*print_element)(const void* data_p))
{
	printf("linkedlist : ");
	printf("head : %d\n", ((int)ll->head));
	printf("tail : %d\n", ((int)ll->tail));
	node* node_p = ll->head;
	while(node_p != NULL)
	{
		printf("\tnode => %d\n", ((int)node_p));
		printf("\tprev => %d\n", ((int)node_p->prev));
		printf("\tdata => ");print_element(node_p->data_p);printf("\n");
		printf("\tnext => %d\n", ((int)node_p->next));
		printf("\n");
		node_p = node_p->next;
	}
	printf("\n");
}