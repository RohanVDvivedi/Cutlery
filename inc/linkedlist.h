#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include<notifier_interface.h>
#include<dstring.h>

// LINKEDLIST is a circular doubly linkedlist

typedef struct llnode llnode;
struct llnode
{
	// the previous and next node in the linked list
	llnode* next;	// the next of the tail node points to head
	// here the next node comes first, because we want linkedlist to be convertible to a singlylist, without much hazzle

	llnode* prev;	// the prev of the head node points to tail
};

// it is a circular doubly linkedlist
typedef struct linkedlist linkedlist;
struct linkedlist
{
	// defines the address of the data, with respect to the linkedlist node
	// this is how we reach node addresses from provided user's structure data addresses and viceversa
	cy_uint node_offset;

	// head->next->...->next = tail // ->next is called n-1 times, n being the number of elements
	// tail->prev->...->prev = head // ->prev is called n-1 times, n being the number of elements
	// and always head->prev = tail and tail->next = head
	llnode* head;
};

// initializes to a new linked list
void initialize_linkedlist(linkedlist* ll, cy_uint node_offset);

// always initialize your linkedlist node before using it
void initialize_llnode(llnode* node_p);

// a free floating llnode is the one, that is not referenced in any linkedlist, i.e a node that can be inserted to any linkedlist
int is_free_floating_llnode(const llnode* node_p);

int is_empty_linkedlist(const linkedlist* ll);

// simply gets head, tail or nth from head or tail data
const void* get_head_of_linkedlist(const linkedlist* ll);
const void* get_tail_of_linkedlist(const linkedlist* ll);
const void* get_from_head_of_linkedlist(const linkedlist* ll, cy_uint index);
const void* get_from_tail_of_linkedlist(const linkedlist* ll, cy_uint index);

// get next or previous element's data of a given data element of the linkedlist
// data_xist must be a valid data pointer existing in the linkedlist
const void* get_next_of_in_linkedlist(const linkedlist* ll, const void* data_xist);
const void* get_prev_of_in_linkedlist(const linkedlist* ll, const void* data_xist);

// insert will return 0, and fail if llnode of data is not a new node
// data_xist must be a valid data pointer existing in the linkedlist
int insert_head_in_linkedlist(linkedlist* ll, const void* data);
int insert_tail_in_linkedlist(linkedlist* ll, const void* data);
int insert_before_in_linkedlist(linkedlist* ll, const void* data_xist, const void* data);
int insert_after_in_linkedlist(linkedlist* ll, const void* data_xist, const void* data);

// insert all from another linkedlist
// both the linkedlists must have the same node_offset
// after this operation insert_from_ll would be an empty linkedlist
int insert_all_at_head_in_linkedlist(linkedlist* ll, linkedlist* insert_from_ll);
int insert_all_at_tail_in_linkedlist(linkedlist* ll, linkedlist* insert_from_ll);
int insert_all_before_in_linkedlist(linkedlist* ll, const void* data_xist, linkedlist* insert_from_ll);
int insert_all_after_in_linkedlist(linkedlist* ll, const void* data_xist, linkedlist* insert_from_ll);

// removes will return 0, and fail if llnode of data is a new node or if llnode does not exist in this linkedlist
int remove_head_from_linkedlist(linkedlist* ll);
int remove_tail_from_linkedlist(linkedlist* ll);
int remove_from_linkedlist(linkedlist* ll, const void* data);

// removes all the elements from the linkedlist and reinitializes their embedded llnode
// after this if a notifier_interface if specified (i.e. not NULL), then it is notified
void remove_all_from_linkedlist(linkedlist* ll, notifier_interface* ni_p);

// swap positions of the 2 elements in a given linkedlist
int swap_in_linkedlist(linkedlist* ll, const void* data_xist1, const void* data_xist2);

// swap positions of the 2 elements in 2 different linkedlists
int swap_in_2_linkedlists(linkedlist* ll_1, const void* data_xist1, linkedlist* ll_2, const void* data_xist2);

// get the data from the linkedlist, that equals data, based on the comparator provided
// it will return the pointer to the data contained in the linkedlist that compares equal (i.e. compare function returns 0)
const void* find_equals_in_linkedlist(const linkedlist* ll, const void* data, int (*compare)(const void* data1, const void* data2));

// radix sort all of the linkedlist
void radix_sort_linkedlist(linkedlist* ll, unsigned long long int (*get_sort_attribute)(const void* data));

// bubble sort all of the linkedlist
void bubble_sort_linkedlist(linkedlist* ll, int (*compare)(const void* data1, const void* data2));

// perform operation on all the elements of the linked list
// the function is designed well, you may call free on your data, in the provided operation function
void for_each_in_linkedlist(const linkedlist* ll, void (*operation)(const void* data_p, const void* additional_params), const void* additional_params);

// serializes the linkedlist, and appends the serialized form to the dstring
void sprint_linkedlist(dstring* append_str, const linkedlist* ll, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs);

#endif