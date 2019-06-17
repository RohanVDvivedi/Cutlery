#include<array.h>

array* get_array(unsigned long long int initial_size, unsigned long long int size_of_data_element)
{
	array* array_p = (array*) calloc(1, sizeof(array));
	array_p->size_of_data_element = size_of_data_element;
	array_p->increment_offset = 1;
	array_p->increment_factor = 2;
	array_p->data_p_p = initial_size > 0 ? ((void**)calloc(initial_size, sizeof(void*))): NULL;
	array_p->total_size = initial_size;
	return array_p;
}

void delete_array(array* array_p)
{
	if(array_p->total_size > 0 && array_p->data_p_p != NULL)
	{
		for(unsigned long long int i = 0; i < array_p->total_size; i++)
		{
			set_element(array_p, NULL, i);
		}
		free(array_p->data_p_p);
	}
	free(array_p);
}

const void* get_element(array* array_p, unsigned long long int index)
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
		// if there was no data there previously
		if(array_p->data_p_p[index] == NULL)
		{
			if(data_p != NULL)
			{
				// this is the pointer that will hold the new element data memory allocated by this array structure
				void* new_data_p = NULL;

				// assign by allocating memory using malloc
				new_data_p = malloc(array_p->size_of_data_element);

				// copy data from user's pointer to array's element memory that we requested above
				memcpy(new_data_p, data_p, array_p->size_of_data_element);

				// assign a new data there and then increment the current size of the array
				array_p->data_p_p[index] = new_data_p;
			}
		}
		else
		{
			// if array element is not null and the new data pointer is not null
			// we just do memcopy
			if(data_p != NULL)
			{
				// copy data from user's pointer to array's element memory that we requested above
				memcpy(array_p->data_p_p[index], data_p, array_p->size_of_data_element);
			}
			// if set data_p is NULL
			// we free the data
			else
			{
				// free the memory assigned to that element
				free(array_p->data_p_p[index]);

				// dont forget to assign it to NULL
				array_p->data_p_p[index] = NULL;
			}
		}

		return 0;
	}
	else
	{
		// this means you made a boo boo
		return -1;
	}
}

void expand_array(array* array_p)
{
	// compute new_size
	unsigned long long int new_total_size = ( array_p->total_size * array_p->increment_factor ) + array_p->increment_offset;

	// request memory for the new computed size
	void** new_data_p_p = ((void**)calloc(new_total_size, sizeof(void*)));

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

void print_array(array* array_p, void (*print_element)(const void* data_p))
{
	printf("\narray:");
	printf("\n\ttotal size : %lld", array_p->total_size);
	printf("\n\tsize_of_data_element : %lld", array_p->size_of_data_element);
	for(unsigned long long int i = 0; i < array_p->total_size; i++)
	{
		printf("\n\telement_index %lld -> ", i);
		const void* element = get_element(array_p, i);
		if(element != NULL)
		{
			print_element(get_element(array_p, i));
		}
		else
		{
			printf("NULL");
		}
	}
	printf("\n\tincrement_factor : %lld", array_p->increment_factor);
	printf("\n\tincrement_offset : %lld\n\n", array_p->increment_offset);
}
