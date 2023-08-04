#ifndef QUEUE_H
#define QUEUE_H

#include<arraylist.h>

typedef struct queue queue;
struct queue
{
	// arraylist to store queue elements
	arraylist queue_holder;
};

// initializes queue and it will depend on initialize_array to give necessary memory to manage internal element contents
// the initialize_queue* functions may fail if the initial memory allocation fails
// in case of a failure, the queue will still be initialized to capacity 0
int initialize_queue(queue* queue_p, cy_uint capacity);
int initialize_queue_with_allocator(queue* queue_p, cy_uint capacity, memory_allocator mem_allocator);
int initialize_queue_with_memory(queue* queue_p, cy_uint capacity, const void* data_ps[]);

// push a new element to the queue
// push returns 1, if data_p is successfully pushed
// push returns 0 and fails if the queue container is full
// if push fails you must expand_queue before pushing any new elements
int push_to_queue(queue* queue_p, const void* data_p);

// pop the element from the queue which was inserted earliest of all other elements
// pop returns 1, if an element is successfully popped
// pop returns 0 and fails if the queue container is already empty and does not have any elements to pop
int pop_from_queue(queue* queue_p);

// returns pointer to the top element data pointer, (i.e. data pointer to the earliest inserted element) 
const void* get_top_of_queue(const queue* queue_p);
const void* get_from_top_of_queue(const queue* queue_p, cy_uint index);

// removes all the elements from queue queue_p
// after a call to this function the queue contains 0 elements (get_element_count() == 0)
// it does not affect the capacity of the queue
void remove_all_from_queue(queue* queue_p);

// frees all the data being held by the queue, this function, does not release memory of the actual queue structure, only the memory of the components
// the same queue can be reused by calling initialize_queue function, after it is deinitialized
void deinitialize_queue(queue* queue_p);

// returns capacity of the queue, i.e. the maximum number of elements that can be accomodated in queue
cy_uint get_capacity_queue(const queue* queue_p);

// returns the number of elements inside the queue
cy_uint get_element_count_queue(const queue* queue_p);

// returns 1, if the queue container is full, i.e. it needs to be expanded to push any futher elements
// else it return 0, this means there is still space in the container to push elements
int is_full_queue(const queue* queue_p);

// returns 1, if the queue is empty, i.e. does not have any elements to pop
// else it return 0
int is_empty_queue(const queue* queue_p);

// expand the queue container, returns 1 on successfull expansion of the queue container
// else returns 0
int expand_queue(queue* queue_p);

// shrink the queue container, returns 1 on successfull shrink of the queue container
// else returns 0
// you may call this function at regular intervals, to keep the queue size in check
int shrink_queue(queue* queue_p);

// expands queue to atleast the capacity of atleast_capacity
// returns 1, only if the queue was expanded
int reserve_capacity_for_queue(queue* queue_p, cy_uint atleast_capacity);

// for each function, to iteration over all non null elements to perform an operation
void for_each_in_queue(const queue* queue_p, void (*operation)(void* data_p, cy_uint index, const void* additional_params), const void* additional_params);

// serializes the queue, and appends the serialized form to the dstring
void sprint_queue(dstring* append_str, const queue* queue_p, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs);

#endif