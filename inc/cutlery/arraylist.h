#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include<cutlery/array.h>
#include<cutlery/comparator_interface.h>
#include<cutlery/index_accessed_search_sort.h>

// the front and back of the arraylist is synonymous to head and tail of the linkedlist
// arraylist is a circular buffer

typedef struct arraylist arraylist;
struct arraylist
{
	// this is the holder for storing the elements in the arraylist
	array arraylist_holder;

	// arraylist is a circular buffer
	// from first_index to ((first_index + element_count - 1) % arraylist_holder.total_size)

	// first_index points to the front of the arraylist
	cy_uint first_index;

	// element_count represents the number of elements contained in the array list
	cy_uint element_count;
};

// the initialize_arraylist* functions may fail if the initial memory allocation fails
// in case of a failure, the arraylist will still be initialized to capacity 0
int initialize_arraylist(arraylist* al, cy_uint capacity);
int initialize_arraylist_with_allocator(arraylist* al, cy_uint capacity, memory_allocator mem_allocator);
int initialize_arraylist_with_memory(arraylist* al, cy_uint capacity, const void* data_ps[]);

// push_*_to_arraylist functions will push data_p to the front/back of the arraylist, and will return 1 on success
int push_front_to_arraylist(arraylist* al, const void* data_p);
int push_back_to_arraylist(arraylist* al, const void* data_p);
// push_*_to_arraylist functions will fail and return 0, if the arraylist is_full

// pop_*_from_arraylist functions will pop a data element from the front/back of the arraylist, and will return 1 on success
int pop_front_from_arraylist(arraylist* al);
int pop_back_from_arraylist(arraylist* al);
// pop_*_from_arraylist functions will fail and return 0, if arraylist is_empty

// returns an element at the front/back of the arraylist
// it fails and returns NULL, if the arraylist is empty
const void* get_front_of_arraylist(const arraylist* al);
const void* get_back_of_arraylist(const arraylist* al);
// get_*_of_arraylist functions will fail and return NULL if the number of elements in the given arraylist is 0

// to get nth element from the front/back of the arraylist
const void* get_from_front_of_arraylist(const arraylist* al, cy_uint index);
const void* get_from_back_of_arraylist(const arraylist* al, cy_uint index);
// get_from_*_of_arraylist functions will fail and return NULL if index is greater than element_count of the arraylist

// default get function
#define get_from_arraylist get_from_front_of_arraylist

// to set nth element from front/back in the arraylist to data_p pointer
int set_from_front_in_arraylist(arraylist* al, const void* data_p, cy_uint index);
int set_from_back_in_arraylist(arraylist* al, const void* data_p, cy_uint index);
// set_nth_from_*_in_arraylist functions will fail and return 0 if index is greater than element_count of the arraylist

// default set function
#define set_in_arraylist set_from_front_in_arraylist

// swap elements from front or back in the arraylist
int swap_from_front_in_arraylist(arraylist* al, cy_uint i1, cy_uint i2);
int swap_from_back_in_arraylist(arraylist* al, cy_uint i1, cy_uint i2);
// returns 0, failure, if either of i1 or i2 are out of bounds

// default swap function
#define swap_in_arraylist swap_from_front_in_arraylist

// insert `NULL_count_to_insert` number of NULLs at given index (when indexes from front of back)
// if index == element_count, then NULLs are inserted at the end of the arraylist (from front or back)
// these functions are O(N) (obviously)
int insert_NULLs_from_front_in_arraylist(arraylist* al, cy_uint index, cy_uint NULL_count_to_insert);
int insert_NULLs_from_back_in_arraylist(arraylist* al, cy_uint index, cy_uint NULL_count_to_insert);

// remove `element_count_to_remove` elements from front or back of arraylist, at given index (indexed from front or back respectively)
// these functions are O(N) (obviously)
int remove_elements_from_front_of_arraylist(arraylist* al, cy_uint index, cy_uint element_count_to_remove);
int remove_elements_from_back_of_arraylist(arraylist* al, cy_uint index, cy_uint element_count_to_remove);

// returns capacity of the arraylist, i.e. the maximum number of elements that can be accomodated in arraylist
cy_uint get_capacity_arraylist(const arraylist* al);
// returns the number of elements inside the arraylist
cy_uint get_element_count_arraylist(const arraylist* al);

// returns 1 if the arraylist is full, else returns 0
int is_full_arraylist(const arraylist* al);
// returns 1 if the arraylist is empty, else returns 0
int is_empty_arraylist(const arraylist* al);

// the below functions can be used to expand or shring the arrayholder of the arraylist
// these functions will return 1, if they succeeds
// else they return 0 on failure
int expand_arraylist(arraylist* al);
int shrink_arraylist(arraylist* al);

// expands arraylist to atleast the capacity of atleast_capacity
// returns 1, only if the arraylist was expanded
int reserve_capacity_for_arraylist(arraylist* al, cy_uint atleast_capacity);

// get the data from the arraylist, that equals data, based on the comparator provided
// it will return the pointer to the data contained in the arraylist that compares equal (i.e. compare function returns 0)
// "data" becomes the second parameter of the comparator
const void* find_equals_in_arraylist(const arraylist* al, const void* data, const comparator_interface* comparator);

// iterates over all the elements in the arraylist from front to back
void for_each_in_arraylist(const arraylist* al, void (*operation)(void* data_p, cy_uint index, const void* additional_params), const void* additional_params);

// serializes the arraylist, and appends the serialized form to the dstring
void sprint_arraylist(dstring* append_str, const arraylist* al, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs);

// removes all the elements from arraylist al
// after a call to this function the arraylist contains 0 elements (get_element_count() == 0)
// it does not affect the capacity of the arraylist
void remove_all_from_arraylist(arraylist* al);

void deinitialize_arraylist(arraylist* al);

// NOTE:: DO NOT MODIFY THE ARRAYLIST DURING THE ACTIVE LIFE TIME OF THE INDEX_ACCESSED_INTERFACE OF THAT ARRAYLIST

index_accessed_interface get_index_accessed_interface_for_front_of_arraylist(arraylist* al);
index_accessed_interface get_index_accessed_interface_for_back_of_arraylist(arraylist* al);

// the below function can be used to generate a circular arraylist slice from an array
// with first_index and element_count set as is (if valid)
// returns 0, on a failure and 1 for success
// NOTE:: This function has come into existence to only allow inplace sorting of an array just like an arraylist by heap_sort_arraylist function given in arraylist_sort.h
int get_slice_as_arraylist_from_array(arraylist* slice, array* array_p, cy_uint first_index, cy_uint element_count);

#endif