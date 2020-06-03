#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include<stdio.h>
#include<stdint.h>

typedef struct llnode llnode;
struct llnode
{
	// the previous and next node in the linked list
	llnode* prev;
	llnode* next;
};

typedef struct linkedlist linkedlist;
struct linkedlist
{
	// head->next->...->next = tail
	// always head->prev = NULL
	llnode* head;

	// tail->prev->...->prev = head
	// always tail->next = NULL
	llnode* tail;

	// number of nodes in the linkedlist
	// this is how we reach node addresses from provided user's structure data addresses
	unsigned long long int node_count;

	// defines the address of the data, with respect to the linkedlist node
	unsigned long long int node_offset;

	// compare data function, this helps in find operation
	int (*compare)(const void* data1, const void* data2);
};

#define initialize_llnode(new_node)		{new_node->next = NULL; new_node->prev = NULL;}

#define is_new_node(new_node)			((new_node->next == NULL) && (new_node->prev == NULL))

// initializes to a new linked list
void initialize_linkedlist(linkedlist* ll, unsigned int node_offset, int (*compare)(const void* data1, const void* data2));

// simply gets head node data
const void* get_head(linkedlist* ll);

// simply gets tail node data
const void* get_tail(linkedlist* ll);

// inserts a new head element, returns 0 if it fails
int insert_head(linkedlist* ll, const void* data);

// inserts a new tail element, returns 0 if it fails
int insert_tail(linkedlist* ll, const void* data);

// inserts the new data in linkedlist before data_xist, returns 0 if it fails
int insert_before(linkedlist* ll, const void* data_xist, const void* data);

// inserts the new data in linkedlist before data_xist, returns 0 if it fails
int insert_after(linkedlist* ll, const void* data_xist, const void* data);

// removes the head, now the new head is next of the previous head
// returns 0 if it fails
int remove_head(linkedlist* ll);

// removes the tail, now the new tail is prev of the previous head
// returns 0 if it fails
int remove_tail(linkedlist* ll);

// remove the given element from the linked list
// returns 0 if it fails
int remove_element(linkedlist*ll, const void* data);

// get pointer to nth element from head by doing next next
const void* get_nth_from_head(linkedlist* ll, unsigned long long int n);

// get pointer to nth element from tail by doing next next
const void* get_nth_from_tail(linkedlist* ll, unsigned long long int n);

// get the data from the linkedlist, that equals data, based on the comparator provided
const void* find_in_list(const linkedlist* ll, const void* data);

// perform operation on all the elements of the linked list
void for_each_in_list(const linkedlist* ll, void (*operation)(const void* data_p, const void* additional_params), const void* additional_params);

// prints complete linked list
void print_linkedlist(linkedlist* ll, void (*print_element)(const void* data_p));

#endif