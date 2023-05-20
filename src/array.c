#include<array.h>

#include<cutlery_stds.h>
#include<cutlery_math.h>

// this is the factor and the constant amount, by which the capacity of data_p_p will be expanded or shrunk
#define EXPANSION_FACTR 1.5
#define EXPANSION_CONST 2

// new_capacity of data_p_p = (old_capacity of data_p_p * EXPANSION_FACTR) + EXPANSION_CONST
// the below function will calculate the next capacity (on expansion) for the given array, if it's current capacity is current_capacity
static cy_uint get_new_capacity(cy_uint current_capacity)
{
	cy_uint new_capacity = (current_capacity * EXPANSION_FACTR) + EXPANSION_CONST;

	// on over flow return max capacity
	return (new_capacity < current_capacity) ? MAX_ARRAY_CAPACITY : new_capacity;
}

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
	mem_size bytes_allocated = ((mem_size)capacity) * sizeof(void*);
	array_p->data_p_p = (capacity > 0) ? zallocate(array_p->mem_allocator, &bytes_allocated) : NULL;
	array_p->capacity = (array_p->data_p_p != NULL) ? (bytes_allocated / sizeof(void*)) : 0;

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
	array_p->capacity = capacity;

	return 1;
}

const void* get_from_array(const array* array_p, cy_uint index)
{
	return (index < array_p->capacity) ? array_p->data_p_p[index] : NULL;
}

int set_in_array(array* array_p, const void* data_p, cy_uint index)
{
	if(index < array_p->capacity)
	{
		array_p->data_p_p[index] = data_p;
		return 1;
	}
	return 0;
}

void swap_in_array(array* array_p, cy_uint i1, cy_uint i2)
{
	const void* data_temp_i1 = get_from_array(array_p, i1);
	set_in_array(array_p, get_from_array(array_p, i2), i1);
	set_in_array(array_p, data_temp_i1, i2);
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
		for(cy_uint i = 0; i < array_p->capacity; i++)
			set_in_array(array_p, NULL, i);
	*/

	return 1;
}

int copy_elements_from_array(array* array_p, unsigned int start_index, const array* array_from_p, unsigned int start_from_index, unsigned int element_count_to_copy)
{
	if(element_count_to_copy == 0)
		return 1;

	if( start_index >= get_capacity_array(array_p) || start_index + element_count_to_copy < start_index || start_index + element_count_to_copy > get_capacity_array(array_p) ||
		start_from_index >= get_capacity_array(array_from_p) || start_from_index + element_count_to_copy < start_from_index || start_from_index + element_count_to_copy > get_capacity_array(array_from_p))
		return 0;

	memory_move(array_p->data_p_p + start_index, array_from_p->data_p_p + start_from_index, element_count_to_copy * sizeof(void*));
	/*
		the above memory_move is equivalent to the below loop
		for(unsigned int i = 0; i < element_count_to_copy; i++)
			set_in_array(array_p, get_from_array(array_from_p, start_from_index + i), start_index + i);
	*/

	return 1;
}

void deinitialize_array(array* array_p)
{
	if(array_p->mem_allocator != NULL && array_p->capacity > 0 && array_p->data_p_p != NULL)
		deallocate(array_p->mem_allocator, array_p->data_p_p, array_p->capacity * sizeof(void*));
	array_p->mem_allocator = NULL;
	array_p->data_p_p = NULL;
	array_p->capacity = 0;
}

unsigned int get_capacity_array(const array* array_p)
{
	return array_p->capacity;
}

void for_each_non_null_in_array(const array* array_p, void (*operation)(void* data_p, cy_uint index, const void* additional_params), const void* additional_params)
{
	for(cy_uint i = 0; i < array_p->capacity; i++)
	{
		if(get_from_array(array_p, i) != NULL)
			operation(((void*)get_from_array(array_p, i)), i, additional_params);
	}
}

void for_each_in_array(const array* array_p, void (*operation)(void* data_p, cy_uint index, const void* additional_params), const void* additional_params)
{
	for(cy_uint i = 0; i < array_p->capacity; i++)
		operation(((void*)get_from_array(array_p, i)), i, additional_params);
}

int expand_array(array* array_p)
{
	cy_uint new_capacity = get_new_capacity(array_p->capacity);

	// new_capacity upon expansion must be greater than the current capacity
	if(new_capacity <= array_p->capacity)
		return 0;

	// expand array to atleast the new_capacity
	return reserve_capacity_for_array(array_p, new_capacity);
}

int reserve_capacity_for_array(array* array_p, cy_uint atleast_capacity)
{
	// can not expand if the allocator is NULL
	if(array_p->mem_allocator == NULL)
		return 0;

	// compute new_capacity to expand to
	cy_uint new_capacity = atleast_capacity;

	// new_capacity must be greater than the old_capacity
	if(new_capacity <= array_p->capacity)
		return 0;

	// if the new capacity is greater than the MAX_ARRAY_CAPACITY, then fail to expand the array
	if(new_capacity > MAX_ARRAY_CAPACITY)
		return 0;

	// number of bytes to be allocated
	cy_uint bytes_allocated = new_capacity * sizeof(void*);

	// reallocate memory for the new_capacity
	const void** new_data_p_p = reallocate(array_p->mem_allocator,
										array_p->data_p_p,
										array_p->capacity * sizeof(void*),
										&bytes_allocated);

	// since memory allocation failed, return 0
	if(new_data_p_p == NULL && new_capacity > 0)
		return 0;

	// the new capacity may be more than what you asked for
	// so calculate the bytes that were allocated
	new_capacity = bytes_allocated / sizeof(void*);

	// set all new pointers to NULL i.e. from old_capacity to new_capacity
	memory_set(new_data_p_p + array_p->capacity, 0,
			(new_capacity - array_p->capacity) * sizeof(void*));

	// new assignment to data_p_p and its capacity
	array_p->data_p_p = new_data_p_p;
	array_p->capacity = new_capacity;

	return 1;
}

int shrink_array(array* array_p, cy_uint new_capacity)
{
	// can not shrink if the allocator is NULL
	if(array_p->mem_allocator == NULL)
		return 0;

	// new_capacity must be lesser than the old_capacity
	if(new_capacity >= array_p->capacity)
		return 0;

	// number of bytes to be allocated
	mem_size bytes_allocated = new_capacity * sizeof(void*);

	// reallocate memory for the new_capacity
	const void** new_data_p_p = reallocate(array_p->mem_allocator,
										array_p->data_p_p,
										array_p->capacity * sizeof(void*),
										&bytes_allocated);

	// since memory allocation failed, return 0
	if(new_data_p_p == NULL && new_capacity > 0)
		return 0;

	// the new capacity may be more than what you asked for
	// so calculate the bytes that were allocated
	new_capacity = bytes_allocated / sizeof(void*);

	// new assignment to data_p_p and its capacity
	array_p->data_p_p = new_data_p_p;
	array_p->capacity = new_capacity;

	return 1;
}

void sprint_array(dstring* append_str, const array* array_p, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs++); snprintf_dstring(append_str, "array :\n");
	sprint_chars(append_str, '\t', tabs++); snprintf_dstring(append_str, "capacity : %u\n", array_p->capacity);

	for(cy_uint i = 0; i < array_p->capacity; i++)
	{
		sprint_chars(append_str, '\t', tabs);
		snprintf_dstring(append_str, "index_id = %u\n", i);

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
