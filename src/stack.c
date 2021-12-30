#include<stack.h>

#include<cutlery_stds.h>

void initialize_stack(stack* stack_p, unsigned int capacity)
{
	initialize_arraylist(&(stack_p->stack_holder), capacity);
}

void initialize_stack_with_allocator(stack* stack_p, unsigned int capacity, memory_allocator mem_allocator)
{
	initialize_arraylist_with_allocator(&(stack_p->stack_holder), capacity, mem_allocator);
}

int push_to_stack(stack* stack_p, const void* data_p)
{
	return push_back(&(stack_p->stack_holder), data_p);
}

int pop_from_stack(stack* stack_p)
{
	return pop_back(&(stack_p->stack_holder));
}

const void* get_top_of_stack(const stack* stack_p)
{
	return get_back(&(stack_p->stack_holder));
}

const void* get_nth_from_top_of_stack(const stack* stack_p, unsigned int n)
{
	return get_nth_from_back(&(stack_p->stack_holder), n);
}

void remove_all_from_stack(stack* stack_p)
{
	remove_all_from_arraylist(&(stack_p->stack_holder));
}

void deinitialize_stack(stack* stack_p)
{
	deinitialize_arraylist(&(stack_p->stack_holder));
}

unsigned int get_capacity_stack(const stack* stack_p)
{
	return get_capacity_arraylist(&(stack_p->stack_holder));
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

int reserve_capacity_for_stack(stack* stack_p, unsigned int atleast_capacity)
{
	return reserve_capacity_arraylist(&(stack_p->stack_holder), atleast_capacity);
}

void for_each_in_stack(const stack* stack_p, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params)
{
	for_each_in_arraylist(&(stack_p->stack_holder), operation, additional_params);
}

void sprint_stack(dstring* append_str, const stack* stack_p, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs++); snprintf_dstring(append_str, "stack :\n");

	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "stack_holder : \n");
	sprint_arraylist(append_str, &(stack_p->stack_holder), sprint_element, tabs + 1);
	snprintf_dstring(append_str, "\n");

	sprint_chars(append_str, '\t', tabs); 
	snprintf_dstring(append_str, "top : ");
	if(get_top_of_stack(stack_p) != NULL)
		sprint_element(append_str, get_top_of_stack(stack_p), 0);
	else
		snprintf_dstring(append_str, "NULL");
	snprintf_dstring(append_str, "\n");
}