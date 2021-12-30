#ifndef ARRAY_H
#define ARRAY_H

#include<dstring.h>
#include<memory_allocator_interface.h>

typedef struct array array;
struct array
{
	// we maintain array of pointers each of which point to actual data
	const void** data_p_p;

	// this many element pointers (const void*) can be accomodated in array
	unsigned int capacity;

	// this is the memory_allocator that will be used for allocating memory for the array
	// for expand or shrink, this function will be called
	memory_allocator mem_allocator;
};

// initializes and gives necessary memory to manage internal element contents
void initialize_array(array* array_p, unsigned int capacity);
void initialize_array_with_allocator(array* array_p, unsigned int capacity, memory_allocator mem_allocator);

// returns pointer to the data at index = index
const void* get_from_array(const array* array_p, unsigned int index);

// returns 1 for success, and fails with a return of 0 for index out of bounds
int set_in_array(array* array_p, const void* data_p, unsigned int index);

// swap elements of array at given indexes
void swap_in_array(array* array_p, unsigned int i1, unsigned int i2);

// removes all the elements from array array_p
// this function effectively NULLs out all the pointers stored within data_p_p
void remove_all_from_array(array* array_p);

// frees all the data being held by the array, this function, does not release memory of the actual array structure, only the memory of the components
// the same array can be reused by calling initialize_array function, after it is deinitialized
void deinitialize_array(array* array_p);

// returns number of elements that can be accomodated in the array
unsigned int get_capacity_array(const array* array_p);

// it expands array
// it returns 1, for success if the array container was successfull expanded,
// else this function may fail with 0, to indicate a failure, from the memory allocator
int expand_array(array* array_p);

// expand array such that it has atleast_capacity after expansion
// returns 1, only if the array was expanded
// returns 0, to indicate a failure from the memory allocator
// this functions may also fail if the array already has capacity more than or equal to atleast_capacity
int reserve_capacity_array(array* array_p, unsigned int atleast_capacity);

// shrinks the array, to a new_capacity
// returns 1, if the array was shrunk to the new_capacity
// else it returns 0 and fails, if it fails (the function fails if new_capacity >= old_capacity or if the allocation fails)
int shrink_array(array* array_p, unsigned int new_capacity);

// perform operation on all the elements of the array, the method operation takes in 2 params, the data_p to operation on and its index
void for_each_in_array(const array* array_p, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params);
//also ->
void for_each_non_null_in_array(const array* array_p, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params);

// serializes the array, and appends the serialized form to the dstring
void sprint_array(dstring* append_str, const array* array_p, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs);

#include<array_search_sort.h>

#endif