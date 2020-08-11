#ifndef QUEUE_H
#define QUEUE_H

#include<array.h>

#define push		push_queue
#define pop 		pop_queue
#define get_top 	get_top_queue
#define for_each 	for_each_in_queue

typedef struct queue queue;
struct queue
{
	// array to store queue elements
	array queue_holder;

	// current size of queue, i.e. number of elements in it
	unsigned int queue_size;

	// index, where the oldest element was inserted, the element at this index is popped
	unsigned int earliest_element_index;

	// index, after which a new element will be inserted, this is the index where the latest element was inserted
	// this attribute of the queue is now, computed, and not stored directly in the queue structure
	// unsigned int latest_element_index;
};

// initializes queue and it will depend on initialize_array to give necessary memory to manage internal element contents
void initialize_queue(queue* queue_p, unsigned int expected_size);

// push a new element to the queue
void push(queue* queue_p, const void* data_p);

// pop the element from the queue which was inserted earliest of all other elements
void pop(queue* queue_p);

// returns pointer to the top element data pointer, (i.e. data pointer to the earliest inserted element) 
const void* get_top(queue* queue_p);

// frees all the data being held by the queue, this function, does not release memory of the actual queue structure, only the memory of the components
// the same queue can be reused by calling initialize_queue function, after it is deinitialized
void deinitialize_queue(queue* queue_p);

// if the queue is empty
int isQueueEmpty(queue* queue_p);

// if the queue_holder is full and needs expansion
int isQueueHolderFull(queue* queue_p);

// for each function, to iteration over all non null elements to perform an operation
void for_each(const queue* queue_p, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params);

// prints the queue
void print_queue(queue* queue_p, void (*print_element)(const void* data_p));

#undef push
#undef pop
#undef get_top
#undef for_each

#endif