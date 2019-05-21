#include<array.h>

array* get_array(unsigned long long int min_size, unsigned long long int size_of_data_element)
{
	array* array_p = (array*) calloc(1, sizeof(array));
	array_p->max_size = min_size;
	array_p->size = 0;
	array_p->size_of_data_element = size_of_data_element;
	array_p->increment_offset = 1;
	array_p->increment_factor = 2;
	array_p->data_p_p = array_p->max_size > 0 ? ((void**)calloc(array_p->max_size, sizeof(void*))): NULL;
}

void delete_array(array* array_p)
{
	for(unsigned long long int i = 0; i < array_p->size; i++)
	{
		if(array_p->data_p_p[i] != NULL)
		{
			free(array_p->data_p_p[i]);
		}
	}
	if(array_p->max_size > 0 && array_p->data_p_p != NULL)
	{
		free(array_p->data_p_p);
	}
	if(array_p != NULL)
	{
		free(array_p);
	}
}

void* get_element(array* array_p, unsigned long long int index)
{
	if(array_p->max_size > index)
	{
		return ((void*)(((char*)array_p->data_p_p)[index]));
	}
	else
	{
		return NULL;
	}
}

void append_element(array* array_p, const void* data_p)
{
	if(array_p->max_size < array_p->size && array_p->data_p_p != NULL)
	{
		// you fuck made an error
		return ;	
	}
	else if(array_p->max_size == array_p->size)
	{
		// compute new_size
		unsigned long long int new_max_size = ( array_p->max_size * array_p->increment_factor ) + array_p->increment_offset;

		// taking precaution
		new_max_size = new_max_size == i ? new_max_size + 1 : new_size;

		// request memory for the new computed size
		void** new_data_p_p = ((void**)calloc(array_p->max_size, sizeof(void*)));

		// copy all pointers from the old pointers array
		for(int i = 0; i < array_p->size; i++)
		{
			((char*)new_data_p_p[i]) = ((char*)array_p->data_p_p)[i];
		}

		// free the old pointers array
		free(array_p->data_p_p);

		// new assignment to data_p_p and the max_size
		array_p->data_p_p = new_data_p_p;
		array_p->max_size = new_max_size;
	}
	if(data_p != NULL)
	{
		// get pointer to to new data memory required
		void* new_data_p = calloc(1, size_of_data_element);

		// copy data from user's pointer to array's memory that we requested above
		for(unsigned long long int i = 0; i < size_of_data_element; i++)
		{
			// word by word :p
			((char*)new_data_p)[i] = ((char*)data_p)[i];
		}

		// assign a new data there and then increment the current size of the array
		array_p->data_p_p[array_p->size++] = new_data_p;
	}
}

void print_array(array* array_p, void (*print_element)(void*))
{
	printf("\narray:")
	printf("\n\tsize : %lld", array_p->size);
	printf("\n\tmax_size : %lld", array_p->max_size);
	printf("\n\tsize_of_data_element : %lld", array_p->size_of_data_element);
	for(unsigned long long int i = 0; i<array_p->size; i++)
	{
		printf("\n\telement_index %lld -> ", i);
		print_element(((char*)array_p->data)[i]);
	}
	printf("\n\tincrement_factor : %lld", array_p->increment_factor);
	printf("\n\tincrement_offset : %lld", array_p->increment_offset);
}