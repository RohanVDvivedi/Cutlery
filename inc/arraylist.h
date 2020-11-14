#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include<array.h>

// the front and back of the arraylist is synonymous to head and tail of the linkedlist
// arraylist is a circular buffer

typedef struct arraylist arraylist;
struct arraylist
{
	// this is the holder for storing the elements in the arraylist
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

// push_front & push_back functions will push data_p to the front and back of the arraylist, respectively
void push_front(arraylist* al, const void* data_p);
void push_back(arraylist* al, const void* data_p);

// pop_front & pop_back functions will pop a data element from the front and back of the arraylist, respectively
void pop_front(arraylist* al);
void pop_back(arraylist* al);

// returns an element at the front of the arraylist, this is also the element at the front_index
const void* get_front(const arraylist* al);
// returns an element at the back of the arraylist, this is also the element at the back_index => (front_index + arraylist_size) % arraylist_holder.total_size
const void* get_back(const arraylist* al);

// to get nth element from front or back respectively
const void* get_nth_from_front(const arraylist* al);
const void* get_nth_from_back(const arraylist* al);

// returns the number of elements in the array list
unsigned int get_arraylist_size(const arraylist* al);

// returns 1 if the arraylist is full, else returns 0
int is_arraylist_full(const arraylist* al);
// returns 1 if the arraylist is empty, else returns 0
int is_arraylist_empty(const arraylist* al);

// get the data from the arraylist, that equals data, based on the comparator provided
// in the compare function the first parameter is the data from the linkedlist,
// while the second parameter is the data that has been provided by you
// it will return the pointer to the linkedlist data that compares equal (i.e. compare function returns 0)
const void* find_equals_in_arraylist(const linkedlist* ll, const void* data, int (*compare)(const void* ll_data, const void* data));

// iterates over all the elements in the arraylist from front to back
void for_each_in_arraylist(const arraylist* al, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params);

void print_arraylist(const arraylist* al);

void deintialize_arraylist(arraylist* al);

#endif