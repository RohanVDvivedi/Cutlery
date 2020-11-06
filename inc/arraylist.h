#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include<array.h>

// the front and back of the arraylist is synonymous to head and tail of the linkedlist
// arraylist is a circular buffer

typedef struct arraylist arraylist;
struct arraylist
{
	array arraylist_holder;

	// arraylist is a circular buffer
	// from front_index to back_index, if front_index < back_index
	// else front_index -> arraylist_holder.total_size - 1 -> 0 -> back_index

	// front_index points to the front of the arraylist
	unsigned int front_index;

	// arraylist_size is the total number of elements currently in the array list
	unsigned int arraylist_size;

	// the back_index => front_index + arraylist_size % arraylist_holder.total_size
};

void intialize_arraylist(arraylist* al, unsigned int initial_size);

// push_* functions return 1 if data_p is successfully pushed in the arraylist
// push fails and returns with a 0, if the arraylist_holder is full
int push_front(arraylist* al, void* data_p);
int push_back(arraylist* al, void* data_p);

// pop_* functions return 1 if data_p is successfully poped in the arraylist
// pop fails and returns with a 0, if the arraylist_holder is already empty
int pop_front(arraylist* al);
int pop_back(arraylist* al);

// returns an element at the front of the arraylist, this is also the element at the front_index
void* get_front(const arraylist* al);
// returns an element at the back of the arraylist, this is also the element at the back_index => (front_index + arraylist_size) % arraylist_holder.total_size
void* get_back(const arraylist* al);

// returns the number of elements in the array list
unsigned int get_arraylist_size(const arraylist* al);

// returns 1 if the arraylist is full, else returns 0
int is_arraylist_full(const arraylist* al);
// returns 1 if the arraylist is empty, else returns 0
int is_arraylist_empty(const arraylist* al);

// iterates over all the elements in the arraylist from front to back
void for_each_in_arraylist(const arraylist* al, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params);

void deintialize_arraylist(arraylist* al);

#endif