#include<cutlery/array.h>

#include<cutlery/cutlery_stds.h>
#include<cutlery/cutlery_math.h>

// this is the factor and the constant amount, by which the capacity of data_p_p will be expanded or shrunk
#define EXPANSION_FACTR 1.5
#define EXPANSION_CONST 2.0

int initialize_array(array* array_p, cy_uint capacity)
{// initialize array with the default memory allocator
	return initialize_array_with_allocator(array_p, capacity, STD_C_mem_allocator);
}

int initialize_array_with_allocator(array* array_p, cy_uint capacity, memory_allocator mem_allocator)
{
	// if the capacity is greater than the MAX_ARRAY_CAPACITY, then fail array initialization
	if(capacity > MAX_ARRAY_CAPACITY)
		return 0;

	array_p->mem_allocator = mem_allocator;
	cy_uint bytes_allocated = capacity * sizeof(void*);
	array_p->data_p_p = (capacity > 0) ? aligned_zallocate(array_p->mem_allocator, &bytes_allocated, _Alignof(void*)) : NULL;
	array_p->capacity_in_bytes = (array_p->data_p_p != NULL) ? bytes_allocated : 0;

	// check for allocator error, if the allocation fails, return 0, else return 1
	if(bytes_allocated > 0 && array_p->data_p_p == NULL)
		return 0;

	return 1;
}

int initialize_array_with_memory(array* array_p, cy_uint capacity, const void* data_ps[])
{
	// if the capacity is greater than the MAX_ARRAY_CAPACITY, then fail array initialization
	if(capacity > MAX_ARRAY_CAPACITY)
		return 0;

	array_p->mem_allocator = NULL;
	array_p->data_p_p = data_ps;
	array_p->capacity_in_bytes = capacity * sizeof(void*);

	return 1;
}

const void* get_from_array(const array* array_p, cy_uint index)
{
	return (index < get_capacity_array(array_p)) ? array_p->data_p_p[index] : NULL;
}

int set_in_array(array* array_p, const void* data_p, cy_uint index)
{
	if(index < get_capacity_array(array_p))
	{
		array_p->data_p_p[index] = data_p;
		return 1;
	}
	return 0;
}

int swap_in_array(array* array_p, cy_uint i1, cy_uint i2)
{
	// make sure that i1 and i2 are not out of range
	if(i1 >= get_capacity_array(array_p) || i2 >= get_capacity_array(array_p))
		return 0;

	// if i1 == i2, swap is NOP
	if(i1 == i2)
		return 1;

	// perform swap for the i1 and i2 with a temp variable
	const void* temp = array_p->data_p_p[i1];
	array_p->data_p_p[i1] = array_p->data_p_p[i2];
	array_p->data_p_p[i2] = temp;

	return 1;
}

void remove_all_from_array(array* array_p)
{
	set_all_NULL_in_array(array_p);
}

void set_all_NULL_in_array(array* array_p)
{
	set_NULLs_in_array(array_p, 0, get_capacity_array(array_p));
}

int set_NULLs_in_array(array* array_p, cy_uint start_index, cy_uint element_count_to_NULL)
{
	// if no elements are to be set to NULL then return success
	if(element_count_to_NULL == 0)
		return 1;

	// make sure that all the indices from [start_index, start_index + element_count_to_NULL - 1) are accessible and in range of the array_p
	if(start_index >= get_capacity_array(array_p) || start_index + element_count_to_NULL < start_index  || start_index + element_count_to_NULL > get_capacity_array(array_p))
		return 0;

	memory_set(array_p->data_p_p + start_index, 0, element_count_to_NULL * sizeof(void*));
	/*
		the above memory_set is equivalent to the below loop
		for(cy_uint i = 0; i < get_capacity_array(array_p); i++)
			set_in_array(array_p, NULL, i);
	*/

	return 1;
}

int copy_elements_from_array(array* array_p, cy_uint start_index, const array* array_from_p, cy_uint start_from_index, cy_uint element_count_to_copy)
{
	if(element_count_to_copy == 0)
		return 1;

	if( start_index >= get_capacity_array(array_p) || start_index + element_count_to_copy < start_index || start_index + element_count_to_copy > get_capacity_array(array_p) ||
		start_from_index >= get_capacity_array(array_from_p) || start_from_index + element_count_to_copy < start_from_index || start_from_index + element_count_to_copy > get_capacity_array(array_from_p))
		return 0;

	memory_move(array_p->data_p_p + start_index, array_from_p->data_p_p + start_from_index, element_count_to_copy * sizeof(void*));
	/*
		the above memory_move is equivalent to the below loop
		for(cy_uint i = 0; i < element_count_to_copy; i++)
			set_in_array(array_p, get_from_array(array_from_p, start_from_index + i), start_index + i);
	*/

	return 1;
}

void deinitialize_array(array* array_p)
{
	if(array_p->mem_allocator != NULL && array_p->capacity_in_bytes > 0 && array_p->data_p_p != NULL)
		deallocate(array_p->mem_allocator, array_p->data_p_p, array_p->capacity_in_bytes);
	array_p->mem_allocator = NULL;
	array_p->data_p_p = NULL;
	array_p->capacity_in_bytes = 0;
}

cy_uint get_capacity_array(const array* array_p)
{
	return array_p->capacity_in_bytes / sizeof(void*);
}

void for_each_non_null_in_array(const array* array_p, void (*operation)(void* data_p, cy_uint index, const void* additional_params), const void* additional_params)
{
	for(cy_uint i = 0; i < get_capacity_array(array_p); i++)
	{
		if(get_from_array(array_p, i) != NULL)
			operation(((void*)get_from_array(array_p, i)), i, additional_params);
	}
}

void for_each_in_array(const array* array_p, void (*operation)(void* data_p, cy_uint index, const void* additional_params), const void* additional_params)
{
	for(cy_uint i = 0; i < get_capacity_array(array_p); i++)
		operation(((void*)get_from_array(array_p, i)), i, additional_params);
}

int expand_array(array* array_p)
{
	// calculate new capacity
	cy_uint capacity = get_capacity_array(array_p);

	cy_uint new_capacity;
	{
		double new_capacity_as_double = (capacity * ((double)EXPANSION_FACTR)) + ((double)EXPANSION_CONST);
		if(new_capacity_as_double > MAX_ARRAY_CAPACITY)
			new_capacity = MAX_ARRAY_CAPACITY;
		else
			new_capacity = new_capacity_as_double;
	}

	// new_capacity must be greater than capacity
	if(new_capacity <= capacity)
		return 0;

	// expand array to atleast the new_capacity
	return reserve_capacity_for_array(array_p, new_capacity);
}

int reserve_capacity_for_array(array* array_p, cy_uint atleast_capacity)
{
	// can not expand if the allocator is NULL
	if(array_p->mem_allocator == NULL)
		return 0;

	// new_capacity to expand to
	cy_uint new_capacity = atleast_capacity;

	// if the new capacity is greater than the MAX_ARRAY_CAPACITY
	// OR if new_capacity is lesser than or equal to the old_capacity, then fail to expand the array
	if(new_capacity > MAX_ARRAY_CAPACITY || new_capacity <= get_capacity_array(array_p))
		return 0;

	// number of bytes to be allocated
	cy_uint bytes_allocated = new_capacity * sizeof(void*);

	// reallocate memory for the new_capacity
	const void** new_data_p_p = aligned_reallocate(array_p->mem_allocator,
													array_p->data_p_p,
													array_p->capacity_in_bytes,
													&bytes_allocated,
													_Alignof(void*));

	// bytes_allocated is our real capacity_in_bytes

	// since memory allocation failed, return 0
	if(new_data_p_p == NULL && new_capacity > 0)
		return 0;

	// set all new pointers to NULL i.e. from old_capacity to new_capacity
	memory_set(new_data_p_p + get_capacity_array(array_p), 0, bytes_allocated - array_p->capacity_in_bytes);

	// new assignment to data_p_p and its capacity
	array_p->data_p_p = new_data_p_p;
	array_p->capacity_in_bytes = bytes_allocated;

	return 1;
}

int shrink_array(array* array_p, cy_uint new_capacity)
{
	// can not shrink if the allocator is NULL
	if(array_p->mem_allocator == NULL)
		return 0;

	// new_capacity must be lesser than the old_capacity
	if(new_capacity >= get_capacity_array(array_p))
		return 0;

	// number of bytes to be allocated
	cy_uint bytes_allocated = new_capacity * sizeof(void*);

	// reallocate memory for the new_capacity
	const void** new_data_p_p = aligned_reallocate(array_p->mem_allocator,
													array_p->data_p_p,
													array_p->capacity_in_bytes,
													&bytes_allocated,
													_Alignof(void*));

	// bytes_allocated is our real capacity_in_bytes

	// since memory allocation failed, return 0
	if(new_data_p_p == NULL && new_capacity > 0)
		return 0;

	// new assignment to data_p_p and its capacity
	array_p->data_p_p = new_data_p_p;
	array_p->capacity_in_bytes = bytes_allocated;

	return 1;
}

void sprint_array(dstring* append_str, const array* array_p, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs++); snprintf_dstring(append_str, "array :\n");
	sprint_chars(append_str, '\t', tabs++); snprintf_dstring(append_str, "capacity : %" PRIu_cy_uint "\n", get_capacity_array(array_p));

	for(cy_uint i = 0; i < get_capacity_array(array_p); i++)
	{
		sprint_chars(append_str, '\t', tabs);
		snprintf_dstring(append_str, "index_id = %" PRIu_cy_uint "\n", i);

		const void* element = get_from_array(array_p, i);
		if(element != NULL)
			sprint_element(append_str, element, tabs + 1);
		else
		{
			sprint_chars(append_str, '\t', tabs + 1); 
			snprintf_dstring(append_str, "NULL");
		}

		snprintf_dstring(append_str, "\n");
	}
}

index_accessed_interface get_index_accessed_interface_for_array(array* array_p)
{
	return (index_accessed_interface){
										.ds_p = array_p,
										.get_element = (const void* (*)(const void*, cy_uint))get_from_array,
										.set_element = (int (*)(void*, const void*, cy_uint))set_in_array,
										.swap_elements = (int (*)(void*, cy_uint, cy_uint))swap_in_array,
										.get_element_count = (cy_uint (*)(const void*))get_capacity_array,
									};
}
