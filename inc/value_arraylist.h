#ifndef VALUE_ARRAYLIST_H
#define VALUE_ARRAYLIST_H

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
/* initialization functions */                                                                                                 \
int initialize_ ## container(container* c, cy_uint capacity);                                                                  \
int initialize_ ## container ## _with_allocator(container* c, cy_uint capacity, memory_allocator mem_allocator);               \
int initialize_ ## container ## _with_memory(container* c, cy_uint capacity, contained_type* data_p);                          \
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


#endif