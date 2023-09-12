#ifndef VALUE_ARRAYLIST_H
#define VALUE_ARRAYLIST_H

#include<cutlery_stds.h>
#include<cutlery_math.h>
#include<memory_allocator_interface.h>
#include<index_accessed_interface.h>
#include<circular_buffer_array_util.h>
#include<n-ary_tree_as_array_util.h>
#include<heap_info.h>

#include<dstring.h>

/*
	value_arraylist is a header that will build a cache friendly container for you
	it is suppossed to be used with only small ephemeral structs and primitive types,
	that have no immovable attributes like (synchronization primitives like) mutex, conditional variables or semaphores
*/

#define declarations_value_arraylist(container, contained_type)                                                                \
typedef struct container container;                                                                                            \
struct container                                                                                                               \
{                                                                                                                              \
	contained_type* data_p;         /* array, each element being od container_type */                                          \
	cy_uint first_index;            /* the index of the first element in the container */                                      \
	cy_uint element_count;          /* number of elements in the container */                                                  \
	cy_uint capacity_in_bytes;      /* capacity in bytes pointed by the holder */                                              \
	memory_allocator mem_allocator; /* allocator for the container */                                                          \
};                                                                                                                             \
                                                                                                                               \
/* initialization functions */                                                                                                 \
int initialize_ ## container(container* c, cy_uint capacity);                                                                  \
int initialize_ ## container ## _with_allocator(container* c, cy_uint capacity, memory_allocator mem_allocator);               \
int initialize_ ## container ## _with_memory(container* c, cy_uint capacity, contained_type* data_p);                          \
                                                                                                                               \
/* basic container oprations to get capacity and element_count */                                                              \
cy_uint get_capacity_ ## container(const container* c);                                                                        \
cy_uint get_element_count_ ## container(const container* c);                                                                   \
                                                                                                                               \
/* check is_empty and is_full */                                                                                               \
int is_empty_ ## container(const container* c);                                                                                \
int is_full_ ## container(const container* c);                                                                                 \
                                                                                                                               \
/* arraylist like functionality for basic stack, queue and array like operations */                                            \
int push_front_to_ ## container(container* c, const contained_type* v);                                                        \
int push_back_to_ ## container(container* c, const contained_type* v);                                                         \
int pop_front_from_ ## container(container* c);                                                                                \
int pop_back_from_ ## container(container* c);                                                                                 \
const contained_type* get_front_of_ ## container(const container* c);                                                          \
const contained_type* get_back_of_ ## container(const container* c);                                                           \
const contained_type* get_from_front_of_ ## container(const container* c, cy_uint index);                                      \
const contained_type* get_from_back_of_ ## container(const container* c, cy_uint index);                                       \
int set_from_front_in_ ## container(container* c, const contained_type* v, cy_uint index);                                     \
int set_from_back_in_ ## container(container* c, const contained_type* v, cy_uint index);                                      \
int swap_from_front_in_ ## container(container* c, cy_uint i1, cy_uint i2);                                                    \
int swap_from_back_in_ ## container(container* c, cy_uint i1, cy_uint i2);                                                     \
                                                                                                                               \
/* below functions will make room for element_count number of elements, at the given index, either from front or back */       \
/* the vacant indices, from front or back will contain garbage data, and hence must be initialized before use */               \
int make_room_from_front_in_ ## container(container* c, cy_uint index, cy_uint element_count);                                 \
int make_room_from_back_in_ ## container(container* c, cy_uint index, cy_uint element_count);                                  \
                                                                                                                               \
/* bulk remove functions */                                                                                                    \
int remove_elements_from_front_of_ ## container(container* al, cy_uint index, cy_uint element_count_to_remove);                \
int remove_elements_from_back_of_ ## container(container* al, cy_uint index, cy_uint element_count_to_remove);                 \
                                                                                                                               \
/* get index accessed interface for elements */                                                                                \
index_accessed_interface get_index_accessed_interface_for_front_of_ ## container(container* c);                                \
index_accessed_interface get_index_accessed_interface_for_back_of_ ## container(container* c);                                 \
                                                                                                                               \
/* heap like access functions (top of heap is same as get_front_of_ function) */                                               \
void heapify_ ## container(container* c, heap_info* hinfo, cy_uint degree);                                                    \
void heapify_at_ ## container(container* c, heap_info* hinfo, cy_uint degree, cy_uint index);                                  \
int push_to_heap_ ## container(container* c, heap_info* hinfo, cy_uint degree, const contained_type* v);                       \
int pop_from_heap_ ## container(container* c, heap_info* hinfo, cy_uint degree);                                               \
int remove_from_heap_ ## container(container* c, heap_info* hinfo, cy_uint degree, cy_uint index);                             \
                                                                                                                               \
/* container specific sorting functions */                                                                                     \
/* (use these when index_accessed_search_sort sorting functions are restricted to only be used with arraylist) */              \
int merge_sort_ ## container(container* c, cy_uint start_index, cy_uint last_index, int (*compare)(const void* data1, const void* data2), memory_allocator mem_allocator);\
int heap_sort_ ## container(container* c, cy_uint start_index, cy_uint last_index, int (*compare)(const void* data1, const void* data2));\
int radix_sort_ ## container(container* c, cy_uint start_index, cy_uint last_index, unsigned long long int (*get_sort_attribute)(const contained_type* data), memory_allocator mem_allocator);\
                                                                                                                               \
/* functions to increase decrease capacity of the container */                                                                 \
int expand_ ## container(container* c);                                                                                        \
int shrink_ ## container(container* c);                                                                                        \
int reserve_capacity_for_ ## container(container* c, cy_uint atleast_capacity);                                                \
                                                                                                                               \
/* deinitialization function */                                                                                                \
void remove_all_from_ ## container(container* c);                                                                              \
void deinitialize_ ## container(container* c);                                                                                 \
                                                                                                                               \
/* sprint function */                                                                                                          \
void sprint_ ## container(dstring* append_str, const container* c, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs);\

// comment break, there must be a newline above this comment

#define definitions_value_arraylist(container, contained_type)                                                                 \
                                                                                                                               \
/* global constant setting the max capacity of this container built */                                                         \
static const cy_uint MAX_ ## container ## _CAPACITY = (CY_UINT_MAX / sizeof(contained_type));                                  \
                                                                                                                               \
/* initialization functions */                                                                                                 \
int initialize_ ## container(container* c, cy_uint capacity)                                                                   \
{                                                                                                                              \
	return initialize_ ## container ## _with_allocator(c, capacity, STD_C_mem_allocator);                                      \
}                                                                                                                              \
int initialize_ ## container ## _with_allocator(container* c, cy_uint capacity, memory_allocator mem_allocator)                \
{                                                                                                                              \
	/* check to ensure max capacity */                                                                                         \
	if(capacity > MAX_ ## container ## _CAPACITY)                                                                              \
		return 0;                                                                                                              \
                                                                                                                               \
	c->mem_allocator = mem_allocator;                                                                                          \
	c->first_index = 0;                                                                                                        \
	c->element_count = 0;                                                                                                      \
	cy_uint bytes_allocated = capacity * sizeof(contained_type);                                                               \
	c->data_p = (capacity > 0) ? aligned_allocate(c->mem_allocator, &bytes_allocated, _Alignof(contained_type)) : NULL;        \
	c->capacity_in_bytes = (c->data_p == NULL) ? 0 : bytes_allocated;                                                          \
                                                                                                                               \
	/* check for allocator error, if the allocation fails, return 0, else return 1 */                                          \
	if(bytes_allocated > 0 && c->data_p == NULL)                                                                               \
		return 0;                                                                                                              \
                                                                                                                               \
	return 1;                                                                                                                  \
}                                                                                                                              \
int initialize_ ## container ## _with_memory(container* c, cy_uint capacity, contained_type* data_p)                           \
{                                                                                                                              \
	/* check to ensure max capacity */                                                                                         \
	if(capacity > MAX_ ## container ## _CAPACITY)                                                                              \
		return 0;                                                                                                              \
                                                                                                                               \
	c->mem_allocator = NULL;                                                                                                   \
	c->first_index = 0;                                                                                                        \
	c->element_count = 0;                                                                                                      \
	c->data_p = data_p;                                                                                                        \
	c->capacity_in_bytes = capacity * sizeof(contained_type);                                                                  \
                                                                                                                               \
	return 1;                                                                                                                  \
}                                                                                                                              \
                                                                                                                               \
/* basic container oprations to get capacity and element_count */                                                              \
cy_uint get_capacity_ ## container (const container* c)                                                                        \
{                                                                                                                              \
	return (c->capacity_in_bytes / sizeof(contained_type));                                                                    \
}                                                                                                                              \
cy_uint get_element_count_ ## container (const container* c)                                                                   \
{                                                                                                                              \
	return c->element_count;                                                                                                   \
}                                                                                                                              \
                                                                                                                               \
/* check is_empty and is_full */                                                                                               \
int is_empty_ ## container(const container* c)                                                                                 \
{                                                                                                                              \
	return get_element_count_ ## container(c) == 0;                                                                            \
}                                                                                                                              \
int is_full_ ## container(const container* c)                                                                                  \
{                                                                                                                              \
	return get_element_count_ ## container(c) == get_capacity_ ## container(c);                                                \
}                                                                                                                              \
                                                                                                                               \
/* arraylist like functionality for basic stack, queue and array like operations */                                            \
int push_front_to_ ## container(container* c, const contained_type* v)                                                         \
{                                                                                                                              \
	if(is_full_ ## container(c))                                                                                               \
		return 0;                                                                                                              \
                                                                                                                               \
	if(is_empty_ ## container(c))                                                                                              \
		c->first_index = 0;                                                                                                    \
	else                                                                                                                       \
		c->first_index = get_circular_prev(c->first_index, get_capacity_ ## container(c));                                     \
                                                                                                                               \
	c->data_p[c->first_index] = (*v);                                                                                          \
                                                                                                                               \
	c->element_count++;                                                                                                        \
                                                                                                                               \
	return 1;                                                                                                                  \
}                                                                                                                              \
int push_back_to_ ## container(container* c, const contained_type* v)                                                          \
{                                                                                                                              \
	if(is_full_ ## container(c))                                                                                               \
		return 0;                                                                                                              \
                                                                                                                               \
	if(is_empty_ ## container(c))                                                                                              \
		c->first_index = 0;                                                                                                    \
                                                                                                                               \
	cy_uint end_index = get_end_index(c->first_index, c->element_count, get_capacity_ ## container(c));                        \
                                                                                                                               \
	c->data_p[end_index] = (*v);                                                                                               \
                                                                                                                               \
	c->element_count++;                                                                                                        \
                                                                                                                               \
	return 1;                                                                                                                  \
}                                                                                                                              \
int pop_front_from_ ## container(container* c)                                                                                 \
{                                                                                                                              \
	if(is_empty_ ## container(c))                                                                                              \
		return 0;                                                                                                              \
                                                                                                                               \
	c->first_index = get_circular_next(c->first_index, get_capacity_ ## container(c));                                         \
	c->element_count--;                                                                                                        \
                                                                                                                               \
	if(is_empty_ ## container(c))                                                                                              \
		c->first_index = 0;                                                                                                    \
                                                                                                                               \
	return 1;                                                                                                                  \
}                                                                                                                              \
int pop_back_from_ ## container(container* c)                                                                                  \
{                                                                                                                              \
	if(is_empty_ ## container(c))                                                                                              \
		return 0;                                                                                                              \
                                                                                                                               \
	c->element_count--;                                                                                                        \
                                                                                                                               \
	if(is_empty_ ## container(c))                                                                                              \
		c->first_index = 0;                                                                                                    \
                                                                                                                               \
	return 1;                                                                                                                  \
}                                                                                                                              \
const contained_type* get_front_of_ ## container(const container* c)                                                           \
{                                                                                                                              \
	if(is_empty_ ## container(c))                                                                                              \
		return NULL;                                                                                                           \
                                                                                                                               \
	return c->data_p + c->first_index;                                                                                         \
}                                                                                                                              \
const contained_type* get_back_of_ ## container(const container* c)                                                            \
{                                                                                                                              \
	if(is_empty_ ## container(c))                                                                                              \
		return NULL;                                                                                                           \
                                                                                                                               \
	return c->data_p +  get_last_index(c->first_index, c->element_count, get_capacity_ ## container(c));                       \
}                                                                                                                              \
const contained_type* get_from_front_of_ ## container(const container* c, cy_uint index)                                       \
{                                                                                                                              \
	if(is_empty_ ## container(c) || index >= c->element_count)                                                                 \
		return NULL;                                                                                                           \
                                                                                                                               \
	cy_uint index_concerned = add_circularly(c->first_index, index, get_capacity_ ## container(c));                            \
                                                                                                                               \
	return c->data_p + index_concerned;                                                                                        \
}                                                                                                                              \
const contained_type* get_from_back_of_ ## container(const container* c, cy_uint index)                                        \
{                                                                                                                              \
	if(is_empty_ ## container(c) || index >= c->element_count)                                                                 \
		return NULL;                                                                                                           \
                                                                                                                               \
	cy_uint last_index = get_last_index(c->first_index, c->element_count, get_capacity_ ## container(c));                      \
	cy_uint index_concerned = sub_circularly(last_index, index, get_capacity_ ## container(c));                                \
                                                                                                                               \
	return c->data_p + index_concerned;                                                                                        \
}                                                                                                                              \
int set_from_front_in_ ## container(container* c, const contained_type* v, cy_uint index)                                      \
{                                                                                                                              \
	if(is_empty_ ## container(c) || index >= c->element_count)                                                                 \
		return 0;                                                                                                              \
                                                                                                                               \
	cy_uint index_concerned = add_circularly(c->first_index, index, get_capacity_ ## container(c));                            \
                                                                                                                               \
	c->data_p[index_concerned] = (*v);                                                                                         \
                                                                                                                               \
	return 1;                                                                                                                  \
}                                                                                                                              \
int set_from_back_in_ ## container(container* c, const contained_type* v, cy_uint index)                                       \
{                                                                                                                              \
	if(is_empty_ ## container(c) || index >= c->element_count)                                                                 \
		return 0;                                                                                                              \
                                                                                                                               \
	cy_uint last_index = get_last_index(c->first_index, c->element_count, get_capacity_ ## container(c));                      \
	cy_uint index_concerned = sub_circularly(last_index, index, get_capacity_ ## container(c));                                \
                                                                                                                               \
	c->data_p[index_concerned] = (*v);                                                                                         \
                                                                                                                               \
	return 1;                                                                                                                  \
}                                                                                                                              \
int swap_from_front_in_ ## container(container* c, cy_uint i1, cy_uint i2)                                                     \
{                                                                                                                              \
	if(i1 >= get_element_count_ ## container(c) || i2 >= get_element_count_ ## container(c))                                   \
		return 0;                                                                                                              \
                                                                                                                               \
	cy_uint i1_concerned = add_circularly(c->first_index, i1, get_capacity_ ## container(c));                                  \
	cy_uint i2_concerned = add_circularly(c->first_index, i2, get_capacity_ ## container(c));                                  \
                                                                                                                               \
	return memory_swap(c->data_p + i1_concerned, c->data_p + i2_concerned, sizeof(contained_type));                            \
}                                                                                                                              \
int swap_from_back_in_ ## container(container* c, cy_uint i1, cy_uint i2)                                                      \
{                                                                                                                              \
	if(i1 >= get_element_count_ ## container(c) || i2 >= get_element_count_ ## container(c))                                   \
		return 0;                                                                                                              \
                                                                                                                               \
	cy_uint last_index = get_last_index(c->first_index, c->element_count, get_capacity_ ## container(c));                      \
                                                                                                                               \
	cy_uint i1_concerned = sub_circularly(last_index, i1, get_capacity_ ## container(c));                                      \
	cy_uint i2_concerned = sub_circularly(last_index, i2, get_capacity_ ## container(c));                                      \
                                                                                                                               \
	return memory_swap(c->data_p + i1_concerned, c->data_p + i2_concerned, sizeof(contained_type));                            \
}                                                                                                                              \
                                                                                                                               \
/* below functions will make room for element_count number of elements, at the given index, either from front or back */       \
/* the vacant indices, from front or back will contain garbage data, and hence must be initialized before use */               \
int make_room_from_front_in_ ## container(container* c, cy_uint index, cy_uint element_count);                                 \
int make_room_from_back_in_ ## container(container* c, cy_uint index, cy_uint element_count);                                  \
                                                                                                                               \
/* bulk remove functions */                                                                                                    \
int remove_elements_from_front_of_ ## container(container* c, cy_uint index, cy_uint element_count_to_remove);                 \
int remove_elements_from_back_of_ ## container(container* c, cy_uint index, cy_uint element_count_to_remove);                  \
                                                                                                                               \
/* get index accessed interface for elements */                                                                                \
index_accessed_interface get_index_accessed_interface_for_front_of_ ## container(container* c)                                 \
{                                                                                                                              \
	return (index_accessed_interface) {                                                                                        \
		.ds_p = c,                                                                                                             \
		.get_element = ((const void* (*)(const void*, cy_uint))(get_from_front_of_ ## container)),                             \
		.set_element = ((int (*)(void*, const void*, cy_uint))(set_from_front_in_ ## container)),                              \
		.swap_elements = ((int (*)(void*, cy_uint, cy_uint))(swap_from_front_in_ ## container)),                               \
		.get_element_count = ((cy_uint (*)(const void*))(get_element_count_ ## container)),                                    \
	};                                                                                                                         \
}                                                                                                                              \
index_accessed_interface get_index_accessed_interface_for_back_of_ ## container(container* c)                                  \
{                                                                                                                              \
	return (index_accessed_interface) {                                                                                        \
		.ds_p = c,                                                                                                             \
		.get_element = ((const void* (*)(const void*, cy_uint))(get_from_back_of_ ## container)),                              \
		.set_element = ((int (*)(void*, const void*, cy_uint))(set_from_back_in_ ## container)),                               \
		.swap_elements = ((int (*)(void*, cy_uint, cy_uint))(swap_from_back_in_ ## container)),                                \
		.get_element_count = ((cy_uint (*)(const void*))(get_element_count_ ## container)),                                    \
	};                                                                                                                         \
}                                                                                                                              \
                                                                                                                               \
/* heap like access functions (top of heap is same as get_front_of_ function) */                                               \
static void bubble_up_ ## container(container* c, heap_info* hinfo, cy_uint degree, cy_uint index)                             \
{                                                                                                                              \
	/* exit at index 0, or the index is out of range */                                                                        \
	while(has_parent_N(index) && index < get_element_count_ ## container(c))                                                   \
	{                                                                                                                          \
		cy_uint parent_index = get_parent_index_N(index, degree);                                                              \
                                                                                                                               \
		/* exit, if reordering is not required */                                                                              \
		if(!is_reordering_required(get_from_front_of_ ## container(c, parent_index), get_from_front_of_ ## container(c, index), hinfo))\
			break;                                                                                                             \
                                                                                                                               \
		swap_from_front_in_ ## container(c, parent_index, index);                                                              \
                                                                                                                               \
		index = parent_index;                                                                                                  \
	}                                                                                                                          \
}                                                                                                                              \
static void bubble_down_ ## container(container* c, heap_info* hinfo, cy_uint degree, cy_uint index)                           \
{                                                                                                                              \
	/* we can not bubble down the last node */                                                                                 \
	while(can_have_any_children_N(index, degree) && index < get_element_count_ ## container(c))                                \
	{                                                                                                                          \
		/* if no reordering is required then the element at the index position remains as the parent */                        \
		cy_uint new_parent_index = index;                                                                                      \
                                                                                                                               \
		/* iterate over all the children of the parent at index */                                                             \
		/* find the one that if made the new parent, will not require reordering with any of its siblings */                   \
		for(cy_uint i = 0, child_index = get_index_of_ith_child_N(index, 0, degree); i < degree && child_index < get_element_count_ ## container(c); i++, child_index++)\
		{                                                                                                                      \
			if(is_reordering_required(get_from_front_of_ ## container(c, new_parent_index), get_from_front_of_ ## container(c, child_index), hinfo))\
				new_parent_index = child_index;                                                                                \
		}                                                                                                                      \
                                                                                                                               \
		/* if this condition becomes true, then the index stays the parent of all its siblings, hence no reordering required */\
		/* and if no reordering was required, exit the loop */                                                                 \
		if(new_parent_index == index)                                                                                          \
			break;                                                                                                             \
                                                                                                                               \
		swap_from_front_in_ ## container(c, new_parent_index, index);                                                          \
                                                                                                                               \
		index = new_parent_index;                                                                                              \
	}                                                                                                                          \
}                                                                                                                              \
void heapify_ ## container(container* c, heap_info* hinfo, cy_uint degree)                                                     \
{                                                                                                                              \
	/* heapify_all is not required, if the element_count is 0 OR 1 */                                                          \
	if(get_element_count_ ## container(c) <= 1)                                                                                \
		return;                                                                                                                \
                                                                                                                               \
	/* bubble_down at all the possible-parent elements in reverse order */                                                     \
	/* this is because a bubble down at any leaf node is just a NOP, hence we are saving almost half of the loop iterations */ \
	for(cy_uint index = get_parent_index_N(get_element_count_ ## container(c) - 1, degree); index != -1; index--)              \
		bubble_down_ ## container(c, hinfo, degree, index);                                                                    \
}                                                                                                                              \
void heapify_at_ ## container(container* c, heap_info* hinfo, cy_uint degree, cy_uint index)                                   \
{                                                                                                                              \
	/* return if index is out-of-bounds */                                                                                     \
	if(index >= get_element_count_ ## container(c))                                                                            \
		return;                                                                                                                \
                                                                                                                               \
	/* if re-ordering is required at the parent side, we bubble up */                                                          \
	if(has_parent_N(index) && is_reordering_required(get_from_front_of_ ## container(c, get_parent_index_N(index, degree)), get_from_front_of_ ## container(c, index), hinfo))\
		bubble_up_ ## container(c, hinfo, degree, index);                                                                      \
                                                                                                                               \
	/* else we attempt a bubble down, only if it can have children */                                                          \
	else if(can_have_any_children_N(index, degree))                                                                            \
		bubble_down_ ## container(c, hinfo, degree, index);                                                                    \
}                                                                                                                              \
int push_to_heap_ ## container(container* c, heap_info* hinfo, cy_uint degree, const contained_type* v)                        \
{                                                                                                                              \
	/* fail, if push back fails */                                                                                             \
	if(!push_back_to_ ## container(c, v))                                                                                      \
		return 0;                                                                                                              \
                                                                                                                               \
	/* bubble up the newly added element at last index (element_count-1), to its desired place */                              \
	bubble_up_ ## container(c, hinfo, degree, get_element_count_ ## container(c) - 1);                                         \
                                                                                                                               \
	return 1;                                                                                                                  \
}                                                                                                                              \
int pop_from_heap_ ## container(container* c, heap_info* hinfo, cy_uint degree)                                                \
{                                                                                                                              \
	/* can not pop, if there are no elements in the heap */                                                                    \
	if(is_empty_ ## container(c))                                                                                              \
		return 0;                                                                                                              \
                                                                                                                               \
	/* remove the 0th element from the heap */                                                                                 \
	return remove_from_heap_ ## container(c, hinfo, degree, 0);                                                                \
}                                                                                                                              \
int remove_from_heap_ ## container(container* c, heap_info* hinfo, cy_uint degree, cy_uint index)                              \
{                                                                                                                              \
	/* an element can be removed, only if heap is not empty, and the index provided is within bounds */                        \
	if(is_empty_ ## container(c) || index >= get_element_count_ ## container(c))                                               \
		return 0;                                                                                                              \
                                                                                                                               \
	/* swap element at index with the last element */                                                                          \
	swap_from_front_in_ ## container(c, index, get_element_count_ ## container(c) - 1);                                        \
                                                                                                                               \
	/* pop the last element */                                                                                                 \
	if(!pop_back_from_ ## container(c))                                                                                        \
	{                                                                                                                          \
		/* undo the swap */                                                                                                    \
		swap_from_front_in_ ## container(c, index, get_element_count_ ## container(c) - 1);                                    \
		return 0;                                                                                                              \
	}                                                                                                                          \
                                                                                                                               \
	/* if the index is still within bounds, heapify_at that index */                                                           \
	if(index < get_element_count_ ## container(c))                                                                             \
		heapify_at_ ## container(c, hinfo, degree, index);                                                                     \
                                                                                                                               \
	return 1;                                                                                                                  \
}                                                                                                                              \
                                                                                                                               \
/* container specific sorting functions */                                                                                     \
/* (use these when index_accessed_search_sort sorting functions are restricted to only be used with arraylist) */              \
int merge_sort_ ## container(container* c, cy_uint start_index, cy_uint last_index, int (*compare)(const void* data1, const void* data2), memory_allocator mem_allocator);\
int heap_sort_ ## container(container* c, cy_uint start_index, cy_uint last_index, int (*compare)(const void* data1, const void* data2))\
{                                                                                                                              \
	if(start_index > last_index || last_index >= get_element_count_ ## container(c))                                           \
		return 0;                                                                                                              \
                                                                                                                               \
	/* compute the number of elements to sort; 0 or 1 number of elements do not need sorting */                                \
	cy_uint total_elements = last_index - start_index + 1;                                                                     \
	if(total_elements <= 1)                                                                                                    \
		return 1;                                                                                                              \
                                                                                                                               \
	/* create a max heap that points to the container's contents that need to be sorted */                                     \
	container sort_heap;                                                                                                       \
	heap_info hinfo;                                                                                                           \
	cy_uint degree;                                                                                                            \
	initialize_ ## container ## _with_memory(&sort_heap, get_capacity_ ## container(c), c->data_p);                            \
	sort_heap.first_index = add_circularly(c->first_index, start_index, get_capacity_ ## container(c));                        \
	sort_heap.element_count = total_elements;                                                                                  \
	hinfo.type = MAX_HEAP;                                                                                                     \
	hinfo.compare = compare;                                                                                                   \
	degree = 2; /* default degree is 2, i.e. a binary heap */                                                                  \
                                                                                                                               \
	/* now max heapify all elements that we need to sort */                                                                    \
	heapify_ ## container(&sort_heap, &hinfo, degree);                                                                         \
                                                                                                                               \
	/* place the top of the heap element in the array at the end, then pop heap */                                             \
	for(cy_uint i = total_elements; i > 1; i--)                                                                                \
	{                                                                                                                          \
		const contained_type max_data = *get_front_of_ ## container(&sort_heap);                                               \
		pop_from_heap_ ## container(&sort_heap, &hinfo, degree);                                                               \
		set_from_front_in_ ## container(c, &max_data, start_index + i - 1);                                                    \
	}                                                                                                                          \
                                                                                                                               \
	/* deinitialize the sort_heap */                                                                                           \
	deinitialize_ ## container(&sort_heap);                                                                                    \
                                                                                                                               \
	return 1;                                                                                                                  \
}                                                                                                                              \
int radix_sort_ ## container(container* c, cy_uint start_index, cy_uint last_index, unsigned long long int (*get_sort_attribute)(const contained_type* data), memory_allocator mem_allocator)\
{                                                                                                                              \
	if(mem_allocator == NULL || start_index > last_index || last_index >= get_element_count_ ## container(c))                  \
		return 0;                                                                                                              \
                                                                                                                               \
	/* compute the number of elements to sort; 0 or 1 number of elements do not need sorting */                                \
	cy_uint total_elements = last_index - start_index + 1;                                                                     \
	if(total_elements <= 1)                                                                                                    \
		return 1;                                                                                                              \
                                                                                                                               \
	/* construct temporary queues for 0 and 1 bit containing elements */                                                       \
	container sort_queue[2];                                                                                                   \
	if(!initialize_ ## container ## _with_allocator(&(sort_queue[0]), total_elements, mem_allocator))                          \
		return 0;                                                                                                              \
	if(!initialize_ ## container ## _with_allocator(&(sort_queue[1]), total_elements, mem_allocator))                          \
	{                                                                                                                          \
		deinitialize_ ## container(&(sort_queue[0]));                                                                          \
		return 0;                                                                                                              \
	}                                                                                                                          \
                                                                                                                               \
	for(cy_uint i = 0; i < (sizeof(unsigned long long int) * CHAR_BIT); i++)                                                   \
	{                                                                                                                          \
		cy_uint index = start_index;                                                                                           \
		while(index <= last_index)                                                                                             \
		{                                                                                                                      \
			const contained_type* data = get_from_front_of_ ## container(c, index++);                                          \
			unsigned int queue_index = ((get_sort_attribute(data) >> i) & 1ULL);                                               \
			push_back_to_ ## container(&(sort_queue[queue_index]), data);                                                      \
		}                                                                                                                      \
                                                                                                                               \
		index = start_index;                                                                                                   \
		while(!is_empty_ ## container(&(sort_queue[0])))                                                                       \
		{                                                                                                                      \
			const contained_type* data = get_front_of_ ## container(&(sort_queue[0]));                                         \
			set_from_front_in_ ## container(c, data, index++);                                                                 \
			pop_front_from_ ## container(&(sort_queue[0]));                                                                    \
		}                                                                                                                      \
		while(!is_empty_ ## container(&(sort_queue[1])))                                                                       \
		{                                                                                                                      \
			const contained_type* data = get_front_of_ ## container(&(sort_queue[1]));                                         \
			set_from_front_in_ ## container(c, data, index++);                                                                 \
			pop_front_from_ ## container(&(sort_queue[1]));                                                                    \
		}                                                                                                                      \
	}                                                                                                                          \
                                                                                                                               \
	deinitialize_ ## container(&(sort_queue[0]));                                                                              \
	deinitialize_ ## container(&(sort_queue[1]));                                                                              \
                                                                                                                               \
	return 1;                                                                                                                  \
}                                                                                                                              \
                                                                                                                               \
/* functions to increase decrease capacity of the container */                                                                 \
int expand_ ## container(container* c)                                                                                         \
{                                                                                                                              \
	/* compute new capacity */                                                                                                 \
	cy_uint capacity = get_capacity_ ## container(c);                                                                          \
	cy_uint new_capacity = capacity * EXPANSION_FACTOR + 2;                                                                    \
	if(new_capacity <= capacity || new_capacity > MAX_ ## container ## _CAPACITY)                                              \
		new_capacity = MAX_ ## container ## _CAPACITY;                                                                         \
                                                                                                                               \
	/* must be greater than old capacity  */                                                                                   \
	if(new_capacity <= capacity)                                                                                               \
		return 0;                                                                                                              \
                                                                                                                               \
	return reserve_capacity_for_ ## container(c, new_capacity);                                                                \
}                                                                                                                              \
static int shrink_ ## container ## _holder(container* c, cy_uint new_capacity)                                                 \
{                                                                                                                              \
	/* can not shrink if the allocator is NULL */                                                                              \
	if(c->mem_allocator == NULL)                                                                                               \
		return 0;                                                                                                              \
                                                                                                                               \
	/* new_capacity must be lesser than the old_capacity */                                                                    \
	if(new_capacity >= get_capacity_ ## container(c))                                                                          \
		return 0;                                                                                                              \
                                                                                                                               \
	/* number of bytes to be allocated */                                                                                      \
	cy_uint bytes_allocated = new_capacity * sizeof(contained_type);                                                           \
                                                                                                                               \
	/* reallocate memory for the new_capacity */                                                                               \
	contained_type* new_data_p = aligned_reallocate(c->mem_allocator,                                                          \
													c->data_p,                                                                 \
													c->capacity_in_bytes,                                                      \
													&bytes_allocated,                                                          \
													_Alignof(contained_type));                                                 \
                                                                                                                               \
	/* bytes_allocated is our real capacity_in_bytes */                                                                        \
                                                                                                                               \
	/* since memory allocation failed, return 0 */                                                                             \
	if(new_data_p == NULL && new_capacity > 0)                                                                                 \
		return 0;                                                                                                              \
                                                                                                                               \
	/* new assignment to data_p and its capacity */                                                                            \
	c->data_p = new_data_p;                                                                                                    \
	c->capacity_in_bytes = bytes_allocated;                                                                                    \
                                                                                                                               \
	return 1;                                                                                                                  \
}                                                                                                                              \
int shrink_ ## container(container* c)                                                                                         \
{                                                                                                                              \
	int has_holder_shrunk = 0;                                                                                                 \
                                                                                                                               \
	/* can't shrink a 0 capacity container */                                                                                  \
	if(get_capacity_ ## container(c) == 0)                                                                                     \
		return has_holder_shrunk;                                                                                              \
                                                                                                                               \
	if(is_empty_ ## container(c) || c->first_index <= get_last_index(c->first_index, c->element_count, get_capacity_ ## container(c)))\
	{                                                                                                                          \
		/* move elements to front */                                                                                           \
		if(!is_empty_ ## container(c) && c->first_index > 0)                                                                   \
		{                                                                                                                      \
			memory_move(c->data_p, c->data_p + c->first_index, c->element_count * sizeof(contained_type));                     \
			c->first_index = 0;                                                                                                \
		}                                                                                                                      \
                                                                                                                               \
		has_holder_shrunk = shrink_ ## container ## _holder(c, c->element_count);                                              \
	}                                                                                                                          \
                                                                                                                               \
	return has_holder_shrunk;                                                                                                  \
}                                                                                                                              \
static int reserve_capacity_for_ ## container ## _holder(container* c, cy_uint atleast_capacity)                               \
{                                                                                                                              \
	/* can not expand if the allocator is NULL */                                                                              \
	if(c->mem_allocator == NULL)                                                                                               \
		return 0;                                                                                                              \
                                                                                                                               \
	/* new_capacity to expand to */                                                                                            \
	cy_uint new_capacity = atleast_capacity;                                                                                   \
                                                                                                                               \
	/* if the new capacity is greater than the MAX_ARRAY_CAPACITY */                                                           \
	/* OR if new_capacity is lesser than or equal to the old_capacity, then fail to expand the array */                        \
	if(new_capacity > MAX_ ## container ## _CAPACITY || new_capacity <= get_capacity_ ## container(c))                         \
		return 0;                                                                                                              \
                                                                                                                               \
	/* number of bytes to be allocated */                                                                                      \
	cy_uint bytes_allocated = new_capacity * sizeof(contained_type);                                                           \
                                                                                                                               \
	/* reallocate memory for the new_capacity */                                                                               \
	contained_type* new_data_p = aligned_reallocate(c->mem_allocator,                                                          \
													c->data_p,                                                                 \
													c->capacity_in_bytes,                                                      \
													&bytes_allocated,                                                          \
													_Alignof(contained_type));                                                 \
                                                                                                                               \
	/* bytes_allocated is our real capacity_in_bytes */                                                                        \
                                                                                                                               \
	/* since memory allocation failed, return 0 */                                                                             \
	if(new_data_p == NULL && new_capacity > 0)                                                                                 \
		return 0;                                                                                                              \
                                                                                                                               \
	c->data_p = new_data_p;                                                                                                    \
	c->capacity_in_bytes = bytes_allocated;                                                                                    \
                                                                                                                               \
	return 1;                                                                                                                  \
}                                                                                                                              \
static void linearlize_ ## container ## _upon_expansion(container* c, cy_uint old_capacity)                                    \
{                                                                                                                              \
	/* element_count remains the same, it is indifferent to re-linearizing the data in this function */                        \
	cy_uint element_count = get_element_count_ ## container(c);                                                                \
                                                                                                                               \
	/* quit if there are no elements to be linearized */                                                                       \
	if(element_count == 0)                                                                                                     \
		return ;                                                                                                               \
                                                                                                                               \
	/* this is the first index before we started our data management and linearization task */                                 \
	cy_uint old_first_index = c->first_index;                                                                                  \
                                                                                                                               \
	/* check if the container does need linearization */                                                                       \
	if(old_first_index <= get_last_index(old_first_index, element_count, old_capacity))                                        \
		return ;                                                                                                               \
                                                                                                                               \
	/* reaching this place implies that the container is wrapped around and has 2 parts: a head and a tail */                  \
	/* lets calculate the number of elements in the head and tail */                                                           \
	cy_uint elements_in_head = old_capacity - old_first_index;   /* elements from old_first_index until old_capacity - 1 */    \
	cy_uint elements_in_tail = element_count - elements_in_head; /* elements from 0 to last_index == element_count - elements_in_head */\
                                                                                                                               \
	if(elements_in_head <= elements_in_tail)                                                                                   \
	{                                                                                                                          \
		/* calculate the new first index */                                                                                    \
		cy_uint new_first_index = get_capacity_ ## container(c) - elements_in_head;                                            \
                                                                                                                               \
		/* move all elements in head to the end of the container holder */                                                     \
		memory_move(c->data_p + new_first_index, c->data_p + old_first_index, elements_in_head * sizeof(contained_type));      \
                                                                                                                               \
		/* update the new first_index */                                                                                       \
		c->first_index = new_first_index;                                                                                      \
	}                                                                                                                          \
	else                                                                                                                       \
	{                                                                                                                          \
		/* number of new slots added */                                                                                        \
		cy_uint new_slots_added = get_capacity_ ## container(c) - old_capacity;                                                \
                                                                                                                               \
		/* we move as many elements (from tail), as we can, to the new slots available at the end */                           \
		cy_uint tail_elements_to_relocate = min(new_slots_added, elements_in_tail);                                            \
		memory_move(c->data_p + old_capacity, c->data_p, tail_elements_to_relocate * sizeof(contained_type));                  \
                                                                                                                               \
		/* then we move the remaining tail elements to the index 0 */                                                          \
		cy_uint tail_elements_to_shift = elements_in_tail - tail_elements_to_relocate;                                         \
		if(tail_elements_to_shift > 0)                                                                                         \
			memory_move(c->data_p, c->data_p + tail_elements_to_relocate, tail_elements_to_shift * sizeof(contained_type));    \
	}                                                                                                                          \
}                                                                                                                              \
int reserve_capacity_for_ ## container(container* c, cy_uint atleast_capacity)                                                 \
{                                                                                                                              \
	int data_movement_will_be_required = 1;                                                                                    \
                                                                                                                               \
	/* on this condition, we can expand without any data movement */                                                           \
	if(is_empty_ ## container(c) || c->first_index <= get_last_index(c->first_index, c->element_count, get_capacity_ ## container(c)))\
		data_movement_will_be_required = 0;                                                                                    \
                                                                                                                               \
	/* record old_capacity for future use */                                                                                   \
	cy_uint old_capacity = get_capacity_ ## container(c);                                                                      \
                                                                                                                               \
	/* expand the holder fearlessly */                                                                                         \
	int has_holder_expanded = reserve_capacity_for_ ## container ## _holder(c, atleast_capacity);                              \
                                                                                                                               \
	/* move data if necessary conditions meet */                                                                               \
	if(data_movement_will_be_required && has_holder_expanded)                                                                  \
		linearlize_ ## container ## _upon_expansion(c, old_capacity);                                                          \
                                                                                                                               \
	return has_holder_expanded;                                                                                                \
}                                                                                                                              \
                                                                                                                               \
/* deinitialization function */                                                                                                \
void remove_all_from_ ## container(container* c)                                                                               \
{                                                                                                                              \
	c->first_index = 0;                                                                                                        \
	c->element_count = 0;                                                                                                      \
}                                                                                                                              \
void deinitialize_ ## container(container* c)                                                                                  \
{                                                                                                                              \
	if(c->mem_allocator != NULL && c->data_p != NULL && c->capacity_in_bytes > 0)                                              \
		deallocate(c->mem_allocator, c->data_p, c->capacity_in_bytes);                                                         \
	c->mem_allocator = NULL;                                                                                                   \
	c->data_p = NULL;                                                                                                          \
	c->capacity_in_bytes = 0;                                                                                                  \
	c->first_index = 0;                                                                                                        \
	c->element_count = 0;                                                                                                      \
}                                                                                                                              \
                                                                                                                               \
/* sprint function */                                                                                                          \
void sprint_ ## container(dstring* append_str, const container* c, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs)\
{                                                                                                                              \
	sprint_chars(append_str, '\t', tabs++); snprintf_dstring(append_str, #container " :\n");                                   \
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "first_index : %" PRIu_cy_uint "\n", c->first_index);   \
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "element_count : %" PRIu_cy_uint "\n", c->element_count);\
                                                                                                                               \
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "holder : \n");                                         \
	for(cy_uint i = 0; i < get_capacity_ ## container(c); i++)                                                                 \
	{                                                                                                                          \
		if(sub_circularly(i, c->first_index, get_capacity_ ## container(c)) < get_element_count_ ## container(c))              \
			sprint_element(append_str, c->data_p + i, tabs + 1);                                                               \
		else                                                                                                                   \
		{                                                                                                                      \
			sprint_chars(append_str, '\t', tabs + 1);                                                                          \
			snprintf_dstring(append_str, "EMPTY\n");                                                                           \
		}                                                                                                                      \
	}                                                                                                                          \
	snprintf_dstring(append_str, "\n");                                                                                        \
}                                                                                                                              \

// comment break, there must be a newline above this comment

#endif