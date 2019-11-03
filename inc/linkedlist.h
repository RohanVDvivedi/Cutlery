#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include<stdio.h>
#include<stdlib.h>
#include<bucket.h>

// to avoid name collision with node of balancedbst
#define node llnode

// to avoid name collision with functions of hashmap
#define insert_entry 			insert_entry_in_ll
#define find_value				find_value_from_ll 
#define update_value			update_value_in_ll
#define delete_entry 			delete_entry_from_ll
#define for_each_entry 			for_each_entry_in_ll

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

	// the type of the linkedlist
	linkedlisttype type;

	// compare keys for bucketted linkedlist and returns 0 if they are same, else non-zero
	// it returns 0 if they are same, >0 if key1 is greater than key2 else it must return <0 value
	int (*key_compare)(const void* key1, const void* key2);
};

// returns a new linked list
linkedlist* get_linkedlist(linkedlisttype type, int (*compare)(const void* key1, const void* key2));

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

// perform operation on all the elements of the linked list
void for_each_in_list(const linkedlist* ll, void (*operation)(const void* data_p, const void* additional_params), const void* additional_params);

// prints complete linked list
void print_linkedlist(linkedlist* ll, void (*print_element)(const void* data_p));

// prints complete linkedlist
void print_linkedlist_bucketted(linkedlist* ll, void (*print_key)(const void* key_p), void (*print_value)(const void* value_p));

/*
	BELOW ARE THE ONLY FUNCTIONS FOR BUCKETTED LINKEDLIST
	DONOT INVOKE SIMPLE LINKEDLIST FUNCTIONS DIRECTLY ON BUCKETTED LINKEDLIST
*/

// place a new bucket with given key and value at the head of the linkedlist
// O(1) operation
void insert_entry(linkedlist* ll, const void* key, const void* value);

// get the value from the linkedlist, stored at a particular key
// O(n) operation
const void* find_value(const linkedlist* ll, const void* key);

// returns 1 if the bucket is found for the given key and updated
// since you would have to first search the bucket, it is O(n) operation
int update_value(linkedlist* ll, const void* key, const void* value, const void** return_value);

// returns 1 if the bucket is found for the given key and removed from linkedlist and deleted
// since you would have to first search the bucket, it is O(n) operation
int delete_entry(linkedlist* ll, const void* key, const void** return_key, const void** return_value);

// perform operation on all the buckets of the linked list
void for_each_entry(const linkedlist* ll, void (*operation)(const void* key_p, const void* value_p, const void* additional_params), const void* additional_params);

/*
	ABOVE ARE THE FUNCTIONS FOR BUCKETTED LINKEDLIST
*/

// deletes the linked list and all of its nodes
void delete_linkedlist(linkedlist* ll);

#undef node
#undef insert_entry
#undef find_value
#undef update_value
#undef delete_entry
#undef for_each_entry

#endif