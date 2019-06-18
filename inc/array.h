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
	// this many elements can be accomodated in array, without expanding
	// expanding which involves copying pointers, and
	// requesting larger pointer array size than previously we had
	// this could get expensive and is non productive computation
	unsigned long long int total_size;

	// we maintain array of pointers each of which point to actual data
	void** data_p_p;

	// size of data, stored in each element of the array
	// array is homogenous ofcourse, because its array
	unsigned long long int size_of_data_element;

	// this is the factor, by which the previous size of data_p_p will be incremented
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
const void* get_element(const array* array_p, unsigned long long int index);

// if index element is NULL, allocates new memory sets new element there 
// and deletes old data permanently
// if not null, old data memory is rewritten, with new data
// returns 0 for success
int set_element(array* array_p, const void* data_p, unsigned long long int index);

// it expands array, as per the array rules
void expand_array(array* array_p);

// prints the array
void print_array(array* array_p, void (*print_element)(const void* data_p));

#endif