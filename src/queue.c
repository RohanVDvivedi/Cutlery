#include<queue.h>

#include<cutlery_stds.h>

void initialize_queue(queue* queue_p, unsigned int initial_size)
{
	initialize_arraylist(&(queue_p->queue_holder), initial_size);
}

void initialize_queue_with_allocator(queue* queue_p, unsigned int initial_size, memory_allocator array_mem_allocator)
{
	initialize_arraylist_with_allocator(&(queue_p->queue_holder), initial_size, array_mem_allocator);
}

int push_queue(queue* queue_p, const void* data_p)
{
	return push_back(&(queue_p->queue_holder), data_p);
}

int pop_queue(queue* queue_p)
{
	return pop_front(&(queue_p->queue_holder));
}

const void* get_top_queue(const queue* queue_p)
{
	return get_front(&(queue_p->queue_holder));
}

const void* get_nth_from_top_queue(const queue* queue_p, unsigned int n)
{
	return get_nth_from_front(&(queue_p->queue_holder), n);
}

void deinitialize_queue(queue* queue_p)
{
	deinitialize_arraylist(&(queue_p->queue_holder));
}

unsigned int get_total_size_queue(const queue* queue_p)
{
	return get_total_size_arraylist(&(queue_p->queue_holder));
}

unsigned int get_element_count_queue(const queue* queue_p)
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

void for_each_in_queue(const queue* queue_p, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params)
{
	for_each_in_arraylist(&(queue_p->queue_holder), operation, additional_params);
}

void sprint_queue(dstring* append_str, const queue* queue_p, void (*sprint_element)(dstring* append_str, const void* data_p), unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs++); snprintf_dstring(append_str, "queue :\n");

	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "queue_holder : \n");
	sprint_arraylist(append_str, &(queue_p->queue_holder), sprint_element, tabs + 1);
	snprintf_dstring(append_str, "\n");

	sprint_chars(append_str, '\t', tabs); 
	snprintf_dstring(append_str, "top : ");
	if(get_top_queue(queue_p) != NULL)
		sprint_element(append_str, get_top_queue(queue_p));
	else
		snprintf_dstring(append_str, "NULL");
	snprintf_dstring(append_str, "\n");
}