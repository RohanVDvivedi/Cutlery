#include<array.h>

array* get_array(unsigned long long int initial_size)
{
	array* array_p = (array*) calloc(1, sizeof(array));
	array_p->increment_offset = 1;
	array_p->increment_factor = 2;
	array_p->data_p_p = initial_size > 0 ? ((const void**)calloc(initial_size, sizeof(void*))): NULL;
	array_p->total_size = initial_size;
	return array_p;
}

void delete_array(array* array_p)
{
	if(array_p->total_size > 0 && array_p->data_p_p != NULL)
	{
		free(array_p->data_p_p);
	}
	free(array_p);
}

const void* get_element(const array* array_p, unsigned long long int index)
{
	if(array_p->total_size > index)
	{
		return array_p->data_p_p[index];
	}
	else
	{
		return NULL;
	}
}

int set_element(array* array_p, const void* data_p, unsigned long long int index)
{
	if(array_p->total_size > index)
	{
		array_p->data_p_p[index] = data_p;
		return 0;
	}
	else
	{
		return -1;
	}
}

void for_each_in_array(const array* array_p, void (*operation)(void* data_p))
{
	for(unsigned long long int i = 0; i < array_p->total_size; i++)
	{
		operation(((void*)get_element(array_p, i)));
	}
}

unsigned long long int find_first_in_array(const array* array_p, void* data_p, int (*compare)(const void* data_p1, const void* data_p2))
{
	for(unsigned long long int i = 0; i < array_p->total_size; i++)
	{
		if(compare(get_element(array_p, i), data_p) == 0)
		{
			return i;
		}
	}
	return array_p->total_size + 1;
}

void expand_array(array* array_p)
{
	// compute new_size
	unsigned long long int new_total_size = ( array_p->total_size * array_p->increment_factor ) + array_p->increment_offset;

	// request memory for the new computed size
	const void** new_data_p_p = ((const void**)calloc(new_total_size, sizeof(void*)));

	// copy all pointers from the old pointers array
	// larger the array larger is this task, O(n) for single expansion of array
	// think of doing it multiple times, and you are done, in case you have a increment_factor 0,
	// and increment_amount = 1
	memcpy(new_data_p_p, array_p->data_p_p, array_p->total_size * sizeof(void**));

	// free the old pointers array
	free(array_p->data_p_p);

	// new assignment to data_p_p and the total_size
	array_p->data_p_p = new_data_p_p;
	array_p->total_size = new_total_size;
}

void print_array(const array* array_p, void (*print_element)(const void* data_p))
{
	printf("\narray:");
	printf("\n\tincrement_factor : %lld", array_p->increment_factor);
	printf("\n\tincrement_offset : %lld", array_p->increment_offset);
	printf("\n\ttotal size : %lld\n", array_p->total_size);
	for(unsigned long long int i = 0; i < array_p->total_size; i++)
	{
		printf("\telement_index %lld -> ", i);
		const void* element = get_element(array_p, i);
		if(element != NULL)
		{
			print_element(get_element(array_p, i));
		}
		else
		{
			printf("NULL");
		}
		printf("\n");
	}
}
