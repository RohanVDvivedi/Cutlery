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
	// from first_index to first_index + ((element_count - 1) % arraylist_holder.total_size)

	// first_index points to the front of the arraylist
	unsigned int first_index;

	// element_count represents the number of elements contained in the array list
	unsigned int element_count;
};

void initialize_arraylist(arraylist* al, unsigned int initial_size);
void initialize_arraylist_with_allocator(arraylist* al, unsigned int initial_size, memory_allocator array_mem_allocator);

// push_front functions will push data_p to the front of the arraylist, and will return 1 on success
int push_front(arraylist* al, const void* data_p);
// push_back functions will push data_p to the back of the arraylist, and will return 1 on success
int push_back(arraylist* al, const void* data_p);
// push_* functions will fail and return 0, if the number of elements in the arraylist is equal to the arraylist_holder.total_size
// this happens because the arraylist does not have additional space to store the new element
// if this happens you may call expand_arraylist to  expand the arrayholder of the arraylist

// pop_front functions will pop a data element from the front of the arraylist, and will return 1 on success
int pop_front(arraylist* al);
// pop_back functions will pop a data element from the front of the arraylist, and will return 1 on success
int pop_back(arraylist* al);
// pop_* functions will fail and return 0, if no element was popped from the arraylist 
// This happens if the number of elements in the given arraylist is 0

// returns an element at the front of the arraylist, this is also the element at the first_index
// it fails and returns NULL, if the 
const void* get_front(const arraylist* al);
// returns an element at the back of the arraylist, this is also the element at the first_index + ((element_count - 1) % arraylist_holder.total_size)
const void* get_back(const arraylist* al);
// get_* functions will fail and return NULL if the number of elements in the given arraylist is 0

// to get nth element from the front of the arraylist
const void* get_nth_from_front(const arraylist* al, unsigned int n);
// to get nth element from the back of the arraylist
const void* get_nth_from_back(const arraylist* al, unsigned int n);
// get_nth_from_* functions will fail and return NULL if n is greater than the number of elements in the given arraylist

// to set nth element from front in the arraylist to data_p pointer
int set_nth_from_front(arraylist* al, const void* data_p, unsigned int n);
// to set nth element from back in the arraylist to data_p pointer
int set_nth_from_back(arraylist* al, const void* data_p, unsigned int n);
// set_nth_from_* functions will fail and return 0 if n is greater than the number of elements in the given arraylist
// on success, these functions will return 1

// returns capacity of the arraylist, i.e. the maximum number of elements that can be accomodated in arraylist
unsigned int get_capacity_arraylist(const arraylist* al);
// returns the number of elements inside the arraylist
unsigned int get_element_count_arraylist(const arraylist* al);

// returns 1 if the arraylist is full (i.e. no new elements can be inserted before expanding), else returns 0
int is_full_arraylist(const arraylist* al);
// returns 1 if the arraylist is empty (i.e. no elements are present), else returns 0
int is_empty_arraylist(const arraylist* al);

// the below functions can be used tto expand or shring the arrayholder of the arraylist
// these functions will return 1, if they succeeds
// else they return 0 on failure
int expand_arraylist(arraylist* al);
int shrink_arraylist(arraylist* al);

// get the data from the arraylist, that equals data, based on the comparator provided
// it will return the pointer to the data contained in the arraylist that compares equal (i.e. compare function returns 0)
const void* find_equals_in_arraylist(const arraylist* al, const void* data, int (*compare)(const void* data1, const void* data2));

// iterates over all the elements in the arraylist from front to back
void for_each_in_arraylist(const arraylist* al, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params);

// serializes the arraylist, and appends the serialized form to the dstring
void sprint_arraylist(dstring* append_str, const arraylist* al, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs);

void deinitialize_arraylist(arraylist* al);

#endif