#include<hashmap.h>

void initialize_hashmap(hashmap* hashmap_p, collision_resolution_policy hashmap_policy, unsigned long long int total_bucket_count, unsigned long long int (*hash_function)(const void* key), int (*compare)(const void* data1, const void* data2), unsigned long long int node_offset)
{
	hashmap_p->hashmap_policy = hashmap_policy;
	hashmap_p->hash_function = hash_function;
	hashmap_p->compare = compare;
	initialize_array(&(hashmap_p->holder), total_bucket_count);
	hashmap_p->node_offset = node_offset;
	hashmap_p->total_bucket_count = total_bucket_count;
	hashmap_p->occupancy = 0;
}

// utility :-> gets plausible index after hashing and mod of the hash
// utility, O(1) operation
static unsigned long long int get_index(const hashmap* hashmap_p, const void* data)
{
	// calculate hash
	unsigned long long int hash = hashmap_p->hash_function(data);

	// calculate index
	unsigned long long int index = hash % hashmap_p->total_bucket_count;

	return index;
}

// utility, O(1) operation
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
				ds_p = malloc(sizeof(linkedlist));
				initialize_linkedlist((linkedlist*)ds_p, hashmap_p->node_offset, hashmap_p->compare);
				break;
			}
			case ELEMENTS_AS_RED_BLACK_BST :
			{
				// create a new red black tree
				ds_p = malloc(sizeof(bst));
				initialize_bst((bst*)ds_p, RED_BLACK_TREE, hashmap_p->node_offset, hashmap_p->compare);
				break;
			}
			case ELEMENTS_AS_AVL_BST :
			{
				// create a new avl tree
				ds_p = malloc(sizeof(bst));
				initialize_bst((bst*)ds_p, AVL_TREE, hashmap_p->node_offset, hashmap_p->compare);
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
static const void* get_data_structure_for_data(const hashmap* hashmap_p, const void* data, int new_if_empty)
{
	// get index for data
	unsigned long long int index = get_index(hashmap_p, data);

	// return the data structure at that index
	return get_data_structure_for_index(hashmap_p, index, new_if_empty);
}

// function used for ROBINHOOD_HASHING only
static unsigned long long int get_probe_sequence_length(const hashmap* hashmap_p, const void* data, unsigned long long int index_actual)
{
	unsigned long long int index_expected = get_index(hashmap_p, data);

	if(index_actual >= index_expected)
	{
		return index_actual - index_expected;
	}
	else
	{
		return index_actual + hashmap_p->total_bucket_count - index_expected;
	}
}

// function used for ROBINHOOD_HASHING only
static unsigned long long int get_actual_index(const hashmap* hashmap_p, const void* data)
{
	unsigned long long int expected_index = get_index(hashmap_p, data);
	unsigned long long int probe_sequence_length = 0;

	const void* data_at_index = NULL;

	while(probe_sequence_length < hashmap_p->total_bucket_count)
	{
		data_at_index = get_element(&(hashmap_p->holder), expected_index);

		if(data_at_index != NULL)
		{
			if(data != data_at_index && hashmap_p->compare(data, data_at_index) != 0)
			{
				if(probe_sequence_length > get_probe_sequence_length(hashmap_p, data_at_index, expected_index))
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

		expected_index = (expected_index + 1) % hashmap_p->total_bucket_count;
		probe_sequence_length++;
	}

	return expected_index;
}

const void* find_equals_in_hashmap(const hashmap* hashmap_p, const void* data)
{
	switch(hashmap_p->hashmap_policy)
	{
		case ROBINHOOD_HASHING :
		{
			unsigned long long int index = get_actual_index(hashmap_p, data);

			const void* data_at_index = get_element(&(hashmap_p->holder), index);
			
			if(data_at_index != NULL && hashmap_p->compare(data, data_at_index) == 0)
			{
				return data_at_index;
			}

			return NULL;
		}
		case ELEMENTS_AS_LINKEDLIST :
		{
			// get the data structure for that key, without any new creation
			const void* ds_p = get_data_structure_for_data(hashmap_p, data, 0);

			// find value in a linkedlist
			return (ds_p == NULL) ? NULL : find_equals_in_list(((linkedlist*)(ds_p)), data);
		}
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			// get the data structure for that key, without any new creation
			const void* ds_p = get_data_structure_for_data(hashmap_p, data, 0);

			// find value in a bst
			return (ds_p == NULL) ? NULL : find_equals_in_bst(((bst*)(ds_p)), data);
		}
		default :
		{
			return NULL;
		}
	}
}

int insert_in_hashmap(hashmap* hashmap_p, const void* data)
{
	int inserted = 0;

	switch(hashmap_p->hashmap_policy)
	{
		case ROBINHOOD_HASHING :
		{
			// if the hashmap is full
			if(hashmap_p->occupancy == hashmap_p->total_bucket_count)
			{
				break;
			}

			unsigned long long int expected_index = get_index(hashmap_p, data);

			unsigned long long int index = expected_index;
			unsigned long long int probe_sequence_length = 0;

			while(1)
			{
				const void* data_at_index = get_element(&(hashmap_p->holder), index);
			
				if(data_at_index == NULL)
				{
					set_element(&(hashmap_p->holder), data, index);
					inserted = 1;
					break;
				}
				// inserting the same data again, that is already existing is not allowed in the hashmap
				else if(data_at_index == data)
				{
					inserted = 0;
					break;
				}
				else
				{
					unsigned long long int probe_sequence_length_data_at_index = get_probe_sequence_length(hashmap_p, data_at_index, index);
					if(probe_sequence_length > probe_sequence_length_data_at_index)
					{
						// steal the slot
						set_element(&(hashmap_p->holder), data, index);
						data = data_at_index;
						probe_sequence_length = probe_sequence_length_data_at_index;
					}
					else
					{
						index = (index + 1) % hashmap_p->total_bucket_count;
						probe_sequence_length++;
					}
				}
			}

			break;
		}
		case ELEMENTS_AS_LINKEDLIST :
		{
			// retrieve data structure for that key
			void* ds_p = (void*)get_data_structure_for_data(hashmap_p, data, 1);

			// insert the new bucket in the linkedlist
			inserted = insert_head(((linkedlist*)(ds_p)), data);
			break;
		}
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			// retrieve data structure for that key
			void* ds_p = (void*)get_data_structure_for_data(hashmap_p, data, 1);

			// insert the new bucket in the bst
			inserted = insert_in_bst(((bst*)(ds_p)), data);
			break;
		}
	}

	hashmap_p->occupancy += inserted;

	return inserted;
}

int remove_from_hashmap(hashmap* hashmap_p, const void* data)
{
	// we are suppossed to return the number of entries we delete
	int deleted = 0;

	switch(hashmap_p->hashmap_policy)
	{
		case ROBINHOOD_HASHING :
		{
			unsigned long long int index = get_actual_index(hashmap_p, data);

			const void* data_at_index = get_element(&(hashmap_p->holder), index);
			
			if(data_at_index == data)
			{
				set_element(&(hashmap_p->holder), NULL, index);
				deleted = 1;
			}
			// if the given element does not exist in the hashmap we can not remove it
			else
			{
				break;
			}

			unsigned long long int previousIndex = index;
			index = (index + 1) % hashmap_p->total_bucket_count;
			data_at_index = get_element(&(hashmap_p->holder), index);
			while(data_at_index != NULL && get_probe_sequence_length(hashmap_p, data_at_index, index) != 0)
			{
				swap_elements(&(hashmap_p->holder), previousIndex, index);
				previousIndex = index;
				index = (index + 1) % hashmap_p->total_bucket_count;
				data_at_index = get_element(&(hashmap_p->holder), index);
			}

			break;
		}
		case ELEMENTS_AS_LINKEDLIST :
		{
			// get the data structure for the given key
			const void* ds_p = get_data_structure_for_data(hashmap_p, data, 0);

			// if there is a linkedlist remove value from that tree
			// here keep the found_bucket_p as NULL, because if you see linkedlist implementation, it handles memory allocation for buckets on its own 
			deleted = ((ds_p == NULL) ? 0 : remove_from_list(((linkedlist*)(ds_p)), data));
			break;
		}
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			// get the data structure for the given key
			const void* ds_p = get_data_structure_for_data(hashmap_p, data, 0);

			// if there is a binary search tree remove value from that tree
			// here keep the found_bucket_p as NULL, because if you see balancedbst implementation, it handles memory allocation for buckets on its own 
			deleted = ((ds_p == NULL) ? 0 : remove_from_bst(((bst*)(ds_p)), data));
			break;
		}
	}

	hashmap_p->occupancy -= deleted;

	// return the number of buckets we deleted => eeither 1 or 0
	return deleted;
}

void for_each_in_hashmap(const hashmap* hashmap_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params)
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
	printf("total_bucket_count : %llu\n", hashmap_p->total_bucket_count);

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