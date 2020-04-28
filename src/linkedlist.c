#include<linkedlist.h>

// to avoid name collision with node of balancedbst
#define node 			llnode
#define delete_node 	delete_llnode
#define find_node		find_llnode

// to avoid name collision with functions of hashmap
#define insert_entry 			insert_entry_in_ll
#define find_value				find_value_from_ll 
#define update_value			update_value_in_ll
#define delete_entry 			delete_entry_from_ll
#define for_each_entry 			for_each_entry_in_ll

linkedlist* get_linkedlist(linkedlisttype type, int (*key_compare)(const void* key1, const void* key2))
{
	linkedlist* ll = (linkedlist*) calloc(1, sizeof(linkedlist));
	ll->head = NULL;
	ll->tail = NULL;
	ll->type = type;
	ll->key_compare = key_compare;
	return ll;
}

node* get_new_simple_node(const void* data_p)
{
	node* node_p = (node*) calloc(1, sizeof(node));
	node_p->data_p = data_p;
	return node_p;
}

void delete_node(linkedlist* ll, node* node_p)
{
	if(ll->type == BUCKETTED && node_p->data_p != NULL)
	{
		delete_bucket(((bucket*)(node_p->data_p)));
	}
	free(node_p);
}

void insert_head(linkedlist* ll, const void* data_p)
{
	// case when the linkedlist is empty
	if(ll->head == NULL)
	{
		ll->head = get_new_simple_node(data_p);
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
		ll->tail = get_new_simple_node(data_p);
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
	node* new_node = get_new_simple_node(data_p);
	
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
	node* new_node = get_new_simple_node(data_p);

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
	delete_node(ll, node_p);
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

static void for_each_node_in_list(const linkedlist* ll, void (*operation)(node* node_p, const void* additional_params), const void* additional_params)
{
	node* node_p = ll->head;
	while(node_p != NULL)
	{
		operation(node_p, additional_params);
		node_p = node_p->next;
	}
}

void for_each_in_list(const linkedlist* ll, void (*operation)(const void* data_p, const void* additional_params), const void* additional_params)
{
	node* node_p = ll->head;
	while(node_p != NULL)
	{
		operation(node_p->data_p, additional_params);
		node_p = node_p->next;
	}
}

const node* find_node(const linkedlist* ll, const void* key)
{
	node* node_p = ll->head;
	while(node_p != NULL)
	{
		bucket* bucket_p = (bucket*)(node_p->data_p);
		if(bucket_p != NULL && (ll->key_compare(key, bucket_p->key) == 0))
		{
			return node_p;
		}
		node_p = node_p->next;
	}
	return NULL;
}

const bucket* find_bucket(const linkedlist* ll, const void* key)
{
	const node* found_node_p = find_node(ll, key);
	return found_node_p != NULL ? ((bucket*)found_node_p->data_p) : NULL;
}

void insert_entry(linkedlist* ll, const void* key, const void* value)
{
	const bucket* new_bucket_p = get_bucket(key, value);
	insert_head(ll, new_bucket_p);
}

const void* find_value(const linkedlist* ll, const void* key)
{
	const bucket* found_bucket_p = find_bucket(ll, key);
	return found_bucket_p != NULL ? found_bucket_p->value : NULL;
}

int update_value(linkedlist* ll, const void* key, const void* value, const void** return_value)
{
	bucket* found_bucket_p = (bucket*)find_bucket(ll, key);
	if(found_bucket_p == NULL)
	{
		return 0;
	}
	if(return_value != NULL)
	{
		(*return_value) = found_bucket_p->value;
	}
	found_bucket_p->value = value;
	return 1;
}

int delete_entry(linkedlist* ll, const void* key, const void** return_key, const void** return_value)
{
	node* found_node_p = ((node*) find_node(ll, key));
	if(found_node_p != NULL)
	{
		const bucket* found_bucket_p = found_node_p->data_p;
		if(return_key != NULL)
		{
			(*return_key) = found_bucket_p->key;
		}
		if(return_value != NULL)
		{
			(*return_value) = found_bucket_p->value;
		}
		remove_node(ll, ((node*)found_node_p));
		return 1;
	}
	return 0;
}

void for_each_entry(const linkedlist* ll, void (*operation)(const void* key_p, const void* value_p, const void* additional_params), const void* additional_params)
{
	node* node_p = ll->head;
	while(node_p != NULL)
	{
		bucket* bucket_p = ((bucket*)(node_p->data_p));
		operation(bucket_p->key, bucket_p->value, additional_params);
		node_p = node_p->next;
	}
}

static void print_linkedlist_element_wrapper(node* node_p, const void* print_element)
{
	printf("\tprev => %d\n", ((int)node_p->prev));
	printf("\t\tnode => %d\n", ((int)node_p));
	printf("\t\tdata => ");((void (*)(const void* data_p))print_element)(node_p->data_p);printf("\n");
	printf("\tnext => %d\n", ((int)node_p->next));
	printf("\n");
}

static void print_linkedlist_bucket_wrapper(node* node_p, const void* prbucket_p)
{
	printf("\tprev => %d\n", ((int)node_p->prev));
	printf("\t\tnode => %d\n", ((int)node_p));
	bucket* bucket_p = ((bucket*)(node_p->data_p));
	bucket* prbucket = ((bucket*)prbucket_p);
	printf("\t\tdata => ");
	print_bucket(node_p->data_p, ((void (*)(const void*))(prbucket->key)), ((void (*)(const void*))(prbucket->value)));
	printf("\n");
	printf("\tnext => %d\n", ((int)node_p->next));
	printf("\n");
}

void print_linkedlist(linkedlist* ll, void (*print_element)(const void* data_p))
{
	if(ll->type == SIMPLE)
	{
		printf("linkedlist : SIMPLE\n");
	}
	else if(ll->type == BUCKETTED)
	{
		printf("linkedlist : BUCKETTED\n");
	}
	printf("head : %d\n", ((int)ll->head));
	printf("tail : %d\n", ((int)ll->tail));
	if(ll->type == SIMPLE)
	{
		for_each_node_in_list(ll, print_linkedlist_element_wrapper, print_element);
	}
	printf("\n");
}

void print_linkedlist_bucketted(linkedlist* ll, void (*print_key)(const void* key_p), void (*print_value)(const void* value_p))
{
	if(ll->type == SIMPLE)
	{
		printf("linkedlist : SIMPLE\n");
	}
	else if(ll->type == BUCKETTED)
	{
		printf("linkedlist : BUCKETTED\n");
	}
	printf("head : %d\n", ((int)ll->head));
	printf("tail : %d\n", ((int)ll->tail));
	if(ll->type == BUCKETTED)
	{
		bucket prbucket = {.key = print_key,.value = print_value};
		for_each_node_in_list(ll, print_linkedlist_bucket_wrapper, &prbucket);
	}
	printf("\n");
}

#undef node
#undef insert_entry
#undef find_value
#undef update_value
#undef delete_entry
#undef for_each_entry
#undef delete_node
#undef find_node