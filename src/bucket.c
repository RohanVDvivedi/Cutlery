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

void delete_bucket(bucket* bucket_p)
{
	free(bucket_p);
}