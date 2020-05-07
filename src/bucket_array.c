#include<bucket_array.h>

void for_each_in_bucket_array(array* array_p, void (*operation)(void* key, void* value, unsigned long long int index, void* additional_params), void* additional_params)
{
	for(unsigned long long int i = 0; i < get_capacity_bucket_array(array_p); i++)
	{
		if(get_key_bucket_array(array_p, i) != NULL)
		{
			operation(((void*)get_key_bucket_array(array_p, i)), ((void*)get_value_bucket_array(array_p, i)), i, additional_params);
		}
	}
}