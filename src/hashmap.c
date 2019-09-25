#include<hashmap.h>

// to avoid name collision with functions of balancedbst
#define put_entry		put_entry_in_hash
#define find_value 		find_value_from_hash
#define remove_value 	remove_value_from_hash
#define for_each_entry 	for_each_entry_in_hash

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
// utility, O(1) operation
unsigned long long int get_index_for_key(const hashmap* hashmap_p, const void* key)
{
	// calculate hash
	unsigned long long int hash = hashmap_p->hash_function(key);

	// calculate index
	unsigned long long int index = hash % hashmap_p->buckets_holder->total_size;

	return index;
}

// utility, O(1) operation
const void* get_data_structure_for_index(const hashmap* hashmap_p, unsigned long long int index, int new_if_empty)
{
	// if you try accessing hashtable, index greater than its size
	if(index >= hashmap_p->buckets_holder->total_size)
	{
		return NULL;
	}

	// get the data structure at that index
	const void* ds_p = get_element(hashmap_p->buckets_holder, index);

	// if the data structure is NULL
	if(ds_p == NULL && new_if_empty)
	{
		switch(hashmap_p->hashmap_policy)
		{
			case NO_POLICY :
			{
				// create a new bucket with key = NULL, value = NULL
				ds_p = get_bucket(NULL, NULL);
				break;
			}
			case ELEMENTS_AS_LINKEDLIST :
			{
				// create a new linked list
				ds_p = get_linkedlist();
				break;
			}
			case ELEMENTS_AS_RED_BLACK_BST :
			{
				// create a new red black tree
				ds_p = get_balancedbst(RED_BLACK_TREE, hashmap_p->key_compare);
				break;
			}
			case ELEMENTS_AS_AVL_BST :
			{
				// create a new svl tree
				ds_p = get_balancedbst(AVL_TREE, hashmap_p->key_compare);
				break;
			}
		}

		// set it at the index there
		set_element(hashmap_p->buckets_holder, ds_p, index);
	}

	// return the data structure at that index
	return ds_p;
}

// utility, O(1) operation
const void* get_data_structure_for_key(const hashmap* hashmap_p, const void* key, int new_if_empty)
{
	// get index for that key
	unsigned long long int index = get_index_for_key(hashmap_p, key);

	// return the data structure at that index
	return get_data_structure_for_index(hashmap_p, index, new_if_empty);
}

bucket* find_bucket_for_no_policy_or_linkedlist_policy(const hashmap* hashmap_p, const void* key)
{
	// to store pointer to the bucket once it is found
	bucket* found_bucket_p = NULL;

	// get the data structure for that key, without any new creation
	const void* ds_p = get_data_structure_for_key(hashmap_p, key, 0);

	// if we found the under lying data struture, which infact han`dles collision, we find our bucket inside it
	if(ds_p != NULL)
	{
		switch(hashmap_p->hashmap_policy)
		{
			case NO_POLICY :
			{
				// if no policy ther is no middle ware data structure handling collision just bucket
				// hence compare key and return
				found_bucket_p = hashmap_p->key_compare(key, ((bucket*)ds_p)->key) == 0 ? ((bucket*)(ds_p)) : NULL;
				break;
			}
			case ELEMENTS_AS_LINKEDLIST :
			{
				// find in linked list
				found_bucket_p = (bucket*)(find_first_in_list(((linkedlist*)(ds_p)), &((bucket){key,NULL}), ((int (*)(const void*, const void*, const void*))(bucket_compare)), hashmap_p->key_compare));
				break;
			}
			case ELEMENTS_AS_AVL_BST :
			case ELEMENTS_AS_RED_BLACK_BST :
			{// bst data structures can only give you values, let found_bukcet_p = NULL
				break;
			}
		}
	}

	// return the bucket that we found
	return found_bucket_p;
}

const void* find_value(const hashmap* hashmap_p, const void* key)
{
	switch(hashmap_p->hashmap_policy)
	{
		case NO_POLICY :
		case ELEMENTS_AS_LINKEDLIST :
		{
			bucket* found_bucket_p = find_bucket_for_no_policy_or_linkedlist_policy(hashmap_p, key);
			return ((found_bucket_p == NULL) ? NULL : found_bucket_p->value);
		}
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			// get the bst data structure for that key, without any new creation
			const void* ds_p = get_data_structure_for_key(hashmap_p, key, 0);

			// find value in a bst
			return ((bucket*)(find_value_from_bst(((balancedbst*)(ds_p)), key)));
		}
	}
}

void put_entry(hashmap* hashmap_p, const void* key, const void* value)
{
	// find the bucket in the hashmap, which has the same key as this bucket
	bucket* found_bucket_p = find_bucket_for_no_policy_or_linkedlist_policy(hashmap_p, key);

	// if bucket with that key exists, then update its value pointer
	if(found_bucket_p != NULL)
	{
		// update the value of the bucket, if it is found 
		found_bucket_p->value = value;
	}
	// we insert a new bucket
	// else case works for bsts (because they have their own way of memory managing buckets), so we dont put dupka there
	// or if it is NO_POLICY or linked_list, where the buckets are maemory managed by hashmap only
	else
	{
		// retrieve data structure for that key
		void* ds_p = (void*)get_data_structure_for_key(hashmap_p, key, 1);

		switch(hashmap_p->hashmap_policy)
		{
			case NO_POLICY :
			{
				// if no policy ther is no middle ware data structure handling collision just bucket, so update its key and value
				((bucket*)ds_p)->key = key;
				((bucket*)ds_p)->value = value;
				break;
			}
			case ELEMENTS_AS_LINKEDLIST :
			{
				// insert the new bucket at its head of the linked list
				insert_head(((linkedlist*)(ds_p)), get_bucket(key, value));
				break;
			}
			case ELEMENTS_AS_AVL_BST :
			case ELEMENTS_AS_RED_BLACK_BST :
			{
				// insert the new bucket in the bst
				put_entry_in_bst(((balancedbst*)(ds_p)), key, value, PUT_IF_EXISTS|PUT_IF_NOT_EXISTS);
				break;
			}
		}
	}
}

int remove_value(hashmap* hashmap_p, const void* key, const void** return_key, const void** return_value)
{
	// this is where the bucket to be deleted gets stored
	bucket* found_bucket_p = NULL;

	// we are suppossed to return the number of entries we delete
	int has_been_deleted = 0;

	switch(hashmap_p->hashmap_policy)
	{
		case NO_POLICY :
		{
			found_bucket_p = find_bucket_for_no_policy_or_linkedlist_policy(hashmap_p, key);
			// if no policy, ds_p is only the bucket
			if(found_bucket_p != NULL)
			{
				// set element at index in array of buckets in the hashmap to NULL
				set_element(hashmap_p->buckets_holder, NULL, get_index_for_key(hashmap_p, key));
				has_been_deleted += 1;
			}
			break;
		}
		case ELEMENTS_AS_LINKEDLIST :
		{
			// get the data structure for the given key
			const void* ds_p = get_data_structure_for_key(hashmap_p, key, 0);

			// offloading the find and delete task to the linkedlist itself
			if(ds_p != NULL)
			{
				has_been_deleted += remove_from_list(((linkedlist*)(ds_p)), &((bucket){key,NULL}), ((int (*)(const void*, const void*, const void*))(bucket_compare)), hashmap_p->key_compare, ((const void**)(&found_bucket_p)));
			}
			break;
		}
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			// get the data structure for the given key
			const void* ds_p = get_data_structure_for_key(hashmap_p, key, 0);

			// if there is a binary search tree remove value from that tree
			// here keep the found_bucket_p as NULL, because if you see balancedbst implementation, it handles memory allocation for buckets on its own 
			if(ds_p != NULL)
			{
				has_been_deleted += remove_value_from_bst(((balancedbst*)(ds_p)), key, return_key, return_value);
			}
			break;
		}
	}

	// in case of NO_POLICY and ELEMENTS_AS_LINKEDLIST, hashmap is responsible for deleting, creating the bucket, hence the memory management part :p
	// the bucket has been detached from the hashmap here, so delete the bucket,
	// and return the key, value pointers to the client so he can delete the data
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

	// return the number of buckets we deleted => eeither 1 or 0
	return has_been_deleted;
}

void put_bucket_in_linkedlist_wrapper(void* bucket_p_v, const void* hashmap_p_v)
{
	bucket* bucket_p = ((bucket*)(bucket_p_v));
	hashmap* hashmap_p = ((hashmap*)(hashmap_p_v));

	// if the index is pointing to NULL linedlist, it has to be initialized first
	linkedlist* ll = (linkedlist*)get_data_structure_for_key(hashmap_p, bucket_p->key, 1);

	// insert the new bucket at its head
	insert_head(ll, bucket_p);
}

void put_entry_in_bst_wrapper(const void* key, const void* value, const void* new_hashmap_p)
{
	// insert this entry in the new hashmap
	put_entry_in_hash( ((hashmap*)(new_hashmap_p)), key, value);
}

void rehash_to_size(hashmap* hashmap_p, unsigned long long int new_bucket_size)
{
	// make a local new hashmap properties holder
	hashmap new_hashmap_properties = {.hashmap_policy = hashmap_p->hashmap_policy, .hash_function = hashmap_p->hash_function, .key_compare = hashmap_p->key_compare, .buckets_holder = get_array(new_bucket_size)};

	// iterate over all the elements in the hashmap_p
	// and add them to the newly statically maintained hashmap
	for(unsigned long long int index = 0; index < hashmap_p->buckets_holder->total_size; index++)
	{
		// get the datastructure to be printed for that index
		const void* ds_p = get_data_structure_for_index(hashmap_p, index, 0);

		if(ds_p != NULL)
		{
			switch(hashmap_p->hashmap_policy)
			{
				case NO_POLICY :
				{
					// ds_p = bucket, when No pilicy is used
					put_entry(&new_hashmap_properties, ((bucket*)(ds_p))->key, ((bucket*)(ds_p))->value);

					// delete the old bucket
					delete_bucket(((bucket*)(ds_p)));
					break;
				}
				case ELEMENTS_AS_LINKEDLIST :
				{
					// iterate over all the elements of the linkedlist and insert the bucket
					for_each_in_list(((linkedlist*)(ds_p)), put_bucket_in_linkedlist_wrapper, &new_hashmap_properties);

					// delete linkedlist
					delete_linkedlist(((linkedlist*)(ds_p)));
					break;
				}
				case ELEMENTS_AS_AVL_BST :
				case ELEMENTS_AS_RED_BLACK_BST :
				{
					// iterate over all the elements of the bst and insert all the key value entries
					for_each_entry_in_bst(((balancedbst*)(ds_p)), put_entry_in_bst_wrapper, &new_hashmap_properties);

					// delete complete old bst
					delete_balancedbst(((balancedbst*)(ds_p)));
					break;
				}
			}

			// since the datastructure is deleted, we set the pointer of linkedlist to NULL, in the array
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

		// get the datastructure to be print for that index
		const void* ds_p = get_data_structure_for_index(hashmap_p, index, 0);

		if(ds_p != NULL)
		{
			switch(hashmap_p->hashmap_policy)
			{
				case NO_POLICY :
				{
					print_bucket(((bucket*)(ds_p)), print_key, print_value);
					break;
				}
				case ELEMENTS_AS_LINKEDLIST :
				{
					for_each_in_list((((linkedlist*)(ds_p))), print_bucket_wrapper, ((const void*)(&print_functions)));
					break;
				}
				case ELEMENTS_AS_AVL_BST :
				case ELEMENTS_AS_RED_BLACK_BST :
				{
					print_balancedbst(((balancedbst*)(ds_p)), print_key, print_value);
					break;
				}
			}
		}
		else
		{
			printf("\tEMPTY\n");
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
	for(unsigned long long int index = 0; index < hashmap_p->buckets_holder->total_size; index++)
	{
		const void* ds_p = get_data_structure_for_index(hashmap_p, index, 0);
		if(ds_p != NULL)
		{
			switch(hashmap_p->hashmap_policy)
			{
				case NO_POLICY :
				{
					delete_bucket(((bucket*)(ds_p)));
					break;
				}
				case ELEMENTS_AS_LINKEDLIST :
				{
					for_each_in_list(((linkedlist*)(ds_p)), delete_bucket_wrapper, NULL);
					delete_linkedlist(((linkedlist*)(ds_p)));
					break;
				}
				case ELEMENTS_AS_AVL_BST :
				case ELEMENTS_AS_RED_BLACK_BST :
				{
					delete_balancedbst(((balancedbst*)(ds_p)));
					break;
				}
			}
			set_element(hashmap_p->buckets_holder, NULL, index);
		}
	}
	delete_array(hashmap_p->buckets_holder);
	free(hashmap_p);
}

#undef put_entry
#undef find_value
#undef remove_value
#undef for_each_entry