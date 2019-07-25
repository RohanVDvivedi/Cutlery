#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include<stdio.h>
#include<stdlib.h>

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

typedef struct linkedlist linkedlist;
struct linkedlist
{
	// head->next->...->next = tail
	// always head->prev = NULL
	node* head;

	// tail->prev->...->prev = head
	// always tail->next = NULL
	node* tail;
};

// returns a new linked list
linkedlist* get_linkedlist();

// inserts a new head, the previous head is now next to the new head
void insert_head(linkedlist* ll, const void* data_p);

// inserts a new tail, the previous tail is now prev to the new tail
void insert_tail(linkedlist* ll, const void* data_p);

// create a node and insert it in linkedlist before node_p
// here node_p may not be null
void insert_node_before(linkedlist* ll, node* node_p, const void* data_p);

// create a node and insert it in linkedlist after node_p
// here node_p may not be null
void insert_node_after(linkedlist* ll, node* node_p, const void* data_p);

// removes the head, now the new head is next of the previous head
void remove_head(linkedlist* ll);

// removes the tail, now the new tail is prev of the previous head
void remove_tail(linkedlist* ll);

// remove the given node from the linked list
void remove_node(linkedlist*ll, node* node_p);

// simply gets head data
const void* get_head_data(linkedlist* ll);

// simply gets tail
const void* get_tail_data(linkedlist* ll);

// get pointer to nth element from head by doing next next
const node* get_nth_node_from_head(linkedlist* ll, unsigned long long int n);

// get pointer to nth element from tail by doing next next
const node* get_nth_node_from_tail(linkedlist* ll, unsigned long long int n);

// deletes the linked list and all of its nodes
void delete_linkedlist(linkedlist* ll);

// prints complete linked list
void print_linkedlist(linkedlist* ll, void (*print_element)(const void* data_p));

#endif