#ifndef STACK_H
#define STACK_H

// stack will only hold your pointer as it is,
// stack does not create/clone/update/mutate your data
// stack does not shallow or deep clone it
// you manage(create delete) your own data,

// the below is a array based stack implementation of stack

typedef struct stack stack;
struct stack
{
	array* stackHolder;
	unsigned long long int lastElementIndex;
};

// build and get a new stack
stack* get_stack();

// push a new element to the stack
void push(stack* stack_p, const void* data_p);

// pop the last inserted element from the stack
void pop(stack* stack_p);

// returns pointer to the top element data pointer, (i.e. data pointer to the last inserted element) 
const void* get_top(stack* stack_p);

// delete stack and stackHolder array
void delete_stack(stack* stack_p);

#endif