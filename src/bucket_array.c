#include<bucket_array.h>

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

void print_bucket_array(const array* array_p, void (*print_key)(const void* key), void (*print_value)(const void* value))
{
	for(unsigned long long int i = 0; i < get_capacity_bucket_array(array_p); i++)
	{
		printf("\tindex : %llu\n", index);
		printf("\t\t");
		if(get_key_bucket_array(array_p, i) != NULL)
		{
			printf("\t");
			print_key(get_key_bucket_array(array_p, i));
			printf(" => ");
			print_value(get_key_bucket_array(array_p, i));
			printf("\n");
		}
		else
		{
			printf("\tNULL");
		}
		printf("\n");
	}
}