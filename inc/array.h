#ifndef ARRAY_H
#define ARRAY_H

#include<cutlery_stds.h>
#include<dstring.h>
#include<memory_allocator_interface.h>

typedef struct array array;
struct array
{
	// we maintain array of pointers each of which point to actual data
	const void** data_p_p;

	// capacity_in_bytes = number of bytes pointed by data_p_p
	// capacity_in_bytes / sizeof(void*) = number of element pointers (const void*) that can be accomodated in this array
	cy_uint capacity_in_bytes;

	// this is the memory_allocator that will be used for allocating memory for the array
	// for expand or shrink, allocation functions pointed to by this allocator will be called
	memory_allocator mem_allocator;
};

// returns maximum possible capacity of the array on your hardware
#define MAX_ARRAY_CAPACITY (CY_UINT_MAX / sizeof(void*))

// initializes and gives necessary memory to manage internal element contents
// initialize functions may fail, if the initial memory allocation fails
// in case of such a failure, the array still gets initialized to capacity 0
int initialize_array(array* array_p, cy_uint capacity);
int initialize_array_with_allocator(array* array_p, cy_uint capacity, memory_allocator mem_allocator);
int initialize_array_with_memory(array* array_p, cy_uint capacity, const void* data_ps[]);

// returns pointer to the data at index = index
const void* get_from_array(const array* array_p, cy_uint index);

// returns 1 for success, and fails with a return of 0 for index out of bounds
int set_in_array(array* array_p, const void* data_p, cy_uint index);

// swap elements of array at given indexes
int swap_in_array(array* array_p, cy_uint i1, cy_uint i2);

// this function effectively NULLs out all the pointers stored within data_p_p
void remove_all_from_array(array* array_p);
void set_all_NULL_in_array(array* array_p);

// this function effectively NULLs out the "element_count_to_NULL" number of pointers, starting at start_index in array_p
// it returns 0 and fails, if any of the indices from start_index to (start_index + element_count_to_NULL - 1) are out of bounds
// upon failure none of the elements are NULLed
int set_NULLs_in_array(array* array_p, cy_uint start_index, cy_uint element_count_to_NULL);

// it either copies all the elements (returning 1) or it does not copy any elements (returning 0)
// array_p and array_from_p can be the same arrays
// this function fails if we may get out of bounds while accessing any of the elements, that we are suppossed to access
int copy_elements_from_array(array* array_p, cy_uint start_index, const array* array_from_p, cy_uint start_from_index, cy_uint elements_to_copy);

// frees all the data being held by the array, this function, does not release memory of the actual array structure, only the memory of the components
// the same array can be reused by calling initialize_array function, after it is deinitialized
void deinitialize_array(array* array_p);

// returns number of elements that can be accomodated in the array
cy_uint get_capacity_array(const array* array_p);

// it expands array
// it returns 1, for success if the array container was successfull expanded,
// else this function may fail with 0, to indicate a failure, from the memory allocator
int expand_array(array* array_p);

// this is the capacity that an array will expand to, upon calling expand_array
cy_uint get_new_expansion_capacity_for_array(cy_uint current_capacity);

// expand array such that it has atleast_capacity after expansion
// returns 1, only if the array was expanded
// returns 0, to indicate a failure from the memory allocator
// this functions may also fail if the array already has capacity more than or equal to atleast_capacity
int reserve_capacity_for_array(array* array_p, cy_uint atleast_capacity);

// shrinks the array, to a new_capacity
// returns 1, if the array was shrunk to the new_capacity
// else it returns 0 and fails, if it fails (the function fails if new_capacity >= old_capacity or if the allocation fails)
int shrink_array(array* array_p, cy_uint new_capacity);

// perform operation on all the elements of the array, the method operation takes in 2 params, the data_p to operation on and its index
void for_each_in_array(const array* array_p, void (*operation)(void* data_p, cy_uint index, const void* additional_params), const void* additional_params);
//also ->
void for_each_non_null_in_array(const array* array_p, void (*operation)(void* data_p, cy_uint index, const void* additional_params), const void* additional_params);

// serializes the array, and appends the serialized form to the dstring
void sprint_array(dstring* append_str, const array* array_p, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs);

#include<indexed_search_sort.h>

// NOTE:: DO NOT MODIFY THE ARRAY DURING THE ACTIVE LIFE TIME OF THE INDEX_ACCESSED_INTERFACE OF THAT ARRAY

index_accessed_interface get_index_accessed_interface_for_array(array* array_p);

#endif