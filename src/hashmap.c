#include<hashmap.h>

// to avoid name collision with functions of balancedbst
#define insert_entry	insert_entry_in_hash
#define find_value		find_value_from_hash
#define update_value	update_value_in_hash
#define delete_entry 	delete_entry_from_hash
#define for_each_entry 	for_each_entry_in_hash

hashmap* get_hashmap(unsigned long long int bucket_count, unsigned long long int (*hash_function)(const void* key), int (*key_compare)(const void* key1, const void* key2), collision_resolution_policy hashmap_policy)
{
	hashmap* hashmap_p = calloc(1, sizeof(hashmap));
	initialize_hashmap(hashmap_p, bucket_count, hash_function, key_compare, hashmap_policy);
	return hashmap_p;
}

void initialize_hashmap(hashmap* hashmap_p, unsigned long long int bucket_count, unsigned long long int (*hash_function)(const void* key), int (*key_compare)(const void* key1, const void* key2), collision_resolution_policy hashmap_policy)
{
	hashmap_p->hashmap_policy = hashmap_policy;
	hashmap_p->hash_function = hash_function;
	hashmap_p->key_compare = key_compare;

	// initialize array of the hashmap, it is a buckets_holder
	if(hashmap_p->hashmap_policy == ROBINHOOD_HASHING)
	{
		initialize_bucket_array(&(hashmap_p->buckets_holder), bucket_count);
	}
	else
	{
		initialize_array(&(hashmap_p->buckets_holder), bucket_count);
	}

	hashmap_p->bucket_count = bucket_count;
	hashmap_p->bucket_occupancy = 0;
}

// utility :-> gets index after hashing and mod of the hash
// utility, O(1) operation
static unsigned long long int get_index_for_key(const hashmap* hashmap_p, const void* key)
{
	// calculate hash
	unsigned long long int hash = hashmap_p->hash_function(key);

	// calculate index
	unsigned long long int index = hash % hashmap_p->bucket_count;

	return index;
}

// utility, O(1) operation
// function used for ELEMENTS_AS_LINKEDLIST, ELEMENTS_AS_RED_BLACK_BST and ELEMENTS_AS_AVL_BST only
// call the below function only if every hashmap bucket is a separate datastructure i.e. closed addressing / open hashing hashmap
static const void* get_data_structure_for_index(const hashmap* hashmap_p, unsigned long long int index, int new_if_empty)
{
	// if you try accessing hashtable, index greater than its size
	if(index >= hashmap_p->buckets_holder.total_size)
	{
		return NULL;
	}

	// get the data structure at that index
	const void* ds_p = get_element(&(hashmap_p->buckets_holder), index);

	// if the data structure is NULL
	if(ds_p == NULL && new_if_empty)
	{
		switch(hashmap_p->hashmap_policy)
		{
			case ELEMENTS_AS_LINKEDLIST :
			{
				// create a new linked list
				ds_p = get_linkedlist(BUCKETTED, hashmap_p->key_compare);
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
				// create a new avl tree
				ds_p = get_balancedbst(AVL_TREE, hashmap_p->key_compare);
				break;
			}
			default :
			{
				break;
			}
		}

		// set it at the index there
		set_element((array*)(&(hashmap_p->buckets_holder)), ds_p, index);
	}

	// return the data structure at that index
	return ds_p;
}

// utility, O(1) operation
// function used for ELEMENTS_AS_LINKEDLIST, ELEMENTS_AS_RED_BLACK_BST and ELEMENTS_AS_AVL_BST only
static const void* get_data_structure_for_key(const hashmap* hashmap_p, const void* key, int new_if_empty)
{
	// get index for that key
	unsigned long long int index = get_index_for_key(hashmap_p, key);

	// return the data structure at that index
	return get_data_structure_for_index(hashmap_p, index, new_if_empty);
}

// function used for ROBINHOOD_HASHING only
static unsigned long long int get_probe_sequence_length(const hashmap* hashmap_p, const void* key, unsigned long long int index_actual)
{
	unsigned long long int index_expected = get_index_for_key(hashmap_p, key);

	if(index_actual >= index_expected)
	{
		return index_actual - index_expected;
	}
	else
	{
		return index_actual + hashmap_p->bucket_count - index_expected;
	}
}

// function used for ROBINHOOD_HASHING only
static unsigned long long int get_expected_index(const hashmap* hashmap_p, const void* key)
{
	unsigned long long int index = get_index_for_key(hashmap_p, key);
	unsigned long long int probe_sequence_length = 0;

	const void* key_at_index = NULL;

	while(probe_sequence_length < hashmap_p->bucket_count)
	{
		key_at_index = get_key_bucket_array(&(hashmap_p->buckets_holder), index);

		if(key_at_index != NULL)
		{
			if(hashmap_p->key_compare(key, key_at_index) != 0)
			{
				if(probe_sequence_length > get_probe_sequence_length(hashmap_p, key_at_index, index))
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}

		index = (index + 1) % hashmap_p->bucket_count;
		probe_sequence_length++;
	}

	return index;
}

const void* find_value(const hashmap* hashmap_p, const void* key)
{
	switch(hashmap_p->hashmap_policy)
	{
		case ROBINHOOD_HASHING :
		{
			unsigned long long int index = get_expected_index(hashmap_p, key);

			const void* key_at_index = get_key_bucket_array(&(hashmap_p->buckets_holder), index);
			
			if(key_at_index != NULL && hashmap_p->key_compare(key, key_at_index) == 0)
			{
				return get_value_bucket_array(&(hashmap_p->buckets_holder), index);
			}

			return NULL;
		}
		case ELEMENTS_AS_LINKEDLIST :
		{
			// get the data structure for that key, without any new creation
			const void* ds_p = get_data_structure_for_key(hashmap_p, key, 0);

			// find value in a linkedlist
			return ds_p == NULL ? NULL : find_value_from_ll(((linkedlist*)(ds_p)), key);
		}
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			// get the data structure for that key, without any new creation
			const void* ds_p = get_data_structure_for_key(hashmap_p, key, 0);

			// find value in a bst
			return ds_p == NULL ? NULL : find_value_from_bst(((balancedbst*)(ds_p)), key);
		}
		default :
		{
			return NULL;
		}
	}
}

void insert_entry(hashmap* hashmap_p, const void* key, const void* value)
{
	int inserted_entry = 0;

	switch(hashmap_p->hashmap_policy)
	{
		case ROBINHOOD_HASHING :
		{
			if(hashmap_p->bucket_occupancy >= hashmap_p->bucket_count)
			{
				inserted_entry = 1;
				break;
			}

			while(1)
			{
				unsigned long long int index = get_expected_index(hashmap_p, key);

				const void* key_at_index = get_key_bucket_array(&(hashmap_p->buckets_holder), index);
			
				if(key_at_index == NULL)
				{
					insert_in_bucket_array(&(hashmap_p->buckets_holder), key, value, index);
					inserted_entry = 1;
					break;
				}
				else
				{
					// steal the slot
					const void* value_at_index = get_value_bucket_array(&(hashmap_p->buckets_holder), index);
					insert_in_bucket_array(&(hashmap_p->buckets_holder), key, value, index);
					key = key_at_index;
					value = value_at_index;
				}
			}

			break;
		}
		case ELEMENTS_AS_LINKEDLIST :
		{
			// retrieve data structure for that key
			void* ds_p = (void*)get_data_structure_for_key(hashmap_p, key, 1);

			// insert the new bucket in the linkedlist
			insert_entry_in_ll(((linkedlist*)(ds_p)), key, value);
			
			inserted_entry = 1;
			break;
		}
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			// retrieve data structure for that key
			void* ds_p = (void*)get_data_structure_for_key(hashmap_p, key, 1);

			// insert the new bucket in the bst
			insert_entry_in_bst(((balancedbst*)(ds_p)), key, value);

			inserted_entry = 1;
			break;
		}
	}

	hashmap_p->bucket_occupancy += inserted_entry;
}

int update_value(hashmap* hashmap_p, const void* key, const void* value, const void** return_value)
{
	int update_made = 0;

	switch(hashmap_p->hashmap_policy)
	{
		case ROBINHOOD_HASHING :
		{
			unsigned long long int index = get_expected_index(hashmap_p, key);

			const void* key_at_index = get_key_bucket_array(&(hashmap_p->buckets_holder), index);
			
			if(key_at_index != NULL && hashmap_p->key_compare(key, key_at_index) == 0)
			{
				const void* value_at_index = get_value_bucket_array(&(hashmap_p->buckets_holder), index);

				if(return_value != NULL)
				{
					(*(return_value)) = value_at_index;
				}

				insert_in_bucket_array(&(hashmap_p->buckets_holder), key_at_index, value, index);

				update_made = 1;
			}

			break;
		}
		case ELEMENTS_AS_LINKEDLIST :
		{
			// retrieve data structure for that key
			void* ds_p = (void*)get_data_structure_for_key(hashmap_p, key, 0);

			// insert the new bucket in the linkedlist
			update_made = ((ds_p == NULL) ? 0 : update_value_in_ll(((linkedlist*)(ds_p)), key, value, return_value));
			break;
		}
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			// retrieve data structure for that key
			void* ds_p = (void*)get_data_structure_for_key(hashmap_p, key, 0);

			// insert the new bucket in the bst
			update_made = ((ds_p == NULL) ? 0 : update_value_in_bst(((balancedbst*)(ds_p)), key, value, return_value));
			break;
		}
	}

	return update_made;
}

int delete_entry(hashmap* hashmap_p, const void* key, const void** return_key, const void** return_value)
{
	// we are suppossed to return the number of entries we delete
	int has_been_deleted = 0;

	switch(hashmap_p->hashmap_policy)
	{
		case ROBINHOOD_HASHING :
		{
			unsigned long long int index = get_expected_index(hashmap_p, key);

			const void* key_at_index = get_key_bucket_array(&(hashmap_p->buckets_holder), index);
			
			if(key_at_index != NULL && hashmap_p->key_compare(key, key_at_index) == 0)
			{
				const void* value_at_index = get_value_bucket_array(&(hashmap_p->buckets_holder), index);

				if(return_key != NULL)
				{
					(*(return_key)) = key_at_index;
				}
				if(return_value != NULL)
				{
					(*(return_value)) = value_at_index;
				}

				insert_in_bucket_array(&(hashmap_p->buckets_holder), NULL, NULL, index);

				has_been_deleted = 1;
			}

			unsigned long long int previousIndex = index;
			index = (index + 1) % hashmap_p->bucket_count;
			key_at_index = get_key_bucket_array(&(hashmap_p->buckets_holder), index);
			while(key_at_index != NULL && get_probe_sequence_length(hashmap_p, key_at_index, index) != 0)
			{
				swap_buckets_bucket_array(&(hashmap_p->buckets_holder), previousIndex, index);
				previousIndex = index;
				index = (index + 1) % hashmap_p->bucket_count;
				key_at_index = get_key_bucket_array(&(hashmap_p->buckets_holder), index);
			}

			break;
		}
		case ELEMENTS_AS_LINKEDLIST :
		{
			// get the data structure for the given key
			const void* ds_p = get_data_structure_for_key(hashmap_p, key, 0);

			// if there is a linkedlist remove value from that tree
			// here keep the found_bucket_p as NULL, because if you see linkedlist implementation, it handles memory allocation for buckets on its own 
			has_been_deleted = ((ds_p == NULL) ? 0 : delete_entry_from_ll(((linkedlist*)(ds_p)), key, return_key, return_value));
			break;
		}
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			// get the data structure for the given key
			const void* ds_p = get_data_structure_for_key(hashmap_p, key, 0);

			// if there is a binary search tree remove value from that tree
			// here keep the found_bucket_p as NULL, because if you see balancedbst implementation, it handles memory allocation for buckets on its own 
			has_been_deleted = ((ds_p == NULL) ? 0 : delete_entry_from_bst(((balancedbst*)(ds_p)), key, return_key, return_value));
			break;
		}
	}

	if(has_been_deleted)
	{
		hashmap_p->bucket_occupancy--;
	}

	// return the number of buckets we deleted => eeither 1 or 0
	return has_been_deleted;
}

void for_each_entry(const hashmap* hashmap_p, void (*operation)(const void* key, const void* value, const void* additional_params), const void* additional_params)
{
	if(hashmap_p->hashmap_policy == ROBINHOOD_HASHING)
	{
		for_each_entry_in_bucket_array(&(hashmap_p->buckets_holder), operation, additional_params);
		return;
	}

	// iterate over all the buckets in the hashmap_p
	for(unsigned long long int index = 0; index < hashmap_p->buckets_holder.total_size; index++)
	{
		// get the datastructure to be print for that index
		const void* ds_p = get_data_structure_for_index(hashmap_p, index, 0);

		if(ds_p != NULL)
		{
			switch(hashmap_p->hashmap_policy)
			{
				case ELEMENTS_AS_LINKEDLIST :
				{
					for_each_entry_in_ll(((linkedlist*)(ds_p)), operation, additional_params);
					break;
				}
				case ELEMENTS_AS_AVL_BST :
				case ELEMENTS_AS_RED_BLACK_BST :
				{
					for_each_entry_in_bst(((balancedbst*)(ds_p)), operation, additional_params);
					break;
				}
				default :
				{
					break;
				}
			}
		}
	}
}

void print_hashmap(const hashmap* hashmap_p, void (*print_key)(const void* key), void (*print_value)(const void* value))
{
	printf("HASHMAP : ");
	switch(hashmap_p->hashmap_policy)
	{
		case ROBINHOOD_HASHING :
		{
			printf("ROBINHOOD_HASHING\n");
			break;
		}
		case ELEMENTS_AS_LINKEDLIST :
		{
			printf("ELEMENTS_AS_LINKEDLIST\n");
			break;
		}
		case ELEMENTS_AS_AVL_BST :
		{
			printf("ELEMENTS_AS_AVL_BST\n");
			break;
		}
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			printf("ELEMENTS_AS_RED_BLACK_BST\n");
			break;
		}
	}
	printf("bucket_occupancy : %llu\n", hashmap_p->bucket_occupancy);
	printf("bucket_count : %llu\n", hashmap_p->bucket_count);

	if(hashmap_p->hashmap_policy == ROBINHOOD_HASHING)
	{
		print_bucket_array(&(hashmap_p->buckets_holder), print_key, print_value);
	}
	else
	{
		// iterate over all the buckets in the hashmap_p
		for(unsigned long long int index = 0; index < hashmap_p->buckets_holder.total_size; index++)
		{
			printf("index = %lld\n", index);

			// get the datastructure to be print for that index
			const void* ds_p = get_data_structure_for_index(hashmap_p, index, 0);

			if(ds_p != NULL)
			{
				switch(hashmap_p->hashmap_policy)
				{
					case ELEMENTS_AS_LINKEDLIST :
					{
						print_linkedlist_bucketted(((linkedlist*)(ds_p)), print_key, print_value);
						break;
					}
					case ELEMENTS_AS_AVL_BST :
					case ELEMENTS_AS_RED_BLACK_BST :
					{
						print_balancedbst(((balancedbst*)(ds_p)), print_key, print_value);
						break;
					}
					default :
					{
						break;
					}
				}
			}
			else
			{
				printf("\tEMPTY\n");
			}

		}
	}
	printf("\n\n");
}

void deinitialize_hashmap(hashmap* hashmap_p)
{
	if(hashmap_p->hashmap_policy != ROBINHOOD_HASHING)
	{
		for(unsigned long long int index = 0; index < hashmap_p->buckets_holder.total_size; index++)
		{
			const void* ds_p = get_data_structure_for_index(hashmap_p, index, 0);
			if(ds_p != NULL)
			{
				switch(hashmap_p->hashmap_policy)
				{
					case ELEMENTS_AS_LINKEDLIST :
					{
						delete_linkedlist(((linkedlist*)(ds_p)));
						break;
					}
					case ELEMENTS_AS_AVL_BST :
					case ELEMENTS_AS_RED_BLACK_BST :
					{
						delete_balancedbst(((balancedbst*)(ds_p)));
						break;
					}
					default :
					{
						break;
					}
				}
				set_element(&(hashmap_p->buckets_holder), NULL, index);
			}
		}
	}
	deinitialize_array(&(hashmap_p->buckets_holder));
}

void delete_hashmap(hashmap* hashmap_p)
{
	deinitialize_hashmap(hashmap_p);
	free(hashmap_p);
}

#undef insert_entry
#undef find_value
#undef update_value
#undef delete_entry
#undef for_each_entry