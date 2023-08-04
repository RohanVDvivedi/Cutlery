#include<queue.h>

#include<cutlery_stds.h>

int initialize_queue(queue* queue_p, cy_uint capacity)
{
	return initialize_arraylist(&(queue_p->queue_holder), capacity);
}

int initialize_queue_with_allocator(queue* queue_p, cy_uint capacity, memory_allocator mem_allocator)
{
	return initialize_arraylist_with_allocator(&(queue_p->queue_holder), capacity, mem_allocator);
}

int initialize_queue_with_memory(queue* queue_p, cy_uint capacity, const void* data_ps[])
{
	return initialize_arraylist_with_memory(&(queue_p->queue_holder), capacity, data_ps);
}

int push_to_queue(queue* queue_p, const void* data_p)
{
	return push_back_to_arraylist(&(queue_p->queue_holder), data_p);
}

int pop_from_queue(queue* queue_p)
{
	return pop_front_from_arraylist(&(queue_p->queue_holder));
}

const void* get_top_of_queue(const queue* queue_p)
{
	return get_front_of_arraylist(&(queue_p->queue_holder));
}

const void* get_from_top_of_queue(const queue* queue_p, cy_uint index)
{
	return get_from_front_of_arraylist(&(queue_p->queue_holder), index);
}

void remove_all_from_queue(queue* queue_p)
{
	remove_all_from_arraylist(&(queue_p->queue_holder));
}

void deinitialize_queue(queue* queue_p)
{
	deinitialize_arraylist(&(queue_p->queue_holder));
}

cy_uint get_capacity_queue(const queue* queue_p)
{
	return get_capacity_arraylist(&(queue_p->queue_holder));
}

cy_uint get_element_count_queue(const queue* queue_p)
{
	return get_element_count_arraylist(&(queue_p->queue_holder));
}

int is_full_queue(const queue* queue_p)
{
	return is_full_arraylist(&(queue_p->queue_holder));
}

int is_empty_queue(const queue* queue_p)
{
	return is_empty_arraylist(&(queue_p->queue_holder));
}

int expand_queue(queue* queue_p)
{
	return expand_arraylist(&(queue_p->queue_holder));
}

int shrink_queue(queue* queue_p)
{
	return shrink_arraylist(&(queue_p->queue_holder));
}

int reserve_capacity_for_queue(queue* queue_p, cy_uint atleast_capacity)
{
	return reserve_capacity_for_arraylist(&(queue_p->queue_holder), atleast_capacity);
}

void for_each_in_queue(const queue* queue_p, void (*operation)(void* data_p, cy_uint index, const void* additional_params), const void* additional_params)
{
	for_each_in_arraylist(&(queue_p->queue_holder), operation, additional_params);
}

void sprint_queue(dstring* append_str, const queue* queue_p, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs++); snprintf_dstring(append_str, "queue :\n");

	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "queue_holder : \n");
	sprint_arraylist(append_str, &(queue_p->queue_holder), sprint_element, tabs + 1);
	snprintf_dstring(append_str, "\n");

	sprint_chars(append_str, '\t', tabs); 
	snprintf_dstring(append_str, "top : ");
	if(get_top_of_queue(queue_p) != NULL)
		sprint_element(append_str, get_top_of_queue(queue_p), 0);
	else
		snprintf_dstring(append_str, "NULL");
	snprintf_dstring(append_str, "\n");
}