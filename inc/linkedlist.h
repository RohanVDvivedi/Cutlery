#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include<dstring.h>

// LINKEDLIST is a circular doubly linkedlist

typedef struct llnode llnode;
struct llnode
{
	// the previous and next node in the linked list
	llnode* next;	// the next of the tail node points to head
	llnode* prev;	// the prev of the head node points to tail
};

// it is a circular doubly linkedlist
typedef struct linkedlist linkedlist;
struct linkedlist
{
	// defines the address of the data, with respect to the linkedlist node
	// this is how we reach node addresses from provided user's structure data addresses and viceversa
	unsigned int node_offset;

	// head->next->...->next = tail // ->next is called n-1 times, n being the number of elements
	// tail->prev->...->prev = head // ->prev is called n-1 times, n being the number of elements
	// and always head->prev = tail and tail->next = head
	llnode* head;
};

// initializes to a new linked list
void initialize_linkedlist(linkedlist* ll, unsigned int node_offset);

// always initialize your linkedlist node before using it
void initialize_llnode(llnode* node_p);

int is_empty_linkedlist(const linkedlist* ll);

// simply gets nth from head or tail node's data
const void* get_head(const linkedlist* ll);
const void* get_tail(const linkedlist* ll);
const void* get_nth_from_head(const linkedlist* ll, unsigned int n);
const void* get_nth_from_tail(const linkedlist* ll, unsigned int n);

// get next or previous node's data of a given data element of the linkedlist
// data_xist must be a valid data pointer existing in the linkedlist
const void* get_next_of(const linkedlist* ll, const void* data_xist);
const void* get_prev_of(const linkedlist* ll, const void* data_xist);

// inserts will return 0, and fail if llnode of data is not a new node
// data_xist must be a valid data pointer existing in the linkedlist
int insert_head(linkedlist* ll, const void* data);
int insert_tail(linkedlist* ll, const void* data);
int insert_before(linkedlist* ll, const void* data_xist, const void* data);
int insert_after(linkedlist* ll, const void* data_xist, const void* data);

// insert all from another linkedlist
// both the linkedlists must have the same node_offset
// after this operation insert_from_ll would be an empty linkedlist
int insert_all_at_head(linkedlist* ll, linkedlist* insert_from_ll);
int insert_all_at_tail(linkedlist* ll, linkedlist* insert_from_ll);
int insert_all_before(linkedlist* ll, const void* data_xist, linkedlist* insert_from_ll);
int insert_all_after(linkedlist* ll, const void* data_xist, linkedlist* insert_from_ll);

// removes will return 0, and fail if llnode of data is a new node or if llnode does not exist in this linkedlist
int remove_head(linkedlist* ll);
int remove_tail(linkedlist* ll);
int remove_from_linkedlist(linkedlist* ll, const void* data);

// This function returns true(1),
// if a llnode is insertable in the given linkedlist ll
// i.e. next and prev pointers are NULL and it is not at the head of ll linkedlist
// please try and avoid using this function in user application
int is_new_llnode(const linkedlist* ll, const llnode* node_p);

// get the data from the linkedlist, that equals data, based on the comparator provided
// it will return the pointer to the data contained in the linkedlist that compares equal (i.e. compare function returns 0)
const void* find_equals_in_linkedlist(const linkedlist* ll, const void* data, int (*compare)(const void* data1, const void* data2));

// perform operation on all the elements of the linked list
// the function is designed well, you may call free on your data, in the provided operation function
void for_each_in_linkedlist(const linkedlist* ll, void (*operation)(const void* data_p, const void* additional_params), const void* additional_params);

// serializes the linkedlist, and appends the serialized form to the dstring
void sprint_linkedlist(dstring* append_str, const linkedlist* ll, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs);

#endif