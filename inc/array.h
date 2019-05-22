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

array* get_array(unsigned long long int initial_size, unsigned long long int size_of_data_element);

void delete_array(array* array_p);

void* get_element(array* array_p, unsigned long long int index);

void append_element(array* array_p, const void* data_p);

void print_array(array* array_p, void (*print_element)(void*));

void sort_array(array* array_p, int (*comparator_function)(void*, void*));

#endif