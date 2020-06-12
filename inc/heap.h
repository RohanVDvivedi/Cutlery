#ifndef HEAP_H
#define HEAP_H

#include<stdio.h>
#include<stdlib.h>

#include<array.h>

#define push 			push_heap
#define pop 			pop_heap
#define get_top 		get_top_heap
#define for_each 		for_each_in_heap

typedef enum heap_type heap_type;
enum heap_type
{
	MIN_HEAP,
	MAX_HEAP
};

typedef struct heap heap;
struct heap
{
	// the type of heap, a heap can be a min heap or a max heap
	// as a MIN_HEAP or MAX_HEAP
	heap_type type;

	// compare data provided and returns 0 if they are same, else non-zero
	// it returns 0 if they are same, >0 if data1 is greater than data2 else it must return <0 value
	int (*compare)(const void* data1, const void* key2);

	// to store pointers to the elements of the heap
	array heap_holder;
	
	// number of elements in the heap
	unsigned int heap_size;

	// heap_index_update
	// everytime heap updates the position/index of a data in heap, it will make a call to this function
	// it can be provided as NULL as well, if you do not need this functionality
	// you may use this functionality to cache the heap_index of any element, and later call heapify on the index, to restore heap property, after updating the index
	// please keep this method as small as possible, to ensure overall O(log(n)) push and pop execution costs
	void (*heap_index_update_callback)(const void* data, unsigned int heap_index, const void* callback_params);
	const void* callback_params;
};

// initializes heap and it will depend on initialize_array to give necessary memory to manage internal element contents
void initialize_heap(heap* heap_p, unsigned int expected_size, heap_type type, int (*compare)(const void* data1, const void* data2), void (*heap_index_update_callback)(const void* data, unsigned int heap_index, const void* callback_params), const void* callback_params);

// push a new data element to the heap
// O(log(N)) operation
void push(heap* heap_p, const void* data);

// returns pointer to the minimum data of the heap, returns NULL if the heap is empty
const void* get_top(const heap* heap_p);

// pop the top element from the heap, no element will be popped if the heap is already empty
void pop(heap* heap_p);

// the below function is to be called when you doubt if the heap properties are being maintained at the given index
// or because you changed the attributes of the data, which changed its ordering
// if your doubt turns out to be true and the heap property is being violated, the heap property would be restored at that place
void heapify_at(heap* heap_p, unsigned int index);

// perform an operation on all the key value pairs for the heap
void for_each(const heap* heap_p, void (*operation)(void* data, unsigned int heap_index, const void* additional_params), const void* additional_params);

// print, heap and all the elements of the heap
void print_heap(heap* heap_p, void (*print_element)(const void* data));

void deinitialize_heap(heap* heap_p);

#undef push
#undef pop
#undef get_top
#undef for_each

#endif