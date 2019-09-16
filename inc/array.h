#ifndef ARRAY_H
#define ARRAY_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// the array will maintain all of its memory on its own
// you manage your memory, you manage (create and free) the memory pointed to by data_p pointers (passed in array by set_element)
// the pointer you pass will be stored internally and managed by the array
// array does not touch/clone/modify your data, array just stores your pointers in a dynamic array
// array is just an implementation to dynamic array of all of your pointers

typedef struct array array;
struct array
{
	// this many elements can be accomodated in array, without expanding
	// expanding which involves copying pointers, and
	// requesting larger pointer array size than previously we had
	// this could get expensive and is non productive computation
	unsigned long long int total_size;

	// we maintain array of pointers each of which point to actual data
	const void** data_p_p;

	// this is the factor, by which the previous size of data_p_p will be incremented
	unsigned long long int increment_factor;

	// this is the constant increment amount, over the increment factor
	unsigned long long int increment_offset;

	// new_total_size of data_p_p = old_total_size of data_p_p * increment_factor + increment_offset
};

// returns a new array with total_size as the initial size, with no elements inside,
// the size of each element could be size_of_data_element
array* get_array(unsigned long long int initial_size);

// deletes all the memory allocated by the array and its ownself
void delete_array(array* array_p);

// returns pointer to the data at index = index
const void* get_element(const array* array_p, unsigned long long int index);

// if index element is NULL, allocates new memory sets new element there 
// and deletes old data permanently
// if not null, old data memory is rewritten, with new data
// returns 0 for success
int set_element(array* array_p, const void* data_p, unsigned long long int index);

// perform operation on all the elements of the array, the method operation takes in 2 params, the data_p to operation on and its index
void for_each_in_array(const array* array_p, void (*operation)(void* data_p, unsigned long long int index, const void* additional_params), const void* additional_params);

// returns index of the element found in array_p, if compare between the element and data_p returns 0
// compare > 0, if data_p1 > data_p2 else compare < 0
unsigned long long int find_first_in_array(const array* array_p, void* data_p, int (*compare)(const void* data_p1, const void* data_p2));

// it expands array, as per the array rules
void expand_array(array* array_p);

// prints the array
void print_array(const array* array_p, void (*print_element)(const void* data_p));

#endif