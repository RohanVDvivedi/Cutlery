#include<stack.h>

#define push		push_stack
#define pop			pop_stack
#define get_top		get_top_stack
#define for_each 	for_each_in_stack

stack* get_stack(unsigned long long int expected_size)
{
	stack* stack_p = calloc(1, sizeof(stack));
	initialize_stack(stack_p, expected_size);
	return stack_p;
}

void initialize_stack(stack* stack_p, unsigned long long int expected_size)
{
	initialize_array(&(stack_p->stack_holder), expected_size + 1);
	stack_p->stack_size = 0;
}

void push(stack* stack_p, const void* data_p)
{
	// expand stack holder if necessary
	if(stack_p->stack_size >= stack_p->stack_holder.total_size)
	{
		expand_array(&(stack_p->stack_holder));
	}

	// set the element to the last index and increment its size
	set_element(&(stack_p->stack_holder), data_p, stack_p->stack_size++);
}

void pop(stack* stack_p)
{
	// we can pop only if the stack size if greater than 0, and there is atleast 1 element
	if(stack_p->stack_size > 0)
	{
		// set the last element to null
		set_element(&(stack_p->stack_holder), NULL, --stack_p->stack_size);

		// let the array be shrunk if it is required
		shrink_array(&(stack_p->stack_holder), 0, stack_p->stack_size - 1);
		// Note: we shrink the holder, only if we sucessfully pop the element
	}
}

const void* get_top(stack* stack_p)
{
	// return the last element from the stack holder
	return stack_p->stack_size > 0 ? get_element(&(stack_p->stack_holder), stack_p->stack_size - 1) : NULL;
}

void deinitialize_stack(stack* stack_p)
{
	deinitialize_array(&(stack_p->stack_holder));
	stack_p->stack_size = 0;
}

void delete_stack(stack* stack_p)
{
	deinitialize_stack(stack_p);
	free(stack_p);
}

void for_each(const stack* stack_p, void (*operation)(void* data_p, unsigned long long int index, const void* additional_params), const void* additional_params)
{
	for_each_non_null_in_array(&(stack_p->stack_holder), operation, additional_params);
}

void print_stack(stack* stack_p, void (*print_element)(const void* data_p))
{
	printf("stack : \n");
	printf("\tstack_size : %llu\n", stack_p->stack_size);
	printf("\tstack array : ");print_array(&(stack_p->stack_holder), print_element);printf("\n");
	printf("\tthe top element : ");
	if(get_top(stack_p)!=NULL)
	{
		print_element(get_top(stack_p));
	}
	else
	{
		printf("NULL");
	}
	printf("\n");
}

#undef push
#undef pop
#undef get_top
#undef for_each
