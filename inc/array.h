#ifndef ARRAY_H
#define ARRAY_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// the array will maintain all of its memory on its own
// you manage your memory, array will manage its own memory on heap
// you may pass pointers, but that will be just to read your object, promise

typedef struct array array;
struct array
{
	// this many elements exist in array, presently
	unsigned long long int occupied_size;

	// this many elements can be accomodated in array, without resizing data_p_p
	// which involves copying pointers, and
	// requesting larger pointer array size then previously we had
	// this could get expensive and is non productive computation
	unsigned long long int total_size;

	// we maintain array of pointers each of which point to actual data
	void** data_p_p;

	// size of data, stored in each element of the array
	// array is homogenous ofc, because its array
	unsigned long long int size_of_data_element;

	// this is the factor, by which the previous size of data_p_p be incremented
	unsigned long long int increment_factor;

	// this is the constant increment amount, over the increment factor
	unsigned long long int increment_offset;

	// new_total_size of data_p_p = old_total_size of data_p_p * increment_factor + increment_offset
};

// returns a new array with total_size as the initial size, with no elements inside,
// the size of each element could be size_of_data_element
array* get_array(unsigned long long int initial_size, unsigned long long int size_of_data_element);

// deletes all the memory allocated by the array and its ownself
void delete_array(array* array_p);

// returns pointer to the data at index = index
void* get_element(array* array_p, unsigned long long int index);

// adds the lement at the end of the array, as the last element
void append_element(array* array_p, const void* data_p);

// removes the element at the index index
void remove_element(array* array_p, unsigned long long int index);

// prints the array
void print_array(array* array_p, void (*print_element)(void*));

// sorts the array, in place
void sort_array(array* array_p, int (*comparator_function)(void*, void*));

// returns new array consisting of data elements, which is returned after passing the each of the data element through map_function
array* map(array* array_p, void (*map_function)(void* result,void* data_p), unsigned long long int size_of_new_data_element);

// reduce all the elements of the array, to produce one output, which is of the same type as the array_element, and is returned
void* reduce(array* array_p, void (*reduce_function)(void* result, void* data_p1));

// applies a function on all the elements of the array, and does not produce any output
void for_each(array* array_p, void (*for_each_function)(void* data_p));

#endif