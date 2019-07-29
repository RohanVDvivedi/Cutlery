#include<hashmap.h>

hashmap* get_hashmap(unsigned long long int bucket_count, unsigned long long int (*hash_function)(const void* key), int (*key_compare)(const void* key1, const void* key2))
{
	hashmap* hashmap_p = ((hashmap*) calloc(1, sizeof(hashmap)));
	hashmap_p->hash_function = hash_function;
	hashmap_p->key_compare = key_compare;
	hashmap_p->buckets_holder = get_array(bucket_count);
	return hashmap_p;
}

unsigned long long int get_index(const hashmap* hashmap_p, const void* key)
{
	// calculate hash
	unsigned long long int hash = hashmap_p->hash_function(key);

	// calculate index
	unsigned long long int index = hash % hashmap_p->buckets_holder->total_size;

	return index;
}

node* find_node(const hashmap* hashmap_p, const void* key)
{
	// get index
	unsigned long long int index = get_index(hashmap_p, key);

	// if the index is pointing to NULL it has to be initialized to a linkedlist
	if(get_element(hashmap_p->buckets_holder, index) == NULL)
	{
		return NULL;
	}

	// find the head of the linkedlist the bucket may lie in
	node* node_p = ((linkedlist*)get_element(hashmap_p->buckets_holder, index))->head;

	// linear search through the linked list :(
	// conditions to keep looping in the search loop : bucket is not null
	// && bucket's key_compare fails (i.e.non zero compare result)
	while(node_p != NULL && hashmap_p->key_compare(key, ((bucket*)node_p->data_p)->key) != 0)
	{
		node_p = node_p->next;
	}

	return node_p;
}

void put_entry(hashmap* hashmap_p, const void* key, const void* value)
{
	// get index
	unsigned long long int index = get_index(hashmap_p, key);

	// if the index is pointing to NULL it has to be initialized to a linkedlist
	if(get_element(hashmap_p->buckets_holder, index) == NULL)
	{
		set_element(hashmap_p->buckets_holder, get_linkedlist(), index);
	}

	// find the bucket in the hashmap, which has the same key as this bucket
	node* found_node_p = find_node(hashmap_p, key);
	bucket* found_bucket_p = ((found_node_p == NULL) ? NULL : ((bucket*)found_node_p->data_p));

	// if bucket with that key exists, then update its value pointer
	if(found_bucket_p != NULL)
	{
		found_bucket_p->value = value;
	}
	else
	{
		// new bucket gets added before the first bucket, in the linked list
		const bucket* new_bucket_p = get_bucket(key, value);
		linkedlist* linkedlist_p = ((linkedlist*)get_element(hashmap_p->buckets_holder, index));
		insert_head(linkedlist_p, new_bucket_p);
	}
}

void put_bucket(hashmap* hashmap_p, bucket* bucket_p)
{
	// get index
	unsigned long long int index = get_index(hashmap_p, bucket_p->key);

	// if the index is pointing to NULL it has to be initialized to a linkedlist
	if(get_element(hashmap_p->buckets_holder, index) == NULL)
	{
		set_element(hashmap_p->buckets_holder, get_linkedlist(), index);
	}

	// find the bucket in the hashmap, which has the same key as this bucket
	node* found_node_p = find_node(hashmap_p, bucket_p->key);
	bucket* found_bucket_p = ((found_node_p == NULL) ? NULL : ((bucket*)found_node_p->data_p));

	// if bucket with that key exists, then remove it
	if(found_bucket_p != NULL)
	{
		remove_value(hashmap_p, found_bucket_p->key);
	}

	// new bucket gets added before the first bucket, in the linked list
	linkedlist* linkedlist_p = ((linkedlist*)get_element(hashmap_p->buckets_holder, index));
	insert_head(linkedlist_p, bucket_p);
}

const void* find_value(const hashmap* hashmap_p, const void* key)
{
	node* found_node_p = find_node(hashmap_p, key);
	bucket* found_bucket_p = ((found_node_p == NULL) ? NULL : ((bucket*)found_node_p->data_p));
	return ((found_bucket_p == NULL) ? NULL : found_bucket_p->value);
}

int remove_value(hashmap* hashmap_p, const void* key)
{
	// get index
	unsigned long long int index = get_index(hashmap_p, key);

	// find the linkedlist in which the bucket may lie
	linkedlist* linkedlist_p = ((linkedlist*)get_element(hashmap_p->buckets_holder, index));

	// find the bucket index group the bucket_p belongs to
	node* found_node_p = find_node(hashmap_p, key);

	int return_val = ((found_node_p == NULL) ? 0 : 1);

	// remove the node of the particular bucket
	delete_bucket(((bucket*)found_node_p->data_p));
	remove_node(linkedlist_p, found_node_p);

	return return_val;
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
			node* node_p = linkedlist_p->head;
			while(node_p != NULL)
			{
				put_bucket(&new_hashmap_properties, ((bucket*)node_p->data_p));
				node_p = node_p->next;
			}
			delete_linkedlist(linkedlist_p);
			set_element(hashmap_p->buckets_holder, NULL, index);
		}
	}
	delete_array(hashmap_p->buckets_holder);

	// reassign the values to it from the new hash map
	(*hashmap_p) = new_hashmap_properties;
}

void print_hashmap(const hashmap* hashmap_p, void (*print_key)(const void* key), void (*print_value)(const void* value))
{
	// iterate over all the elements in the hashmap_p
	for(unsigned long long int index = 0; index < hashmap_p->buckets_holder->total_size; index++)
	{
		printf("index = %lld\n", index);
		linkedlist* linkedlist_p = ((linkedlist*)get_element(hashmap_p->buckets_holder, index));
		if(linkedlist_p != NULL)
		{
			node* node_p = linkedlist_p->head;
			while(node_p != NULL)
			{
				print_bucket(((bucket*)node_p->data_p), print_key, print_value);
				node_p = node_p->next;
			}
		}
	}
	printf("\n\n");
}

void delete_hashmap(hashmap* hashmap_p)
{
	for(unsigned long long int i = 0; i < hashmap_p->buckets_holder->total_size; i++)
	{
		linkedlist* linkedlist_p = ((linkedlist*)get_element(hashmap_p->buckets_holder, i));
		if(linkedlist_p != NULL)
		{
			node* node_p = linkedlist_p->head;
			while(node_p != NULL)
			{
				delete_bucket(((bucket*)node_p->data_p));
				node_p = node_p->next;
			}
			delete_linkedlist(linkedlist_p);
			set_element(hashmap_p->buckets_holder, NULL, i);
		}
	}
	delete_array(hashmap_p->buckets_holder);
	free(hashmap_p);
}