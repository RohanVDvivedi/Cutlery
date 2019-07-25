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

}

void insert_tail(linkedlist* ll, const void* data_p)
{

}

void intert_node_before(node* node_p, const void* data_p)
{

}

void intert_node_after(node* node_p, const void* data_p)
{

}

void remove_head(linkedlist* ll)
{
	node* old_head = ll->head;
	if(old_head != NULL)
	{
		ll->head = old_head->next;
		if(ll->head != NULL)
		{
			ll->head->prev = NULL;
		}
		delete_node(old_head);
	}
}

void remove_tail(linkedlist* ll)
{
	node* old_tail = ll->tail;
	if(old_tail != NULL)
	{
		ll->tail = old_tail->prev;
		if(ll->tail != NULL)
		{
			ll->tail->next = NULL;
		}
		delete_node(old_tail);
	}
}

void remove_node(node* node_p)
{
	
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