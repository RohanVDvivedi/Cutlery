#include<hashmap.h>

hashmap* get_hashmap(unsigned long long int bucket_count, unsigned long long int (*hash_function)(void* key))
{
	hashmap* hashmap_p = ((hashmap*) calloc(1, sizeof(hashmap)));
	hashmap_p->bucket_count = bucket_count;
	hashmap_p->hash_function = hash_function;
	hashmap_p->buckets = ((bucket**) calloc(hashmap_p->bucket_count, sizeof(bucket*)));
	return hashmap_p;
}

bucket* get_bucket(void* key, unsigned long long int size_of_key, void* value, unsigned long long int size_of_value)
{
	bucket* bucket_p = ((bucket*) malloc(sizeof(bucket)));

	// setup key for the bucket
	bucket_p->size_of_key = size_of_key;
	bucket_p->key = malloc(bucket_p->size_of_key);
	memcpy(bucket_p->key, key, bucket_p->size_of_key);

	// setup value for the bucket
	bucket_p->size_of_value = size_of_value;
	bucket_p->value = malloc(bucket_p->size_of_value);
	memcpy(bucket_p->value, value, bucket_p->size_of_value);

	return bucket_p;
}

unsigned long long int get_index(hashmap* hashmap_p, void* key)
{
	// calculate hash
	unsigned long long int hash = hashmap_p->hash_function(key);

	// calculate index
	unsigned long long int index = hash % hashmap_p->bucket_count;

	return index;
}

void put(hashmap* hashmap_p, bucket* bucket_p)
{
	// get index
	unsigned long long int index = get_index(hashmap_p, bucket_p->key);

	// find the bucket in the hashmap, which has the same key as this bucket
	bucket* found_bucket_p = get(hashmap_p, bucket_p->key, bucket_p->size_of_key);

	// if bucket with that key exists, remove and delete existing bucket and add new bucket
	if(found_bucket_p != NULL)
	{
		remove_bucket(hashmap_p, bucket_p->key, bucket_p->size_of_key);
	}

	// new bucket geta added before the first bucket, in the linked list
	bucket_p->next_bucket = hashmap_p->buckets[index];
	hashmap_p->buckets[index] = bucket_p;
	hashmap_p->occupancy++;
}

bucket* get(hashmap* hashmap_p, void* key, unsigned long long int size_of_key)
{
	// get index
	unsigned long long int index = get_index(hashmap_p, key);

	// find the bucket index group the bucket_p belongs to
	bucket* bucket_p = hashmap_p->buckets[index];

	// linear search through the linked list :(
	// conditions to keep looping in the search loop : bucket is not null && bucket not found
	// bucket not found = size of key does not match || key contents does not match
	while(bucket_p != NULL && (size_of_key != bucket_p->size_of_key || memcmp( key, bucket_p->key, size_of_key)))
	{
		bucket_p = bucket_p->next_bucket;
	}

	return bucket_p;
}

int remove_bucket(hashmap* hashmap_p, void* key, unsigned long long int size_of_key)
{
	// get index
	unsigned long long int index = get_index(hashmap_p, key);

	// find the bucket index group the bucket_p belongs to
	bucket* prev_bucket_p = NULL;
	bucket* bucket_p = hashmap_p->buckets[index];

	// this will be set if the bucket with corresponding key is found
	int found = 0;

	while( ( bucket_p != NULL && (! (size_of_key == bucket_p->size_of_key && (!memcmp(key, bucket_p->key, size_of_key))) ) ) || (bucket_p != NULL && (found = 1) && 0))
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
		delete_bucket(bucket_p);
		hashmap_p->occupancy--;
	}

	return found;
}

void rehash_to_size(hashmap* hashmap_p, unsigned long long int new_bucket_size)
{
	// make a local new hashmap properties holder
	hashmap new_hashmap_properties = { .bucket_count = new_bucket_size, .hash_function = hashmap_p->hash_function, .buckets = ((bucket**) calloc(new_bucket_size, sizeof(bucket*)))};

	// iterate over all the elements in the hashmap_p
	// and add them to the newly statically maintained hashmap
	for(unsigned long long int index = 0; index < hashmap_p->bucket_count; index++)
	{
		while(hashmap_p->buckets[index] != NULL)
		{
			bucket* bucket_p = hashmap_p->buckets[index];
			hashmap_p->buckets[index] = hashmap_p->buckets[index]->next_bucket;
			put(&new_hashmap_properties, bucket_p);
		}
	}

	// free buckets data of the old hashmap
	free(hashmap_p->buckets);

	// assign it to the new hash map
	hashmap_p->bucket_count = new_hashmap_properties.bucket_count;
	hashmap_p->buckets = new_hashmap_properties.buckets;
}

void print_bucket(bucket* bucket_p, void (*print_key)(void* key), void (*print_value)(void* value))
{
	printf("\t");
	print_key(bucket_p->key);
	printf(" => ");
	print_value(bucket_p->value);
	printf("\n");
}

void print_hashmap(hashmap* hashmap_p, void (*print_key)(void* key), void (*print_value)(void* value))
{
	// iterate over all the elements in the hashmap_p
	printf("bucket_count %lld\n", hashmap_p->bucket_count);
	printf("occupancy %lld\n", hashmap_p->occupancy);
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
				delete_bucket(bucket_p);
				bucket_p = next_bucket;
			}
		}
		free(hashmap_p->buckets);
	}
	free(hashmap_p);
}

void delete_bucket(bucket* bucket_p)
{
	if(bucket_p->key != NULL)
	{
		free(bucket_p->key);
	}
	if(bucket_p->value != NULL)
	{
		free(bucket_p->value);
	}
	free(bucket_p);
}