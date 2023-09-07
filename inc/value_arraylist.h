#ifndef VALUE_ARRAYLIST_H
#define VALUE_ARRAYLIST_H

#include<cutlery_stds.h>
#include<memory_allocator_interface.h>
#include<index_accessed_interface.h>
#include<circular_buffer_array_util.h>
#include<heap_info.h>

/*
	value_arraylist is a header that will build a cache friendly container for you
	it is suppossed to be used with only small ephemeral structs and primitive types,
	that have no immovable attributes like (synchronization primitives like) mutex, conditional variables or semaphores
*/

#define declarations_value_arraylist(container, contained_type)                                                                \
struct container                                                                                                               \
{                                                                                                                              \
	container_type* data_p;         /* array, each element being od container_type */                                          \
	cy_uint first_index;            /* the index of the first element in the container */                                      \
	cy_uint element_count;          /* number of elements in the container */                                                  \
	cy_uint capacity_in_bytes;      /* capacity in bytes pointed by the holder */                                              \
	memory_allocator mem_allocator; /* allocator for the container */                                                          \
};                                                                                                                             \
                                                                                                                               \
#define MAX_ ## container ## _CAPACITY CU_UINT_MAX / sizeof(contained_type)                                                    \
                                                                                                                               \
/* initialization functions */                                                                                                 \
int initialize_ ## container(container* c, cy_uint capacity);                                                                  \
int initialize_ ## container ## _with_allocator(container* c, cy_uint capacity, memory_allocator mem_allocator);               \
int initialize_ ## container ## _with_memory(container* c, cy_uint capacity, contained_type* data_p);                          \
                                                                                                                               \
/* basic container oprations to get capacity and element_count */                                                              \
cy_uint get_capacity_ ## container (const container* c);                                                                       \
cy_uint get_element_count_ ## container (const container* c);                                                                  \
                                                                                                                               \
/* arraylist like functionality for basic stack, queue and array like operations */                                            \
int push_front_to_ ## container(container* c, const contained_type* v);                                                        \
int push_back_to_ ## container(container* c, const contained_type* v);                                                         \
int pop_front_from_ ## container(container* c);                                                                                \
int pop_back_from_ ## container(container* c);                                                                                 \
const contained_type* get_front_of_ ## container(const container* c);                                                          \
const contained_type* get_back_of_ ## container(const container* c);                                                           \
const contained_type* get_from_front_of_ ## container(const container* c);                                                     \
const contained_type* get_from_back_of_ ## container(const container* c);                                                      \
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
int heapify_ ## container(container* c, heap_info* hinfo);                                                                     \
int push_to_heap_ ## container(container* c, heap_info* hinfo, contained_type* v);                                             \
int pop_from_heap_ ## container(container* c, heap_info* hindo);                                                               \
int remove_from_heap_ ## container(container* c, heap_info* hinfo);                                                            \
                                                                                                                               \
/* container specific sorting functions */                                                                                     \
/* (use these when index_accessed_search_sort sorting functions are restricted to only be used with arraylist) */              \
int merge_sort_ ## container(container* c, cy_uint start_index, cy_uint last_index, int (*compare)(const void* data1, const void* data2));\
int heap_sort_ ## container(container* c, cy_uint start_index, cy_uint last_index, int (*compare)(const void* data1, const void* data2));\
int radix_sort_ ## container(container* c, cy_uint start_index, cy_uint last_index, unsigned long long int (*get_sort_attribute)(const void* data));\
                                                                                                                               \
/* functions to increase decrease capacity of the container */                                                                 \
int expand_ ## container(container* c);                                                                                        \
int shrink_ ## container(container* c);                                                                                        \
int reserve_capacity_for_ ## container(container* c, cy_uint atleast_capacity);                                                \
                                                                                                                               \
/* deinitialization function */                                                                                                \
void remove_all_from_ ## container(container* c);                                                                              \
int deinitialize_ ## container(container* c);                                                                                  \

// comment break, there must be a newline above this comment

#define definitions_value_arraylist(container, contained_type)                                                                 \
/* initialization functions */                                                                                                 \
int initialize_ ## container(container* c, cy_uint capacity)                                                                   \
{                                                                                                                              \
	return initialize_ ## container ## _with_allocator(c, capacity, STD_C_mem_allocator);                                      \
}                                                                                                                              \
int initialize_ ## container ## _with_allocator(container* c, cy_uint capacity, memory_allocator mem_allocator)                \
{                                                                                                                              \
	/* check to ensure max capacity */                                                                                         \
	if(capacity > MAX_ container _ CAPACITY)                                                                                   \
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
	if(bytes_allocated > 0 && array_p->data_p_p == NULL)                                                                       \
		return 0;                                                                                                              \
                                                                                                                               \
	return 1;                                                                                                                  \
}                                                                                                                              \
int initialize_ ## container ## _with_memory(container* c, cy_uint capacity, contained_type* data_p)                           \
{                                                                                                                              \
	/* check to ensure max capacity */                                                                                         \
	if(capacity > MAX_ container _ CAPACITY)                                                                                   \
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
/* arraylist like functionality for basic stack, queue and array like operations */                                            \
int push_front_to_ ## container(container* c, const contained_type* v);                                                        \
int push_back_to_ ## container(container* c, const contained_type* v);                                                         \
int pop_front_from_ ## container(container* c);                                                                                \
int pop_back_from_ ## container(container* c);                                                                                 \
const contained_type* get_front_of_ ## container(const container* c);                                                          \
const contained_type* get_back_of_ ## container(const container* c);                                                           \
const contained_type* get_from_front_of_ ## container(const container* c);                                                     \
const contained_type* get_from_back_of_ ## container(const container* c);                                                      \
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
int heapify_ ## container(container* c, heap_info* hinfo);                                                                     \
int push_to_heap_ ## container(container* c, heap_info* hinfo, contained_type* v);                                             \
int pop_from_heap_ ## container(container* c, heap_info* hindo);                                                               \
int remove_from_heap_ ## container(container* c, heap_info* hinfo);                                                            \
                                                                                                                               \
/* container specific sorting functions */                                                                                     \
/* (use these when index_accessed_search_sort sorting functions are restricted to only be used with arraylist) */              \
int merge_sort_ ## container(container* c, cy_uint start_index, cy_uint last_index, int (*compare)(const void* data1, const void* data2));\
int heap_sort_ ## container(container* c, cy_uint start_index, cy_uint last_index, int (*compare)(const void* data1, const void* data2));\
int radix_sort_ ## container(container* c, cy_uint start_index, cy_uint last_index, unsigned long long int (*get_sort_attribute)(const void* data));\
                                                                                                                               \
/* functions to increase decrease capacity of the container */                                                                 \
int expand_ ## container(container* c);                                                                                        \
int shrink_ ## container(container* c);                                                                                        \
int reserve_capacity_for_ ## container(container* c, cy_uint atleast_capacity);                                                \
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

// comment break, there must be a newline above this comment

#endif