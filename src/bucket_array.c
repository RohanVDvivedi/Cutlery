#include<bucket_array.h>

array* get_bucket_array(unsigned long long int initial_size)
{
	return get_array(initial_size * BUCKET_SIZE);
}

void initialize_bucket_array(array* array_p, unsigned long long int initial_size)
{
	initialize_array(array_p, initial_size * BUCKET_SIZE);
}

unsigned long long int get_capacity_bucket_array(const array* array_p)
{
	return array_p->total_size / BUCKET_SIZE;
}

const void* get_key_bucket_array(const array* array_p, unsigned long long int index)
{
	return get_element(array_p, KEY_INDEX(index));
}

const void* get_value_bucket_array(const array* array_p, unsigned long long int index)
{
	return get_element(array_p, VALUE_INDEX(index));
}

int insert_in_bucket_array(array* array_p, const void* key_p, const void* value_p, unsigned long long int index)
{
	return (set_element(array_p, value_p, VALUE_INDEX(index)) || set_element(array_p, key_p, KEY_INDEX(index)));
}

int shrink_bucket_array(array* array_p, unsigned long long int start_index, unsigned long long int end_index)
{
	return shrink_array(array_p, KEY_INDEX(start_index), VALUE_INDEX(end_index));
}

void for_each_in_bucket_array(const array* array_p, void (*operation)(const void* key, const void* value, unsigned long long int index, const void* additional_params), const void* additional_params)
{
	for(unsigned long long int i = 0; i < get_capacity_bucket_array(array_p); i++)
	{
		if(get_key_bucket_array(array_p, i) != NULL)
		{
			operation(((void*)get_key_bucket_array(array_p, i)), ((void*)get_value_bucket_array(array_p, i)), i, additional_params);
		}
	}
}

void for_each_entry_in_bucket_array(const array* array_p, void (*operation)(const void* key, const void* value, const void* additional_params), const void* additional_params)
{
	for(unsigned long long int i = 0; i < get_capacity_bucket_array(array_p); i++)
	{
		if(get_key_bucket_array(array_p, i) != NULL)
		{
			operation(((void*)get_key_bucket_array(array_p, i)), ((void*)get_value_bucket_array(array_p, i)), additional_params);
		}
	}
}

void print_bucket_array(const array* array_p, void (*print_key)(const void* key), void (*print_value)(const void* value))
{
	for(unsigned long long int i = 0; i < get_capacity_bucket_array(array_p); i++)
	{
		printf("\tindex : %llu\n", i);
		printf("\t\t");
		if(get_key_bucket_array(array_p, i) != NULL)
		{
			printf("\t");
			print_key(get_key_bucket_array(array_p, i));
			printf(" => ");
			print_value(get_value_bucket_array(array_p, i));
			printf("\n");
		}
		else
		{
			printf("\tNULL");
		}
		printf("\n");
	}
}

void swap_buckets_bucket_array(array* array_p, unsigned long long int i1, unsigned long long int i2)
{
	void* key_p_i1 = ((void*)get_key_bucket_array(array_p, i1));
	void* value_p_i1 = ((void*)get_value_bucket_array(array_p, i1));

	void* key_p_i2 = ((void*)get_key_bucket_array(array_p, i2));
	void* value_p_i2 = ((void*)get_value_bucket_array(array_p, i2));

	insert_in_bucket_array(array_p, key_p_i1, value_p_i1, i2);
	insert_in_bucket_array(array_p, key_p_i2, value_p_i2, i1);
}