#ifndef QUEUE_H
#define QUEUE_H

// queue will only hold your pointer as it is,
// queue does not create/clone/update/mutate your data
// queue does not shallow or deep clone it
// you manage(create delete) your own data,

// the below is a array based ring buffer implementation of queue

#include<array.h>

typedef struct queue queue;
struct queue
{
	array* queue_holder;
	unsigned long long int earliest_element_index;
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

#endif