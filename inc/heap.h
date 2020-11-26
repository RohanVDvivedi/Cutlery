#ifndef HEAP_H
#define HEAP_H

#include<array.h>

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
	unsigned int element_count;

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
// push returns 1, if data_p is successfully pushed
// push returns 0 and fails if the heap container is full
// if push fails you must expand_heap before pushing any new elements
// O(log(N)) operation
int push_heap(heap* heap_p, const void* data);

// pop the top element from the heap
// pop returns 1, if an element is successfully popped
// pop returns 0 and fails if the heap container is already empty and does not have any elements to pop
int pop_heap(heap* heap_p);

// returns pointer to the top data element of the heap, returns NULL if the heap is empty
const void* get_top_heap(const heap* heap_p);

// the below function is to be called when you doubt if the heap properties are being maintained at the given index
// or because you changed the attributes of the data, which changed its ordering
// if your doubt turns out to be true and the heap property is being violated, the heap property would be restored at that place
void heapify_at(heap* heap_p, unsigned int index);

// this function removes an eelement from the heap, at a particular index
// it returns 0, if no element was removed, else it returns 1
// this functions fails with 0 returns, if the index provided is greater than the elements in the heap
// i.e. fails if index is out of bounds, else if the element was removed it will return 1
int remove_from_heap(heap* heap_p, unsigned int index);

void deinitialize_heap(heap* heap_p);

// returns total_size of the heap
unsigned int get_total_size_heap(heap* heap_p);

// returns the number of elements inside the heap
unsigned int get_element_count_heap(heap* heap_p);

// returns 1, if the heap container is full, i.e. it needs to be expanded to push any futher elements
// else it return 0, this means there is still space in the container to push elements
int is_full_heap(heap* heap_p);

// returns 1, if the heap is empty, i.e. does not have any elements to pop
// else it return 0
int is_empty_heap(heap* heap_p);

// expand the heap container, returns 1 on successfull expansion of the heap container
// else returns 0
int expand_heap(heap* heap_p);

// shrink the heap container, returns 1 on successfull shrink of the heap container
// else returns 0
// you may call this function at regular intervals, to keep the heap size in check
int shrink_heap(heap* heap_p);

// perform an operation on all the key value pairs for the heap
void for_each_in_heap(const heap* heap_p, void (*operation)(void* data, unsigned int heap_index, const void* additional_params), const void* additional_params);

// print, heap and all the elements of the heap
void print_heap(heap* heap_p, void (*print_element)(const void* data));

#endif