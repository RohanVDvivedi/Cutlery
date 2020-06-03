#include<bucket.h>

void initialize_bucket(bucket* bucket_p, const void* key, const void* value)
{
	bucket_p->key = key;
	bucket_p->value = value;
}

void print_bucket(const bucket* bucket_p, void (*print_key)(const void* key), void (*print_value)(const void* value))
{
	printf("\t");
	print_key(bucket_p->key);
	printf(" => ");
	print_value(bucket_p->value);
	printf("\n");
}

int bucket_compare(const bucket* bucket_p1, const bucket* bucket_p2, int (*key_compare)(const void* key_1, const void* key_2))
{
	return key_compare(bucket_p1->key, bucket_p2->key);
}