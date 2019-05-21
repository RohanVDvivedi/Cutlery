#ifndef ARRAY_H
#define ARRAY_H

#include<stdio.h>
#include<stdlib.h>

// the array will maintain all of its memory on its own
// you manage your memory, array will manage its own memory on heap
// you may pass pointers, but that will be just to read your object, promise

struct array
{
	// can be zero
	unsigned long long int size;

	// total number of buckets, can not be 0, at least 1
	unsigned long long int max_size;

	// we maintain array of pointers each of which point to actual data
	void** data_p_p;

	// size of data
	unsigned long long int size_of_data_element;

	// this is the factor, by which the previous size be incremented
	unsigned long long int increment_factor;

	// this is the increment amount
	unsigned long long int increment_offset;

	// new_size = old_size * increment_factor + increment_offset
};

array* get_array(unsigned long long int min_size, unsigned long long int size_of_data_element);

void delete_array(array* array_p);

void* get_element(array* array_p,unsigned long long int i);

void append_element(array* array_p, const void* data_p);

void print_array(array* array_p, void (*print_element)(void*));

void sort_array(array* array_p, int (*comparator_function)(void*, void*));

#endif