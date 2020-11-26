#include<array.h>

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// this is the factor, by which the previous size of data_p_p will be incremented
static const float increment_factor = 1.8;

// this is the constant increment amount, over the increment factor
static const unsigned char increment_offset = 1;

// new_total_size of data_p_p = old_total_size of data_p_p * increment_factor + increment_offset

// the below function will calculate the next size (on expansion) for the given array, if it's current size is current_size
static unsigned int next_expansion_size(unsigned int current_size)
{
	return ((unsigned int)(current_size * increment_factor)) + increment_offset;
}

void initialize_array(array* array_p, unsigned int initial_size)
{
	array_p->data_p_p = initial_size > 0 ? calloc(initial_size, sizeof(void*)) : NULL;
	array_p->total_size = initial_size;
	array_p->initial_size = initial_size;
}

void deinitialize_array(array* array_p)
{
	if(array_p->total_size > 0 && array_p->data_p_p != NULL)
	{
		free(array_p->data_p_p);
	}
	array_p->total_size = 0;
	array_p->initial_size = 0;
	array_p->data_p_p = NULL;
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
		return 0;
	}
	return -1;
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
	// compute new_size
	unsigned int new_total_size = next_expansion_size(array_p->total_size);

	// request memory for the new computed size
	const void** new_data_p_p = calloc(new_total_size, sizeof(void*));

	// since memory allocation failed, return 0
	if(new_data_p_p == NULL)
		return 0;

	// copy all pointers from the old pointers array
	// larger the array larger is this task, O(n) for single expansion of array
	// think of doing it multiple times, and you are done, in case you have a increment_factor 0,
	// and increment_amount = 1
	memcpy(new_data_p_p, array_p->data_p_p, array_p->total_size * sizeof(void*));

	// free the old pointers array
	free(array_p->data_p_p);

	// new assignment to data_p_p and the total_size
	array_p->data_p_p = new_data_p_p;
	array_p->total_size = new_total_size;

	// expansion of the array container is successfull
	return 1;
}

int shrink_array(array* array_p, unsigned int start_index, unsigned int end_index)
{
	if(end_index < start_index)
		return 0;

	unsigned int minimum_size = end_index - start_index + 1;

	unsigned int maximum_size = next_expansion_size(next_expansion_size(minimum_size));

	if(array_p->total_size > maximum_size)
	{
		unsigned int new_total_size = next_expansion_size(minimum_size);

		// The array is not allowed to shrink below its initial size,
		// it shrinks only if the new_total_size is greater than or equal to the initial size
		if(new_total_size >= array_p->initial_size)
		{
			// request memory for the new computed size
			const void** new_data_p_p = ((const void**)calloc(new_total_size, sizeof(void*)));

			memcpy(new_data_p_p, array_p->data_p_p + start_index, minimum_size * sizeof(void*));

			// free the old pointers array
			free(array_p->data_p_p);

			// new assignment to data_p_p and the total_size
			array_p->data_p_p = new_data_p_p;
			array_p->total_size = new_total_size;

			return 1;
		}
	}

	return 0;
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
	printf("\n\tincrement_factor : %f", increment_factor);
	printf("\n\tincrement_offset : %u", increment_offset);
	printf("\n\ttotal size : %u\n", array_p->total_size);
	for_each_in_array(array_p, print_array_element_wrapper, print_element);
	printf("\n");
}
