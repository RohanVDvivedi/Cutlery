#ifndef QUEUE_H
#define QUEUE_H

#include<array.h>

#define push    push_queue
#define pop     pop_queue
#define get_top get_top_queue

typedef struct queue queue;
struct queue
{
	// array to store queue elements
	array queue_holder;

	// size of queue
	unsigned long long int queue_size;

	// index, where the oldest element was inserted, the element at this index is popped
	unsigned long long int earliest_element_index;

	// index, after which a new element will be inserted, this is the index where the latest element was inserted
	unsigned long long int latest_element_index;
};

// build and get a new queue
queue* get_queue(unsigned long long int expected_size);

// push a new element to the queue
void push(queue* queue_p, const void* data_p);

// pop the element from the queue which was inserted earliest of all other elements
void pop(queue* queue_p);

// returns pointer to the top element data pointer, (i.e. data pointer to the earliest inserted element) 
const void* get_top(queue* queue_p);

// delete queue and queue_holder array
void delete_queue(queue* queue_p);

// if the queue is empty
int isQueueEmpty(queue* queue_p);

// if the queue_holder is full and needs expansion
int isQueueHolderFull(queue* queue_p);

// prints the queue
void print_queue(queue* queue_p, void (*print_element)(const void* data_p));

#undef push
#undef pop
#undef get_top

#endif