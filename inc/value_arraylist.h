#ifndef VALUE_ARRAYLIST_H
#define VALUE_ARRAYLIST_H

#define declarations_value_arraylist(container_name, contained_type)                                                \
struct container_name                                                                                               \
{                                                                                                                   \
	container_type* holder;         /* array, each element being od container_type */                               \
	cy_uint first_index;            /* the index of the first element in the container */                           \
	cy_uint element_count;          /* number of elements in the container */                                       \
	cy_uint capacity_in_bytes;      /* capacity in bytes pointed by the holder */                                   \
	memory_allocator mem_allocator; /* allocator for the container */                                               \
};

#define definitions_value_arraylist(container_name, contained_type)

#endif