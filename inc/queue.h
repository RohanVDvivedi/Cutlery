#ifndef QUEUE_H
#define QUEUE_H

#include<array.h>

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
// push returns 1, if data_p is successfully pushed
// push returns 0 and fails if the queue container is full
// if push fails you must expand_queue before pushing any new elements
int push_queue(queue* queue_p, const void* data_p);

// pop the element from the queue which was inserted earliest of all other elements
// pop returns 1, if an element is successfully popped
// pop returns 0 and fails if the queue container is already empty and does not have any elements to pop
int pop_queue(queue* queue_p);

// returns pointer to the top element data pointer, (i.e. data pointer to the earliest inserted element) 
const void* get_top_queue(queue* queue_p);
const void* get_nth_from_top_queue(queue* queue_p, unsigned int n);

// frees all the data being held by the queue, this function, does not release memory of the actual queue structure, only the memory of the components
// the same queue can be reused by calling initialize_queue function, after it is deinitialized
void deinitialize_queue(queue* queue_p);

// returns total_size of the queue
unsigned int get_total_size_queue(queue* queue_p);

// returns the number of elements inside the queue
unsigned int get_element_count_queue(queue* queue_p);

// returns 1, if the queue container is full, i.e. it needs to be expanded to push any futher elements
// else it return 0, this means there is still space in the container to push elements
int is_full_queue(queue* queue_p);

// returns 1, if the queue is empty, i.e. does not have any elements to pop
// else it return 0
int is_empty_queue(queue* queue_p);

// expand the queue container, returns 1 on successfull expansion of the queue container
// else returns 0
int expand_queue(queue* queue_p);

// shrink the queue container, returns 1 on successfull shrink of the queue container
// else returns 0
// you may call this function at regular intervals, to keep the queue size in check
int shrink_queue(queue* queue_p);

// for each function, to iteration over all non null elements to perform an operation
void for_each_in_queue(const queue* queue_p, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params);

// prints the queue
void print_queue(queue* queue_p, void (*print_element)(const void* data_p));

#endif