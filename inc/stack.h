#ifndef STACK_H
#define STACK_H

#include<arraylist.h>

typedef struct stack stack;
struct stack
{
	// arraylist to store stack elements
	arraylist stack_holder;
};

// initializes stack and it will depend on initialize_array to give necessary memory to manage internal element contents
void initialize_stack(stack* stack_p, unsigned int expected_size);

// push a new element to the stack
// push returns 1, if data_p is successfully pushed
// push returns 0 and fails if the stack container is full
// if push fails you must expand_stack before pushing any new elements
int push_stack(stack* stack_p, const void* data_p);

// pop the last inserted element from the stack
// pop returns 1, if an element is successfully popped
// pop returns 0 and fails if the stack container is already empty and does not have any elements to pop
int pop_stack(stack* stack_p);

// returns pointer to the top element data pointer, (i.e. data pointer to the last inserted element) 
const void* get_top_stack(stack* stack_p);
const void* get_nth_from_top_stack(stack* stack_p, unsigned int n);

// frees all the data being held by the stack, this function, does not release memory of the actual stack structure, only the memory of the components
// the same stack can be reused by calling initialize_stack function, after it is deinitialized
void deinitialize_stack(stack* stack_p);

// returns total_size of the stack
unsigned int get_total_size_stack(stack* stack_p);

// returns the number of elements inside the stack
unsigned int get_element_count_stack(stack* stack_p);

// returns 1, if the stack container is full, i.e. it needs to be expanded to push any futher elements
// else it return 0, this means there is still space in the container to push elements
int is_full_stack(stack* stack_p);

// returns 1, if the stack is empty, i.e. does not have any elements to pop
// else it return 0
int is_empty_stack(stack* stack_p);

// expand the stack container, returns 1 on successfull expansion of the stack container
// else returns 0
int expand_stack(stack* stack_p);

// shrink the stack container, returns 1 on successfull shrink of the stack container
// else returns 0
// you may call this function at regular intervals, to keep the stack size in check
int shrink_stack(stack* stack_p);

// for each element of the stack, perform the given operation
void for_each_in_stack(const stack* stack_p, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params);

// prints the stack
void print_stack(stack* stack_p, void (*print_element)(const void* data_p));

#endif