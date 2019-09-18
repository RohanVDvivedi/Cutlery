#include<hashmap.h>

hashmap* get_hashmap(unsigned long long int bucket_count, unsigned long long int (*hash_function)(const void* key), int (*key_compare)(const void* key1, const void* key2), collision_resolution_policy hashmap_policy)
{
	hashmap* hashmap_p = ((hashmap*) calloc(1, sizeof(hashmap)));
	hashmap_p->hashmap_policy = hashmap_policy;
	hashmap_p->hash_function = hash_function;
	hashmap_p->key_compare = key_compare;

	// initialize array of the hashmap, it is a buckets_holder
	hashmap_p->buckets_holder = get_array(bucket_count);
	return hashmap_p;
}

// utility :-> gets index after hashing and mod of the hash
unsigned long long int get_index_for_key(const hashmap* hashmap_p, const void* key)
{
	// calculate hash
	unsigned long long int hash = hashmap_p->hash_function(key);

	// calculate index
	unsigned long long int index = hash % hashmap_p->buckets_holder->total_size;

	return index;
}

// utility :-> if future this could be a bst as well
const void* get_data_structure_for_key(const hashmap* hashmap_p, const void* key)
{
	// get index for that key
	unsigned long long int index = get_index_for_key(hashmap_p, key);

	// get the data structure at that index
	linkedlist* ll = (linkedlist*)get_element(hashmap_p->buckets_holder, index);

	// if the data structure is NULL
	if(ll == NULL)
	{
		// create a new linked list
		ll = get_linkedlist();

		// set it at the index there
		set_element(hashmap_p->buckets_holder, ll, index);
	}

	// return the data structure at that index
	return ll;
}

bucket* find_bucket(const hashmap* hashmap_p, const void* key)
{
	// get the linkedlist at that index
	linkedlist* linkedlist_p = (linkedlist*)get_data_structure_for_key(hashmap_p, key);

	// linear search through the linked list :(
	return (bucket*)(find_first_in_list(linkedlist_p, &((bucket){key,NULL}), ((int (*)(const void*, const void*, const void*))(bucket_compare)), hashmap_p->key_compare));
}

const void* find_value(const hashmap* hashmap_p, const void* key)
{
	bucket* found_bucket_p = find_bucket(hashmap_p, key);
	return ((found_bucket_p == NULL) ? NULL : found_bucket_p->value);
}

void put_entry(hashmap* hashmap_p, const void* key, const void* value)
{
	// find the bucket in the hashmap, which has the same key as this bucket
	bucket* found_bucket_p = find_bucket(hashmap_p, key);

	// if bucket with that key exists, then update its value pointer
	if(found_bucket_p != NULL)
	{
		found_bucket_p->value = value;
	}
	else
	{
		// get the linked list for the given key
		linkedlist* linkedlist_p = (linkedlist*)get_data_structure_for_key(hashmap_p, key);
		
		// then insert the new element at its head
		insert_head(linkedlist_p, get_bucket(key, value));
	}
}

int remove_value(hashmap* hashmap_p, const void* key, const void** return_key, const void** return_value)
{
	// get the linked list for the given key
	linkedlist* linkedlist_p = (linkedlist*)get_data_structure_for_key(hashmap_p, key);

	bucket* found_bucket_p = NULL;

	// offloading the find and delete task to the linkedlist itself
	int has_been_deleted = remove_from_list(linkedlist_p, &((bucket){key,NULL}), ((int (*)(const void*, const void*, const void*))(bucket_compare)), hashmap_p->key_compare, ((const void**)(&found_bucket_p)));

	// if a bucket was ther in the liunkedlist, we have to set the return keys and values so that the client can delete them
	// also delete the bucket itself
	if(found_bucket_p != NULL)
	{
		if(return_key != NULL)
		{
			(*(return_key)) = found_bucket_p->key;
		}
		if(return_value != NULL)
		{
			(*(return_value)) = found_bucket_p->value;
		}
		delete_bucket(found_bucket_p);
	}

	return has_been_deleted;
}

void put_bucket_wrapper(void* bucket_p_v, const void* hashmap_p_v)
{
	bucket* bucket_p = ((bucket*)(bucket_p_v));
	hashmap* hashmap_p = ((hashmap*)(hashmap_p_v));

	// if the index is pointing to NULL linedlist, it has to be initialized first
	linkedlist* ll = (linkedlist*)get_data_structure_for_key(hashmap_p, bucket_p->key);

	// insert the new bucket at its head
	insert_head(ll, bucket_p);
}

void rehash_to_size(hashmap* hashmap_p, unsigned long long int new_bucket_size)
{
	// make a local new hashmap properties holder
	hashmap new_hashmap_properties = {.hash_function = hashmap_p->hash_function, .key_compare = hashmap_p->key_compare, .buckets_holder = get_array(new_bucket_size)};

	// iterate over all the elements in the hashmap_p
	// and add them to the newly statically maintained hashmap
	for(unsigned long long int index = 0; index < hashmap_p->buckets_holder->total_size; index++)
	{
		linkedlist* linkedlist_p = ((linkedlist*)get_element(hashmap_p->buckets_holder, index));
		if(linkedlist_p != NULL)
		{
			// iterate over all the elements of the linkedlist
			for_each_in_list(linkedlist_p, put_bucket_wrapper, &new_hashmap_properties);

			// delete linkedlist
			delete_linkedlist(linkedlist_p);

			// since the hashmap is deleted, we set the pointer of linkedlist to NULL, in the array
			set_element(hashmap_p->buckets_holder, NULL, index);
		}
	}

	// delete bucket array and assign a new one to it
	delete_array(hashmap_p->buckets_holder);

	// by reassign the values to it from the new hash map
	(*hashmap_p) = new_hashmap_properties;
}

void print_bucket_wrapper(void* bucket_p_to_print, const void* bucket_p_functions)
{
	bucket* bucket_p_to_print_t = ((bucket*)bucket_p_to_print);
	bucket* bucket_p_functions_t = ((bucket*)bucket_p_functions);
	print_bucket(bucket_p_to_print_t, bucket_p_functions_t->key, bucket_p_functions_t->value);
}

void print_hashmap(const hashmap* hashmap_p, void (*print_key)(const void* key), void (*print_value)(const void* value))
{
	bucket print_functions = {.key = print_key, .value = print_value};
	// iterate over all the elements (linkedlists) in the hashmap_p
	for(unsigned long long int index = 0; index < hashmap_p->buckets_holder->total_size; index++)
	{
		printf("index = %lld\n", index);
		linkedlist* linkedlist_p = ((linkedlist*)get_element(hashmap_p->buckets_holder, index));
		if(linkedlist_p != NULL)
		{
			for_each_in_list(linkedlist_p, print_bucket_wrapper, ((const void*)(&print_functions)));
		}
	}
	printf("\n\n");
}

void delete_bucket_wrapper(void* bucket_p, const void* additional_params)
{
	delete_bucket(((bucket*)(bucket_p)));
}

void delete_hashmap(hashmap* hashmap_p)
{
	for(unsigned long long int i = 0; i < hashmap_p->buckets_holder->total_size; i++)
	{
		linkedlist* linkedlist_p = ((linkedlist*)get_element(hashmap_p->buckets_holder, i));
		if(linkedlist_p != NULL)
		{
			for_each_in_list(linkedlist_p, delete_bucket_wrapper, NULL);
			delete_linkedlist(linkedlist_p);
			set_element(hashmap_p->buckets_holder, NULL, i);
		}
	}
	delete_array(hashmap_p->buckets_holder);
	free(hashmap_p);
}