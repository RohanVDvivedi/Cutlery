#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include<stdio.h>

// node that must be used for building a singly linkedlist
typedef struct sllnode sllnode;
struct sllnode
{
	sllnode* next; // need only the next node in the singly linked list
};

// node that must be used for building a doubly linkedlist
typedef struct dllnode dllnode;
struct dllnode
{
	// the previous and next node in the doubly linked list
	dllnode* next;
	dllnode* prev;	
	// in the head node of the doubly linkedlist, the prev pointer points to the tail of the doubly linkedlist
};

typedef enum lltype lltype;
enum lltype
{
	SINGLY_LIST,
	DOUBLY_LIST
};

typedef struct linkedlist linkedlist;
struct linkedlist
{
	// this is the type of linkedlist, either singly or doubly
	lltype type;

	// defines the address of the data, with respect to the linkedlist node
	// this is how we reach node addresses from provided user's structure data addresses and viceversa
	unsigned int node_offset;

	// head->next->...->next = tail
	void* head;
	// this head can be a sllnode* or a dllnode*
};

/*
**      PLEASE UNDERSTAND THE STRUCTURE OF SINGLY AND DOUBLY LIST 
**      BEFORE PROCEEDING ANY FURTHER
*********************************************************************************
**		Legend :
**			-+-+>	:	next
**			~~~~>	:	prev
**
**********************************************************************************
**      singly linkedlist with 4 nodes
**
**      head -+-+> node_1 -+-+> node_2 -+-+> tail -+-+>(null)
**
**********************************************************************************
**
**      For a singly linkedlist is very simple to understand, 
**      every node has a next pointer, pointing to the next node in the list
**      you can not go back to the previous node
**      the next pointer of the tail is always equal to NULL
**
**********************************************************************************
**      doubly linkedlist with 4 nodes
**
**      head -+-+> node_1 -+-+> node_2 -+-+> tail -+-+>(null)
**        |  <~~~~        <~~~~        <~~~~   ^
**        |                                    |
**        +~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+
**
**********************************************************************************
**
**      For a doubly linkedlist , 
**      If you consider only the next nodes it behaves the same way as a singly linkedlist
**      but there is also a prev pointer that points to previous node of every node
**
**      the next pointer of the last node (i.e. tail node) is NULL
**      but the prev pointer of the first node (i.e. head node) is not NULL
**      and, is used for instead pointing to the tail node 
**      (this helps us to save 8 bytes in linkedlist struct)
**      
**      thus
**          linkedlist.head->next->next ... ->next = NULL (i.e. tail->next = NULL)
**          linkedlist.head->prev = tail
**          tail->next = NULL    but   linkedlist.head->prev = tail
**
**********************************************************************************
*/

// initializes to a new linked list
void initialize_linkedlist(linkedlist* ll, lltype type, unsigned int node_offset);

// always initialize your linkedlist node before using it
void initialize_dllnode(dllnode* node_p);
void initialize_sllnode(sllnode* node_p);

// simply gets head node data
const void* get_head(linkedlist* ll);
const void* get_tail(linkedlist* ll);
const void* get_nth_from_head(linkedlist* ll, unsigned int n);
const void* get_nth_from_tail(linkedlist* ll, unsigned int n);

// inserts will return 0, and fail if llnode of data is not a new node
int insert_head(linkedlist* ll, const void* data);
int insert_tail(linkedlist* ll, const void* data);
int insert_before(linkedlist* ll, const void* data_xist, const void* data);
int insert_after(linkedlist* ll, const void* data_xist, const void* data);

// removes will return 0, and fail if llnode of data is a new node
int remove_head(linkedlist* ll);
int remove_tail(linkedlist* ll);
int remove_from_list(linkedlist*ll, const void* data);
int remove_next_of(linkedlist* ll, const void* data_xist);
int remove_prev_of(linkedlist* ll, const void* data_xist);

// This function is tricky to explain, it returns true 2 types of nodes
// 1. a node that does not exist in any linkedlist
// 2. a node that is a only single element of any other linkedlist (other than the ll linkedlist provided)
// a true of this function signifies that this node can be inserted into ll linkedlist without any bad consequences
// a true signifies that this node_p does not exist in any other linkedlist having more than one element
int is_new_dllnode(linkedlist* ll, dllnode* node_p);
int is_new_sllnode(linkedlist* ll, sllnode* node_p);

// get the data from the linkedlist, that equals data, based on the comparator provided
// in the compare function the first parameter is the data from the linkedlist,
// while the second parameter is the data that has been provided by you
// it will return the pointer to the linkedlist data that compares equal (i.e. compare function returns 0)
const void* find_equals_in_list(const linkedlist* ll, const void* data, int (*compare)(const void* ll_data, const void* data));

int is_linkedlist_empty(const linkedlist* ll);

// perform operation on all the elements of the linked list
// the function is designed well, you may call free on your data, in the provided operation function
void for_each_in_list(const linkedlist* ll, void (*operation)(const void* data_p, const void* additional_params), const void* additional_params);

// prints complete linked list
void print_linkedlist(linkedlist* ll, void (*print_element)(const void* data_p));

#endif