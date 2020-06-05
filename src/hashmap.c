#include<hashmap.h>

void initialize_hashmap(hashmap* hashmap_p, collision_resolution_policy hashmap_policy, unsigned long long int bucket_count, unsigned long long int (*hash_function)(const void* key), int (*compare)(const void* data1, const void* data2), unsigned long long int node_offset)
{
	hashmap_p->hashmap_policy = hashmap_policy;
	hashmap_p->hash_function = hash_function;
	hashmap_p->compare = compare;
	initialize_array(&(hashmap_p->holder), bucket_count);
	hashmap_p->node_offset = node_offset;
	hashmap_p->element_count = bucket_count;
	hashmap_p->occupancy = 0;
}

// utility :-> gets index after hashing and mod of the hash
// utility, O(1) operation
static unsigned long long int get_index_for_key(const hashmap* hashmap_p, const void* key)
{
	// calculate hash
	unsigned long long int hash = hashmap_p->hash_function(key);

	// calculate index
	unsigned long long int index = hash % hashmap_p->element_count;

	return index;
}

// utility, O(1) operation
// function used for ELEMENTS_AS_LINKEDLIST, ELEMENTS_AS_RED_BLACK_BST and ELEMENTS_AS_AVL_BST only
// call the below function only if every hashmap bucket is a separate datastructure i.e. closed addressing / open hashing hashmap
static const void* get_data_structure_for_index(const hashmap* hashmap_p, unsigned long long int index, int new_if_empty)
{
	// if you try accessing hashtable, index greater than its size
	if(index >= hashmap_p->holder.total_size)
	{
		return NULL;
	}

	// get the data structure at that index
	const void* ds_p = get_element(&(hashmap_p->holder), index);

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
		set_element((array*)(&(hashmap_p->holder)), ds_p, index);
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
		return index_actual + hashmap_p->element_count - index_expected;
	}
}

// function used for ROBINHOOD_HASHING only
static unsigned long long int get_expected_index(const hashmap* hashmap_p, const void* key)
{
	unsigned long long int index = get_index_for_key(hashmap_p, key);
	unsigned long long int probe_sequence_length = 0;

	const void* key_at_index = NULL;

	while(probe_sequence_length < hashmap_p->element_count)
	{
		key_at_index = get_key_bucket_array(&(hashmap_p->holder), index);

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

		index = (index + 1) % hashmap_p->element_count;
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

			const void* key_at_index = get_key_bucket_array(&(hashmap_p->holder), index);
			
			if(key_at_index != NULL && hashmap_p->key_compare(key, key_at_index) == 0)
			{
				return get_value_bucket_array(&(hashmap_p->holder), index);
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
			return ds_p == NULL ? NULL : find_value_from_bst(((bst*)(ds_p)), key);
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
			if(hashmap_p->occupancy >= hashmap_p->element_count)
			{
				inserted_entry = 1;
				break;
			}

			while(1)
			{
				unsigned long long int index = get_expected_index(hashmap_p, key);

				const void* key_at_index = get_key_bucket_array(&(hashmap_p->holder), index);
			
				if(key_at_index == NULL)
				{
					insert_in_bucket_array(&(hashmap_p->holder), key, value, index);
					inserted_entry = 1;
					break;
				}
				else
				{
					// steal the slot
					const void* value_at_index = get_value_bucket_array(&(hashmap_p->holder), index);
					insert_in_bucket_array(&(hashmap_p->holder), key, value, index);
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
			insert_entry_in_bst(((bst*)(ds_p)), key, value);

			inserted_entry = 1;
			break;
		}
	}

	hashmap_p->occupancy += inserted_entry;
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

			const void* key_at_index = get_key_bucket_array(&(hashmap_p->holder), index);
			
			if(key_at_index != NULL && hashmap_p->key_compare(key, key_at_index) == 0)
			{
				const void* value_at_index = get_value_bucket_array(&(hashmap_p->holder), index);

				if(return_key != NULL)
				{
					(*(return_key)) = key_at_index;
				}
				if(return_value != NULL)
				{
					(*(return_value)) = value_at_index;
				}

				insert_in_bucket_array(&(hashmap_p->holder), NULL, NULL, index);

				has_been_deleted = 1;
			}

			unsigned long long int previousIndex = index;
			index = (index + 1) % hashmap_p->element_count;
			key_at_index = get_key_bucket_array(&(hashmap_p->holder), index);
			while(key_at_index != NULL && get_probe_sequence_length(hashmap_p, key_at_index, index) != 0)
			{
				swap_buckets_bucket_array(&(hashmap_p->holder), previousIndex, index);
				previousIndex = index;
				index = (index + 1) % hashmap_p->element_count;
				key_at_index = get_key_bucket_array(&(hashmap_p->holder), index);
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
			has_been_deleted = ((ds_p == NULL) ? 0 : delete_entry_from_bst(((bst*)(ds_p)), key, return_key, return_value));
			break;
		}
	}

	if(has_been_deleted)
	{
		hashmap_p->occupancy--;
	}

	// return the number of buckets we deleted => eeither 1 or 0
	return has_been_deleted;
}

void for_each_entry(const hashmap* hashmap_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params)
{
	// iterate over all the buckets in the hashmap_p
	for(unsigned long long int index = 0; index < hashmap_p->holder.total_size; index++)
	{
		// get the datastructure to be print for that index
		const void* ds_p = get_data_structure_for_index(hashmap_p, index, 0);

		if(ds_p != NULL)
		{
			switch(hashmap_p->hashmap_policy)
			{
				case ROBINHOOD_HASHING :
				{
					operation(ds_p, additional_params);
					break;
				}
				case ELEMENTS_AS_LINKEDLIST :
				{
					for_each_in_list(((linkedlist*)(ds_p)), operation, additional_params);
					break;
				}
				case ELEMENTS_AS_AVL_BST :
				case ELEMENTS_AS_RED_BLACK_BST :
				{
					for_each_in_bst(((bst*)(ds_p)), operation, additional_params);
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

void print_hashmap(const hashmap* hashmap_p, void (*print_element)(const void* data))
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
	printf("node_offset : %llu\n", hashmap_p->node_offset);
	printf("occupancy : %llu\n", hashmap_p->occupancy);
	printf("element_count : %llu\n", hashmap_p->element_count);

	if(hashmap_p->hashmap_policy == ROBINHOOD_HASHING)
	{
		print_array(&(hashmap_p->holder), print_element);
	}
	else
	{
		// iterate over all the buckets in the hashmap_p
		for(unsigned long long int index = 0; index < hashmap_p->holder.total_size; index++)
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
						print_linkedlist(((linkedlist*)(ds_p)), print_element);
						break;
					}
					case ELEMENTS_AS_AVL_BST :
					case ELEMENTS_AS_RED_BLACK_BST :
					{
						print_bst(((bst*)(ds_p)), print_element);
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
		for(unsigned long long int index = 0; index < hashmap_p->holder.total_size; index++)
		{
			void* ds_p = (void*) get_data_structure_for_index(hashmap_p, index, 0);
			if(ds_p != NULL)
			{
				free(ds_p);
				set_element(&(hashmap_p->holder), NULL, index);
			}
		}
	}
	deinitialize_array(&(hashmap_p->holder));
}