#include<array.h>

#include<cutlery_stds.h>

// this is the factor and the constant amount, by which the size of data_p_p will be expanded or shrunk
#define EXPANSION_FACTR 1.5
#define EXPANSION_CONST 2

// new_size of data_p_p = (old_size of data_p_p * EXPANSION_FACTR) + EXPANSION_CONST
// the below function will calculate the next size (on expansion) for the given array, if it's current size is current_size
static unsigned int get_new_size(unsigned int current_size)
{
	return (current_size * EXPANSION_FACTR) + EXPANSION_CONST;
}

void initialize_array(array* array_p, unsigned int initial_size)
{// initialize array with the default memory allocator
	initialize_array_with_allocator(array_p, initial_size, STD_C_mem_allocator);
}

void initialize_array_with_allocator(array* array_p, unsigned int initial_size, memory_allocator array_mem_allocator)
{
	array_p->array_mem_allocator = array_mem_allocator;
	array_p->data_p_p = (initial_size > 0) ? zallocate(array_p->array_mem_allocator, initial_size * sizeof(void*)) : NULL;
	array_p->size = (array_p->data_p_p != NULL) ? initial_size : 0;
}

void deinitialize_array(array* array_p)
{
	if(array_p->array_mem_allocator != NULL && array_p->size > 0 && array_p->data_p_p != NULL)
		deallocate(array_p->array_mem_allocator, array_p->data_p_p, array_p->size * sizeof(void*));
	array_p->array_mem_allocator = NULL;
	array_p->data_p_p = NULL;
	array_p->size = 0;
}

const void* get_element(const array* array_p, unsigned int index)
{
	return (index < array_p->size) ? array_p->data_p_p[index] : NULL;
}

int set_element(array* array_p, const void* data_p, unsigned int index)
{
	if(index < array_p->size)
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

unsigned int get_size_array(const array* array_p)
{
	return array_p->size;
}

void for_each_non_null_in_array(const array* array_p, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params)
{
	for(unsigned int i = 0; i < array_p->size; i++)
	{
		if(get_element(array_p, i) != NULL)
			operation(((void*)get_element(array_p, i)), i, additional_params);
	}
}

void for_each_in_array(const array* array_p, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params)
{
	for(unsigned int i = 0; i < array_p->size; i++)
		operation(((void*)get_element(array_p, i)), i, additional_params);
}

int expand_array(array* array_p)
{
	// can not expand if the allocator is NULL
	if(array_p->array_mem_allocator == NULL)
		return 0;

	// compute new_size to expand to
	unsigned int new_size = get_new_size(array_p->size);

	// new_size must be greater than the old_size
	if(new_size <= array_p->size)
		return 0;

	// reallocate memory for the newsize
	const void** new_data_p_p = reallocate(array_p->array_mem_allocator,
										array_p->data_p_p,
										array_p->size * sizeof(void*),
										new_size * sizeof(void*));

	// since memory allocation failed, return 0
	if(new_data_p_p == NULL && new_size > 0)
		return 0;

	// set all new pointers to NULL i.e. from old_size to new_size
	memory_set(new_data_p_p + array_p->size, 0,
			(new_size - array_p->size) * sizeof(void*));

	// new assignment to data_p_p and the size
	array_p->data_p_p = new_data_p_p;
	array_p->size = new_size;

	return 1;
}

int shrink_array(array* array_p, unsigned int new_size)
{
	// can not shrink if the allocator is NULL
	if(array_p->array_mem_allocator == NULL)
		return 0;

	// new_size must be lesser than the old_size
	if(new_size >= array_p->size)
		return 0;

	// reallocate memory for the new_size
	const void** new_data_p_p = reallocate(array_p->array_mem_allocator,
										array_p->data_p_p,
										array_p->size * sizeof(void*),
										new_size * sizeof(void*));

	// since memory allocation failed, return 0
	if(new_data_p_p == NULL && new_size > 0)
		return 0;

	// new assignment to data_p_p and the size
	array_p->data_p_p = new_data_p_p;
	array_p->size = new_size;

	return 1;
}

void sprint_array(dstring* append_str, const array* array_p, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs++); snprintf_dstring(append_str, "array :\n");
	sprint_chars(append_str, '\t', tabs++); snprintf_dstring(append_str, "size : %u\n", array_p->size);

	for(unsigned int i = 0; i < array_p->size; i++)
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
