#include<hashmap.h>

hashmap* get_hashmap(unsigned long long int bucket_count, unsigned long long int (*hash_function)(const void* key), int (*key_compare)(const void* key1, const void* key2))
{
	hashmap* hashmap_p = ((hashmap*) calloc(1, sizeof(hashmap)));
	hashmap_p->bucket_count = bucket_count;
	hashmap_p->hash_function = hash_function;
	hashmap_p->key_compare = key_compare;
	hashmap_p->buckets = ((bucket**) calloc(hashmap_p->bucket_count, sizeof(bucket*)));
	return hashmap_p;
}

bucket* get_new_bucket(const void* key, const void* value)
{
	bucket* bucket_p = ((bucket*) malloc(sizeof(bucket)));
	// setup key for the bucket
	bucket_p->key = key;
	// setup value for the bucket
	bucket_p->value = value;
	bucket_p->next_bucket = NULL;
	return bucket_p;
}

unsigned long long int get_index(const hashmap* hashmap_p, const void* key)
{
	// calculate hash
	unsigned long long int hash = hashmap_p->hash_function(key);

	// calculate index
	unsigned long long int index = hash % hashmap_p->bucket_count;

	return index;
}

bucket* get_bucket(const hashmap* hashmap_p, const void* key)
{
	// get index
	unsigned long long int index = get_index(hashmap_p, key);

	// find the bucket index group the bucket_p belongs to
	bucket* bucket_p = hashmap_p->buckets[index];

	// linear search through the linked list :(
	// conditions to keep looping in the search loop : bucket is not null
	// && bucket's key_compare fails (i.e.non zero compare result)
	while(bucket_p != NULL && hashmap_p->key_compare(key, bucket_p->key) != 0)
	{
		bucket_p = bucket_p->next_bucket;
	}

	return bucket_p;
}

void put_entry(hashmap* hashmap_p, const void* key, const void* value)
{
	// get index
	unsigned long long int index = get_index(hashmap_p, key);

	// find the bucket in the hashmap, which has the same key as this bucket
	bucket* found_bucket_p = get_bucket(hashmap_p, key);

	// if bucket with that key exists, then update its value pointer
	if(found_bucket_p != NULL)
	{
		found_bucket_p->value = value;
	}
	else
	{
		// new bucket gets added before the first bucket, in the linked list
		bucket* new_bucket_p = get_new_bucket(key, value);
		new_bucket_p->next_bucket = hashmap_p->buckets[index];
		hashmap_p->buckets[index] = new_bucket_p;
	}
}

void put_bucket(hashmap* hashmap_p, bucket* bucket_p)
{
	// get index
	unsigned long long int index = get_index(hashmap_p, bucket_p->key);

	// find the bucket in the hashmap, which has the same key as this bucket
	bucket* found_bucket_p = get_bucket(hashmap_p, bucket_p->key);

	// if bucket with that key exists, then update its value pointer
	if(found_bucket_p != NULL)
	{
		remove_bucket(hashmap_p, bucket_p->key);
	}

	// new bucket gets added before the first bucket, in the linked list
	bucket_p->next_bucket = hashmap_p->buckets[index];
	hashmap_p->buckets[index] = bucket_p;
}

const void* get_value(const hashmap* hashmap_p, const void* key)
{
	const bucket* bucket_p = get_bucket(hashmap_p, key);
	return bucket_p == NULL ? NULL : bucket_p->value;
}

int remove_bucket(hashmap* hashmap_p, const void* key)
{
	// get index
	unsigned long long int index = get_index(hashmap_p, key);

	// find the bucket index group the bucket_p belongs to
	bucket* prev_bucket_p = NULL;
	bucket* bucket_p = hashmap_p->buckets[index];

	// this will be set if the bucket with corresponding key is found
	int found = 0;

	while( ( bucket_p != NULL && ( hashmap_p->key_compare( key, bucket_p->key) != 0 ) ) || (bucket_p != NULL && (found = 1) && 0))
	{
		prev_bucket_p = bucket_p;
		bucket_p = bucket_p->next_bucket;
	}

	// if the bucket with the same key is found, we remove it
	if(found)
	{
		if(prev_bucket_p != NULL)
		{
			prev_bucket_p->next_bucket = bucket_p->next_bucket;
		}
		else
		{
			hashmap_p->buckets[index] = bucket_p->next_bucket;
		}
		free(bucket_p);
	}

	return found;
}

void rehash_to_size(hashmap* hashmap_p, unsigned long long int new_bucket_size)
{
	// make a local new hashmap properties holder
	hashmap new_hashmap_properties = { .bucket_count = new_bucket_size, .hash_function = hashmap_p->hash_function, .key_compare = hashmap_p->key_compare, .buckets = ((bucket**) calloc(new_bucket_size, sizeof(bucket*)))};

	// iterate over all the elements in the hashmap_p
	// and add them to the newly statically maintained hashmap
	for(unsigned long long int index = 0; index < hashmap_p->bucket_count; index++)
	{
		while(hashmap_p->buckets[index] != NULL)
		{
			bucket* bucket_p = hashmap_p->buckets[index];
			hashmap_p->buckets[index] = hashmap_p->buckets[index]->next_bucket;
			put_bucket(&new_hashmap_properties, bucket_p);
		}
	}

	// free buckets data of the old hashmap
	free(hashmap_p->buckets);

	// assign it to the new hash map
	hashmap_p->bucket_count = new_hashmap_properties.bucket_count;
	hashmap_p->buckets = new_hashmap_properties.buckets;
}

void print_bucket(const bucket* bucket_p, void (*print_key)(const void* key), void (*print_value)(const void* value))
{
	printf("\t");
	print_key(bucket_p->key);
	printf(" => ");
	print_value(bucket_p->value);
	printf("\n");
}

void print_hashmap(const hashmap* hashmap_p, void (*print_key)(const void* key), void (*print_value)(const void* value))
{
	// iterate over all the elements in the hashmap_p
	printf("bucket_count %lld\n", hashmap_p->bucket_count);
	for(unsigned long long int index = 0; index < hashmap_p->bucket_count; index++)
	{
		printf("index = %lld\n", index);
		bucket* bucket_p = hashmap_p->buckets[index];
		while(bucket_p != NULL)
		{
			print_bucket(bucket_p, print_key, print_value);
			bucket_p = bucket_p->next_bucket;
		}
	}
	printf("\n\n");
}

void delete_hashmap(hashmap* hashmap_p)
{
	if(hashmap_p->bucket_count > 0)
	{
		for(unsigned long long int i = 0; i < hashmap_p->bucket_count; i++)
		{
			bucket* bucket_p = hashmap_p->buckets[i];
			while(bucket_p != NULL)
			{
				bucket* next_bucket = bucket_p->next_bucket;
				free(bucket_p);
				bucket_p = next_bucket;
			}
		}
		free(hashmap_p->buckets);
	}
	free(hashmap_p);
}