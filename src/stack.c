#include<stack.h>

#include<stdio.h>

void initialize_stack(stack* stack_p, unsigned int initial_size)
{
	initialize_arraylist(&(stack_p->stack_holder), initial_size);
}

int push_stack(stack* stack_p, const void* data_p)
{
	return push_back(&(stack_p->stack_holder), data_p);
}

int pop_stack(stack* stack_p)
{
	return pop_back(&(stack_p->stack_holder));
}

const void* get_top_stack(const stack* stack_p)
{
	return get_back(&(stack_p->stack_holder));
}

const void* get_nth_from_top_stack(const stack* stack_p, unsigned int n)
{
	return get_nth_from_back(&(stack_p->stack_holder), n);
}

void deinitialize_stack(stack* stack_p)
{
	deinitialize_arraylist(&(stack_p->stack_holder));
}

unsigned int get_total_size_stack(const stack* stack_p)
{
	return get_total_size_arraylist(&(stack_p->stack_holder));
}

unsigned int get_element_count_stack(const stack* stack_p)
{
	return get_element_count_arraylist(&(stack_p->stack_holder));
}

int is_full_stack(const stack* stack_p)
{
	return is_full_arraylist(&(stack_p->stack_holder));
}

int is_empty_stack(const stack* stack_p)
{
	return is_empty_arraylist(&(stack_p->stack_holder));
}

int expand_stack(stack* stack_p)
{
	return expand_arraylist(&(stack_p->stack_holder));
}

int shrink_stack(stack* stack_p)
{
	return shrink_arraylist(&(stack_p->stack_holder));
}

void for_each_in_stack(const stack* stack_p, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params)
{
	for_each_in_arraylist(&(stack_p->stack_holder), operation, additional_params);
}

void print_stack(const stack* stack_p, void (*print_element)(const void* data_p))
{
	printf("stack : \n\t");
	print_arraylist(&(stack_p->stack_holder), print_element);
	printf("\n");
	printf("\tthe top element of this stack : ");
	if(get_top_stack(stack_p)!=NULL)
		print_element(get_top_stack(stack_p));
	else
		printf("NULL");
	printf("\n");
}