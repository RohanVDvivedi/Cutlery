#include<mmarray.h>

mmarray* get_mmarray(unsigned long long int initial_size, unsigned long long int size_of_data_element)
{
	mmarray* mmarray_p = (mmarray*) calloc(1, sizeof(mmarray));
	mmarray_p->size_of_data_element = size_of_data_element;
	mmarray_p->increment_offset = 1;
	mmarray_p->increment_factor = 2;
	mmarray_p->data_p_p = initial_size > 0 ? ((void**)calloc(initial_size, sizeof(void*))): NULL;
	mmarray_p->total_size = initial_size;
	return mmarray_p;
}

void delete_mmarray(mmarray* mmarray_p)
{
	if(mmarray_p->total_size > 0 && mmarray_p->data_p_p != NULL)
	{
		for(unsigned long long int i = 0; i < mmarray_p->total_size; i++)
		{
			set_mmarray_element(mmarray_p, NULL, i);
		}
		free(mmarray_p->data_p_p);
	}
	free(mmarray_p);
}

const void* get_mmarray_element(const mmarray* mmarray_p, unsigned long long int index)
{
	if(mmarray_p->total_size > index)
	{
		return mmarray_p->data_p_p[index];
	}
	else
	{
		return NULL;
	}
}

int set_mmarray_element(mmarray* mmarray_p, const void* data_p, unsigned long long int index)
{
	if(mmarray_p->total_size > index)
	{
		// if there was no data there previously
		if(mmarray_p->data_p_p[index] == NULL)
		{
			if(data_p != NULL)
			{
				// this is the pointer that will hold the new element data memory allocated by this mmarray structure
				void* new_data_p = NULL;

				// assign by allocating memory using malloc
				new_data_p = malloc(mmarray_p->size_of_data_element);

				// copy data from user's pointer to mmarray's element memory that we requested above
				memcpy(new_data_p, data_p, mmarray_p->size_of_data_element);

				// assign a new data there and then increment the current size of the mmarray
				mmarray_p->data_p_p[index] = new_data_p;
			}
		}
		else
		{
			// if mmarray element is not null and the new data pointer is not null
			// we just do memcopy
			if(data_p != NULL)
			{
				// copy data from user's pointer to mmarray's element memory that we requested above
				memcpy(mmarray_p->data_p_p[index], data_p, mmarray_p->size_of_data_element);
			}
			// if set data_p is NULL
			// we free the data
			else
			{
				// free the memory assigned to that element
				free(mmarray_p->data_p_p[index]);

				// dont forget to assign it to NULL
				mmarray_p->data_p_p[index] = NULL;
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

void expand_mmarray(mmarray* mmarray_p)
{
	// compute new_size
	unsigned long long int new_total_size = ( mmarray_p->total_size * mmarray_p->increment_factor ) + mmarray_p->increment_offset;

	// request memory for the new computed size
	void** new_data_p_p = ((void**)calloc(new_total_size, sizeof(void*)));

	// copy all pointers from the old pointers mmarray
	// larger the mmarray larger is this task, O(n) for single expansion of mmarray
	// think of doing it multiple times, and you are done, in case you have a increment_factor 0,
	// and increment_amount = 1
	memcpy(new_data_p_p, mmarray_p->data_p_p, mmarray_p->total_size * sizeof(void**));

	// free the old pointers mmarray
	free(mmarray_p->data_p_p);

	// new assignment to data_p_p and the total_size
	mmarray_p->data_p_p = new_data_p_p;
	mmarray_p->total_size = new_total_size;
}

void print_mmarray(const mmarray* mmarray_p, void (*print_element)(const void* data_p))
{
	printf("\nmmarray:");
	printf("\n\ttotal size : %lld", mmarray_p->total_size);
	printf("\n\tsize_of_data_element : %lld", mmarray_p->size_of_data_element);
	for(unsigned long long int i = 0; i < mmarray_p->total_size; i++)
	{
		printf("\n\telement_index %lld -> ", i);
		const void* element = get_mmarray_element(mmarray_p, i);
		if(element != NULL)
		{
			print_element(get_mmarray_element(mmarray_p, i));
		}
		else
		{
			printf("NULL");
		}
	}
	printf("\n\tincrement_factor : %lld", mmarray_p->increment_factor);
	printf("\n\tincrement_offset : %lld\n\n", mmarray_p->increment_offset);
}
