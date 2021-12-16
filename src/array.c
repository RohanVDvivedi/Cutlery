#include<array.h>

#include<cutlery_stds.h>

// this is the factor and the constant amount, by which the capacity of data_p_p will be expanded or shrunk
#define EXPANSION_FACTR 1.5
#define EXPANSION_CONST 2

// new_capacity of data_p_p = (old_capacity of data_p_p * EXPANSION_FACTR) + EXPANSION_CONST
// the below function will calculate the next capacity (on expansion) for the given array, if it's current capacity is current_capacity
static unsigned int get_new_capacity(unsigned int current_capacity)
{
	return (current_capacity * EXPANSION_FACTR) + EXPANSION_CONST;
}

void initialize_array(array* array_p, unsigned int capacity)
{// initialize array with the default memory allocator
	initialize_array_with_allocator(array_p, capacity, STD_C_mem_allocator);
}

void initialize_array_with_allocator(array* array_p, unsigned int capacity, memory_allocator mem_allocator)
{
	array_p->mem_allocator = mem_allocator;
	array_p->data_p_p = (capacity > 0) ? zallocate(array_p->mem_allocator, capacity * sizeof(void*)) : NULL;
	array_p->capacity = (array_p->data_p_p != NULL) ? capacity : 0;
}

void deinitialize_array(array* array_p)
{
	if(array_p->mem_allocator != NULL && array_p->capacity > 0 && array_p->data_p_p != NULL)
		deallocate(array_p->mem_allocator, array_p->data_p_p, array_p->capacity * sizeof(void*));
	array_p->mem_allocator = NULL;
	array_p->data_p_p = NULL;
	array_p->capacity = 0;
}

const void* get_element(const array* array_p, unsigned int index)
{
	return (index < array_p->capacity) ? array_p->data_p_p[index] : NULL;
}

int set_element(array* array_p, const void* data_p, unsigned int index)
{
	if(index < array_p->capacity)
	{
		array_p->data_p_p[index] = data_p;
		return 1;
	}
	return 0;
}

void swap_elements(array* array_p, unsigned int i1, unsigned int i2)
{
	const void* data_temp_i1 = get_element(array_p, i1);
	set_element(array_p, get_element(array_p, i2), i1);
	set_element(array_p, data_temp_i1, i2);
}

unsigned int get_capacity_array(const array* array_p)
{
	return array_p->capacity;
}

void for_each_non_null_in_array(const array* array_p, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params)
{
	for(unsigned int i = 0; i < array_p->capacity; i++)
	{
		if(get_element(array_p, i) != NULL)
			operation(((void*)get_element(array_p, i)), i, additional_params);
	}
}

void for_each_in_array(const array* array_p, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params)
{
	for(unsigned int i = 0; i < array_p->capacity; i++)
		operation(((void*)get_element(array_p, i)), i, additional_params);
}

int expand_array(array* array_p)
{
	// can not expand if the allocator is NULL
	if(array_p->mem_allocator == NULL)
		return 0;

	// compute new_capacity to expand to
	unsigned int new_capacity = get_new_capacity(array_p->capacity);

	// new_capacity must be greater than the old_capacity
	if(new_capacity <= array_p->capacity)
		return 0;

	// reallocate memory for the new_capacity
	const void** new_data_p_p = reallocate(array_p->mem_allocator,
										array_p->data_p_p,
										array_p->capacity * sizeof(void*),
										new_capacity * sizeof(void*));

	// since memory allocation failed, return 0
	if(new_data_p_p == NULL && new_capacity > 0)
		return 0;

	// set all new pointers to NULL i.e. from old_capacity to new_capacity
	memory_set(new_data_p_p + array_p->capacity, 0,
			(new_capacity - array_p->capacity) * sizeof(void*));

	// new assignment to data_p_p and its capacity
	array_p->data_p_p = new_data_p_p;
	array_p->capacity = new_capacity;

	return 1;
}

int shrink_array(array* array_p, unsigned int new_capacity)
{
	// can not shrink if the allocator is NULL
	if(array_p->mem_allocator == NULL)
		return 0;

	// new_capacity must be lesser than the old_capacity
	if(new_capacity >= array_p->capacity)
		return 0;

	// reallocate memory for the new_capacity
	const void** new_data_p_p = reallocate(array_p->mem_allocator,
										array_p->data_p_p,
										array_p->capacity * sizeof(void*),
										new_capacity * sizeof(void*));

	// since memory allocation failed, return 0
	if(new_data_p_p == NULL && new_capacity > 0)
		return 0;

	// new assignment to data_p_p and its capacity
	array_p->data_p_p = new_data_p_p;
	array_p->capacity = new_capacity;

	return 1;
}

void sprint_array(dstring* append_str, const array* array_p, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs++); snprintf_dstring(append_str, "array :\n");
	sprint_chars(append_str, '\t', tabs++); snprintf_dstring(append_str, "capacity : %u\n", array_p->capacity);

	for(unsigned int i = 0; i < array_p->capacity; i++)
	{
		sprint_chars(append_str, '\t', tabs);
		snprintf_dstring(append_str, "index_id = %u\n", i);

		const void* element = get_element(array_p, i);
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
