#include<bucket.h>

const bucket* get_bucket(const void* key, const void* value)
{
	bucket* bucket_p = ((bucket*) malloc(sizeof(bucket)));
	bucket_p->key = key;
	bucket_p->value = value;
	return bucket_p;
}

void print_bucket(const bucket* bucket_p, void (*print_key)(const void* key), void (*print_value)(const void* value))
{
	printf("\t");
	print_key(bucket_p->key);
	printf(" => ");
	print_value(bucket_p->value);
	printf("\n");
}

void print_bucket_wrapper_for_array(void* bucket_p_to_print, unsigned long long int index, const void* bucket_p_functions)
{
	bucket* bucket_p_to_print_t = ((bucket*)bucket_p_to_print);
	bucket* bucket_p_functions_t = ((bucket*)bucket_p_functions);
	printf("\tindex : %llu\n", index);
	printf("\t\t");
	if(bucket_p_to_print_t != NULL)
	{
		print_bucket(bucket_p_to_print_t, bucket_p_functions_t->key, bucket_p_functions_t->value);
	}
	else
	{
		printf("\tNULL");
	}
	printf("\n");
}

void delete_bucket(bucket* bucket_p)
{
	free(bucket_p);
}

void delete_bucket_wrapper_for_array(void* data_p, unsigned long long int index, const void* additional_params)
{
	if(data_p != NULL)
	{
		delete_bucket(((bucket*)data_p));
	}
}

int bucket_compare(const bucket* bucket_p1, const bucket* bucket_p2, int (*key_compare)(const void* key_1, const void* key_2))
{
	return key_compare(bucket_p1->key, bucket_p2->key);
}