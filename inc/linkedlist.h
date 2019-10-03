#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include<stdio.h>
#include<stdlib.h>

// to avoid name collision with node of balancedbst
#define node llnode

// the datastructure of the linkedlist manages its data on its own
// you manage your own data
// the pointer you give will be directly stored internally
// you have to delete your data pointers that you pass

typedef struct node node;
struct node
{
	// the previous node in the linked list
	node* prev;

	// the data that this node addresses
	const void* data_p;

	// the next node in the linked list
	node* next;
};

typedef enum linkedlisttype linkedlisttype;
enum linkedlisttype
{
	// a SIMPLE linked list is a double linked list, that stores and maintains pointers to your data
	SIMPLE,

	// a BUCKETTED linked list, maintaine buckets on its node's data_p, you can put entries, and search using the key
	BUCKETTED
};

typedef struct linkedlist linkedlist;
struct linkedlist
{
	// head->next->...->next = tail
	// always head->prev = NULL
	node* head;

	// tail->prev->...->prev = head
	// always tail->next = NULL
	node* tail;

	// compare data and returns 0 if they are same, else non-zero
	// it returns 0 if they are same, >0 if key_or_data11 is greater than key_or_data12 else it must return <0 value
	// if SIMPLE, we compare data_p else we consider data_p as a bucket, and compare their keys
	int (*compare)(const void* key_or_data1, const void* key_or_data2);
};

// returns a new linked list
linkedlist* get_linkedlist();

/*
	BELOW LINKEDLIST FUNCTIONS CAN BE USED TO IMPLEMENT QUEUES, STACKS AND DOUBLE ENDED QUEUES
*/

// simply gets head data
const void* get_head_data(linkedlist* ll);

// simply gets tail
const void* get_tail_data(linkedlist* ll);

// inserts a new head, the previous head is now next to the new head
void insert_head(linkedlist* ll, const void* data_p);

// inserts a new tail, the previous tail is now prev to the new tail
void insert_tail(linkedlist* ll, const void* data_p);

// removes the head, now the new head is next of the previous head
void remove_head(linkedlist* ll);

// removes the tail, now the new tail is prev of the previous head
void remove_tail(linkedlist* ll);

/*
	ABOVE LINKEDLIST FUNCTIONS CAN BE USED TO IMPLEMENT QUEUES, STACKS AND DOUBLE ENDED QUEUES
*/



/*
	BELOW LINKEDLIST FUNCTIONS CAN BE USED TO DO NODE LEVEL MANIPULATIONS
	TRY NOT TO USE THEM
*/

// create a node and insert it in linkedlist before node_p
// here node_p may not be null
void insert_node_before(linkedlist* ll, node* node_p, const void* data_p);

// create a node and insert it in linkedlist after node_p
// here node_p may not be null
void insert_node_after(linkedlist* ll, node* node_p, const void* data_p);

// remove the given node from the linked list
void remove_node(linkedlist*ll, node* node_p);

// get pointer to nth element from head by doing next next
const node* get_nth_node_from_head(linkedlist* ll, unsigned long long int n);

// get pointer to nth element from tail by doing next next
const node* get_nth_node_from_tail(linkedlist* ll, unsigned long long int n);

/*
	ABOVE LINKEDLIST FUNCTIONS CAN BE USED TO DO NODE LEVEL MANIPULATIONS
	TRY NOT TO USE THEM
*/


// deletes the linked list and all of its nodes
void delete_linkedlist(linkedlist* ll);

// perform operation on all the elements of the linked list
void for_each_in_list(const linkedlist* ll, void (*operation)(void* data_p, const void* additional_params), const void* additional_params);

// prints complete linked list
void print_linkedlist(linkedlist* ll, void (*print_element)(const void* data_p));

/*
	BELOW ARE THE FUNCTIONS FOR BUCKETTED LINKEDLIST
*/

// place the bucket at the head of the linkedlist
// O(1) operation
void put_entry_to_head(linkedlist* ll, const void* key, const void* value);

// place the bucket at the tail of the linkedlist
// O(1) operation
void put_entry_to_tail(linkedlist* ll, const void* key, const void* value);

// get the value from the linkedlist, stored at a particular key
// O(n) operation
const void* find_value(const linkedlist* ll, const void* key);

// returns 1 if the bucket is found for the givemn key and removed from linkedlist and deleted
// since you would have to first search the bucket
int remove_value(linkedlist* ll, const void* key, const void** return_key, const void** return_value);

/*
	ABOVE ARE THE FUNCTIONS FOR BUCKETTED LINKEDLIST
*/

#undef node

#endif