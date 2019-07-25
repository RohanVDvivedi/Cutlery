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
	if(ll->head != NULL)
	{
		remove_node(ll, ll->head);
	}
}

void remove_tail(linkedlist* ll)
{
	if(ll->tail != NULL)
	{
		remove_node(ll, ll->tail);
	}
}

void remove_node(linkedlist* ll, node* node_p)
{
	if(node_p->next != NULL)
	{
		node_p->next->prev = node_p->prev;
	}
	if(node_p->prev != NULL)
	{
		node_p->prev->next = node_p->next;
	}
	if(node_p == ll->head)
	{
		ll->head = node_p->next;
	}
	if(node_p == ll->tail)
	{
		ll->tail = node_p->prev;
	}
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