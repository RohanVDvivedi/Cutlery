#ifndef STACK_H
#define STACK_H

#include<array.h>

#define push    push_stack
#define pop     pop_stack
#define get_top get_top_stack

typedef struct stack stack;
struct stack
{
	array stack_holder;
	unsigned long long int stack_size;
};

// build and get a new stack
stack* get_stack(unsigned long long int expected_size);

// push a new element to the stack
void push(stack* stack_p, const void* data_p);

// pop the last inserted element from the stack
void pop(stack* stack_p);

// returns pointer to the top element data pointer, (i.e. data pointer to the last inserted element) 
const void* get_top(stack* stack_p);

// delete stack and stack_holder array
void delete_stack(stack* stack_p);

// prints the stack
void print_stack(stack* stack_p, void (*print_element)(const void* data_p));

#undef push
#undef pop
#undef get_top

#endif