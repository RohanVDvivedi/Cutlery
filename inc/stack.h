#ifndef STACK_H
#define STACK_H

#include<array.h>

typedef struct stack stack;
struct stack
{
	// array to store stack elements
	array stack_holder;

	// number of elements currently in stack
	unsigned int stack_size;
};

// initializes stack and it will depend on initialize_array to give necessary memory to manage internal element contents
void initialize_stack(stack* stack_p, unsigned int expected_size);

// push a new element to the stack
void push_stack(stack* stack_p, const void* data_p);

// pop the last inserted element from the stack
void pop_stack(stack* stack_p);

// returns pointer to the top element data pointer, (i.e. data pointer to the last inserted element) 
const void* get_top_stack(stack* stack_p);

// frees all the data being held by the stack, this function, does not release memory of the actual stack structure, only the memory of the components
// the same stack can be reused by calling initialize_stack function, after it is deinitialized
void deinitialize_stack(stack* stack_p);

// for each element of the stack, perform the given operation
void for_each_in_stack(const stack* stack_p, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params);

// prints the stack
void print_stack(stack* stack_p, void (*print_element)(const void* data_p));

#endif