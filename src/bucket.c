#include<bucket.h>

bucket* get_bucket(const void* key, const void* value)
{
	bucket* bucket_p = ((bucket*) malloc(sizeof(bucket)));
	bucket_p->key = key;
	bucket_p->value = value;
	return bucket_p;
}

void delete_bucket(bucket* bucket_p)
{
	free(bucket_p);
}