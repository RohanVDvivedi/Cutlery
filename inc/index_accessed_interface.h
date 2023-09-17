#ifndef INDEX_ACCESSED_INTERFACE_H
#define INDEX_ACCESSED_INTERFACE_H

#include<cutlery_stds.h>

/*
	below is an interface to be implemented by any data structure,
	that wants to be and can be randomly accessed.

	implementing this interface allows you to
	search and sort the underlying data structure using the "indexed_search_sort.h" functions.
*/

typedef struct index_accessed_interface index_accessed_interface;
struct index_accessed_interface
{
	// the datastructure that implements it
	void* ds_p;

	// get element by its index
	const void* (*get_element)(const void* ds_p, cy_uint index);

	// set element at index
	int (*set_element)(void* ds_p, const void* data_p, cy_uint index);

	// swap elements at indices i1 and i2
	int (*swap_elements)(void* ds_p, cy_uint i1, cy_uint i2);

	// returns the number of elements in the data structure;
	cy_uint (*get_element_count)(const void* ds_p);
};

#endif