#include<array.h>

#include<cutlery_stds.h>

#include<stdio.h>

// this is the factor and the constant amount, by which the size of data_p_p will be expanded or shrunk
#define EXPANSION_FACTR 1.8
#define EXPANSION_CONST 2

// new_total_size of data_p_p = (old_total_size of data_p_p * EXPANSION_FACTR) + EXPANSION_CONST
// the below function will calculate the next size (on expansion) for the given array, if it's current size is current_size
static unsigned int get_new_total_size(unsigned int current_size)
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
	array_p->total_size = (array_p->data_p_p != NULL) ? initial_size : 0;
}

void deinitialize_array(array* array_p)
{
	if(array_p->total_size > 0 && array_p->data_p_p != NULL)
		deallocate(array_p->array_mem_allocator, array_p->data_p_p, array_p->total_size);
	array_p->data_p_p = NULL;
	array_p->total_size = 0;
}

const void* get_element(const array* array_p, unsigned int index)
{
	return (index < array_p->total_size) ? array_p->data_p_p[index] : NULL;
}

int set_element(array* array_p, const void* data_p, unsigned int index)
{
	if(index < array_p->total_size)
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

void for_each_non_null_in_array(const array* array_p, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params)
{
	for(unsigned int i = 0; i < array_p->total_size; i++)
	{
		if(get_element(array_p, i) != NULL)
			operation(((void*)get_element(array_p, i)), i, additional_params);
	}
}

void for_each_in_array(const array* array_p, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params)
{
	for(unsigned int i = 0; i < array_p->total_size; i++)
		operation(((void*)get_element(array_p, i)), i, additional_params);
}

int expand_array(array* array_p)
{
	// compute new_total_size to expand to
	unsigned int new_total_size = get_new_total_size(array_p->total_size);

	// new_total_size must be greater than the old_total_size
	if(new_total_size <= array_p->total_size)
		return 0;

	// reallocate memory for the new_total_size
	const void** new_data_p_p = reallocate(array_p->array_mem_allocator,
										array_p->data_p_p,
										array_p->total_size * sizeof(void*),
										new_total_size * sizeof(void*));

	// since memory allocation failed, return 0
	if(new_data_p_p == NULL && new_total_size > 0)
		return 0;

	// set all new pointers to NULL i.e. from old_total_size to new_total_size
	memory_set(new_data_p_p + array_p->total_size, 0,
			(new_total_size - array_p->total_size) * sizeof(void*));

	// new assignment to data_p_p and the total_size
	array_p->data_p_p = new_data_p_p;
	array_p->total_size = new_total_size;

	return 1;
}

int shrink_array(array* array_p, unsigned int new_total_size)
{
	// new_total_size must be lesser than the old_total_size
	if(new_total_size >= array_p->total_size)
		return 0;

	// reallocate memory for the new_total_size
	const void** new_data_p_p = reallocate(array_p->array_mem_allocator,
										array_p->data_p_p,
										array_p->total_size * sizeof(void*),
										new_total_size * sizeof(void*));

	// since memory allocation failed, return 0
	if(new_data_p_p == NULL && new_total_size > 0)
		return 0;

	// new assignment to data_p_p and the total_size
	array_p->data_p_p = new_data_p_p;
	array_p->total_size = new_total_size;

	return 1;
}

static void print_array_element_wrapper(void* element, unsigned int index, const void* print_element)
{
	printf("\telement_index %u -> ", index);
	if(element != NULL)
		((void (*)(const void*))print_element)(element);
	else
		printf("NULL");
	printf("\n");
}

void print_array(const array* array_p, void (*print_element)(const void* data_p))
{
	printf("array:");
	printf("\n\ttotal size : %u\n", array_p->total_size);
	for_each_in_array(array_p, print_array_element_wrapper, print_element);
	printf("\n");
}
