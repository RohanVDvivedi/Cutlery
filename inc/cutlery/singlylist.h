#ifndef singlylist_H
#define singlylist_H

#include<cutlery/comparator_interface.h>
#include<cutlery/notifier_interface.h>
#include<cutlery/dstring.h>

// SINGLYLIST is a singly circular linkedlist

typedef struct slnode slnode;
struct slnode
{
	// the next node in the singly linkedlist
	slnode* next;
};

// it is a singly circular linkedlist
typedef struct singlylist singlylist;
struct singlylist
{
	// defines the address of the data, with respect to the singlylist node (slnode)
	// this is how we reach node addresses from provided user's structure data addresses and viceversa
	cy_uint node_offset;

	// head->next->...->next = tail // ->next is called n-1 times, n being the number of elements
	// tail->next = head
	slnode* head;
	slnode* tail;
};

// initializes to a new singlylist
void initialize_singlylist(singlylist* sl, cy_uint node_offset);

// always initialize your singlylist node before using it
void initialize_slnode(slnode* node_p);

// returns 1, if the node_p does not exist in any singlylist
int is_free_floating_slnode(const slnode* node_p);

int is_empty_singlylist(const singlylist* sl);

// simply gets head, tail or nth from head data
const void* get_head_of_singlylist(const singlylist* sl);
const void* get_tail_of_singlylist(const singlylist* sl);
const void* get_from_head_of_singlylist(const singlylist* sl, cy_uint index);

// get next element's data of a given data element of the singlylist
// data_xist must be a valid data pointer existing in the singlylist
const void* get_next_of_in_singlylist(const singlylist* sl, const void* data_xist);

// insert will return 0, and fail if slnode of data is not a new node
// data_xist must be a valid data pointer existing in the singlylist
int insert_head_in_singlylist(singlylist* sl, const void* data);
int insert_tail_in_singlylist(singlylist* sl, const void* data);
int insert_after_in_singlylist(singlylist* sl, const void* data_xist, const void* data);

// insert all from another singlylist
// both the singlylists must have the same node_offset
// after this operation insert_from_ll would be an empty singlylist
int insert_all_at_head_in_singlylist(singlylist* sl, singlylist* insert_from_sl);
int insert_all_at_tail_in_singlylist(singlylist* sl, singlylist* insert_from_sl);
int insert_all_after_in_singlylist(singlylist* sl, const void* data_xist, singlylist* insert_from_sl);

// removes will return 0, and fail if slnode of data is a new node or if slnode does not exist in this singlylist
int remove_head_from_singlylist(singlylist* sl);

// it returns the pointer to removed data, else on failure it returns NULL
const void* remove_next_of_from_singlylist(singlylist* sl, const void* data_xist);

// removes all the elements from the singlylist and reinitializes their embedded slnode
// after this if a notifier_interface if specified (i.e. not NULL), then it is notified
void remove_all_from_singlylist(singlylist* sl, notifier_interface* ni_p);

// get the data from the singlylist, that equals data, based on the comparator provided
// it will return the pointer to the data contained in the singlylist that compares equal (i.e. comparator returns 0)
// "data" becomes the second parameter of the comparator
const void* find_equals_in_singlylist(const singlylist* sl, const void* data, const comparator_interface* comparator);

// radix sort all of the singlylist
void radix_sort_singlylist(singlylist* sl, unsigned long long int (*get_sort_attribute)(const void* data));

// bubble sort all of the singlylist
void bubble_sort_singlylist(singlylist* sl, const comparator_interface* comparator);

// perform operation on all the elements of the linked list
// the function is designed well, you may call free on your data, in the provided operation function
void for_each_in_singlylist(const singlylist* sl, void (*operation)(const void* data_p, const void* additional_params), const void* additional_params);

// serializes the singlylist, and appends the serialized form to the dstring
void sprint_singlylist(dstring* append_str, const singlylist* sl, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs);

#endif