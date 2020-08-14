#include<hashmap.h>

#include<linkedlist.h>
#include<bst.h>

void initialize_hashmap(hashmap* hashmap_p, collision_resolution_policy hashmap_policy, unsigned int total_bucket_count, unsigned int (*hash_function)(const void* key), int (*compare)(const void* data1, const void* data2), unsigned int node_offset)
{
	hashmap_p->hashmap_policy = hashmap_policy;
	hashmap_p->hash_function = hash_function;
	hashmap_p->compare = compare;
	hashmap_p->holder = calloc(total_bucket_count, sizeof(void*));
	hashmap_p->node_offset = node_offset;
	hashmap_p->total_bucket_count = total_bucket_count;
	hashmap_p->occupancy = 0;
}

// utility :-> gets plausible index after hashing and mod of the hash
// utility, O(1) operation
static unsigned int get_index(const hashmap* hashmap_p, const void* data)
{
	// calculate hash
	unsigned int hash = hashmap_p->hash_function(data);

	// calculate index
	unsigned int index = hash % hashmap_p->total_bucket_count;

	return index;
}

static void init_data_structure(const hashmap* hashmap_p, void* ds_p)
{
	switch(hashmap_p->hashmap_policy)
	{
		case ELEMENTS_AS_LINKEDLIST :
		{
			initialize_linkedlist((linkedlist*)ds_p, hashmap_p->node_offset);
			break;
		}
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			initialize_bst((bst*)ds_p, RED_BLACK_TREE, hashmap_p->node_offset, hashmap_p->compare);
			break;
		}
		case ELEMENTS_AS_AVL_BST :
		{
			initialize_bst((bst*)ds_p, AVL_TREE, hashmap_p->node_offset, hashmap_p->compare);
			break;
		}
		default :
		{
			break;
		}
	}
}

// function used for ROBINHOOD_HASHING only
static unsigned int get_probe_sequence_length(const hashmap* hashmap_p, const void* data, unsigned int index_actual)
{
	unsigned int index_expected = get_index(hashmap_p, data);

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
static unsigned int get_actual_index(const hashmap* hashmap_p, const void* data)
{
	unsigned int expected_index = get_index(hashmap_p, data);
	unsigned int probe_sequence_length = 0;

	const void* data_at_index = NULL;

	while(probe_sequence_length < hashmap_p->total_bucket_count)
	{
		data_at_index = hashmap_p->holder[expected_index];

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
			unsigned int index = get_actual_index(hashmap_p, data);

			const void* data_at_index = hashmap_p->holder[index];
			
			if(data_at_index != NULL && hashmap_p->compare(data, data_at_index) == 0)
			{
				return data_at_index;
			}

			return NULL;
		}
		case ELEMENTS_AS_LINKEDLIST :
		{
			unsigned int index = get_index(hashmap_p, data);
			linkedlist ll; init_data_structure(hashmap_p, &ll);
			
			ll.head = hashmap_p->holder[index];
			return find_equals_in_list(&ll, data, hashmap_p->compare);
		}
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			unsigned int index = get_index(hashmap_p, data);
			bst bstt; init_data_structure(hashmap_p, &bstt);
			
			bstt.root = hashmap_p->holder[index];
			return find_equals_in_bst(&bstt, data);
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

			unsigned int expected_index = get_index(hashmap_p, data);

			unsigned int index = expected_index;
			unsigned int probe_sequence_length = 0;

			while(1)
			{
				void* data_at_index = hashmap_p->holder[index];
			
				if(data_at_index == NULL)
				{
					hashmap_p->holder[index] = (void*) data;
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
					unsigned int probe_sequence_length_data_at_index = get_probe_sequence_length(hashmap_p, data_at_index, index);
					if(probe_sequence_length > probe_sequence_length_data_at_index)
					{
						// steal the slot
						hashmap_p->holder[index] = (void*)data;
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
			unsigned int index = get_index(hashmap_p, data);
			linkedlist ll; init_data_structure(hashmap_p, &ll);

			ll.head = hashmap_p->holder[index];
			inserted = insert_head(&ll, data);
			hashmap_p->holder[index] = ll.head;
			break;
		}
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			unsigned int index = get_index(hashmap_p, data);
			bst bstt; init_data_structure(hashmap_p, &bstt);
			
			bstt.root = hashmap_p->holder[index];
			inserted = insert_in_bst(&bstt, data);
			hashmap_p->holder[index] = bstt.root;
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
			unsigned int index = get_actual_index(hashmap_p, data);

			void* data_at_index = hashmap_p->holder[index];
			
			if(data_at_index == data)
			{
				hashmap_p->holder[index] = NULL;
				deleted = 1;
			}
			// if the given element does not exist in the hashmap we can not remove it
			else
			{
				break;
			}

			unsigned int previousIndex = index;
			index = (index + 1) % hashmap_p->total_bucket_count;
			data_at_index = hashmap_p->holder[index];
			while(data_at_index != NULL && get_probe_sequence_length(hashmap_p, data_at_index, index) != 0)
			{
				// shift null downwards
				{
					hashmap_p->holder[previousIndex] = hashmap_p->holder[index];
					hashmap_p->holder[index] = NULL;
				}
				previousIndex = index;
				index = (index + 1) % hashmap_p->total_bucket_count;
				data_at_index = hashmap_p->holder[index];
			}

			break;
		}
		case ELEMENTS_AS_LINKEDLIST :
		{
			unsigned int index = get_index(hashmap_p, data);
			linkedlist ll; init_data_structure(hashmap_p, &ll);

			ll.head = hashmap_p->holder[index];
			deleted = remove_from_list(&ll, data);
			hashmap_p->holder[index] = ll.head;
			break;
		}
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			unsigned int index = get_index(hashmap_p, data);
			bst bstt; init_data_structure(hashmap_p, &bstt);
			
			bstt.root = hashmap_p->holder[index];
			deleted = remove_from_bst(&bstt, data);
			hashmap_p->holder[index] = bstt.root;
			break;
		}
	}

	hashmap_p->occupancy -= deleted;

	// return the number of buckets we deleted => eeither 1 or 0
	return deleted;
}

void for_each_in_hashmap(const hashmap* hashmap_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params)
{
	linkedlist ll; init_data_structure(hashmap_p, &ll);
	bst bstt; init_data_structure(hashmap_p, &bstt);

	// iterate over all the buckets in the hashmap_p
	for(unsigned int index = 0; index < hashmap_p->total_bucket_count; index++)
	{
		if(hashmap_p->holder[index] != NULL)
		{
			switch(hashmap_p->hashmap_policy)
			{
				case ROBINHOOD_HASHING :
				{
					operation(hashmap_p->holder[index], additional_params);
					break;
				}
				case ELEMENTS_AS_LINKEDLIST :
				{
					ll.head = hashmap_p->holder[index];
					for_each_in_list(&ll, operation, additional_params);
					break;
				}
				case ELEMENTS_AS_AVL_BST :
				case ELEMENTS_AS_RED_BLACK_BST :
				{
					bstt.root = hashmap_p->holder[index];
					for_each_in_bst(&bstt, POST_ORDER, operation, additional_params);
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
	printf("node_offset : %u\n", hashmap_p->node_offset);
	printf("occupancy : %u\n", hashmap_p->occupancy);
	printf("total_bucket_count : %u\n", hashmap_p->total_bucket_count);

	linkedlist ll; init_data_structure(hashmap_p, &ll);
	bst bstt; init_data_structure(hashmap_p, &bstt);

	// iterate over all the buckets in the hashmap_p
	for(unsigned int index = 0; index < hashmap_p->total_bucket_count; index++)
	{
		printf("index = %u\n", index);

		if(hashmap_p->holder[index] != NULL)
		{
			switch(hashmap_p->hashmap_policy)
			{
				case ROBINHOOD_HASHING :
				{
					printf("  \t -> ");print_element(hashmap_p->holder[index]);printf("\n");
					break;
				}
				case ELEMENTS_AS_LINKEDLIST :
				{
					ll.head = hashmap_p->holder[index];
					print_linkedlist(&ll, print_element);
					break;
				}
				case ELEMENTS_AS_AVL_BST :
				case ELEMENTS_AS_RED_BLACK_BST :
				{
					bstt.root = hashmap_p->holder[index];
					print_bst(&bstt, print_element);
					break;
				}
				default :
				{
					break;
				}
			}
		}
		else
			printf("\tEMPTY\n");
	}
	printf("\n\n");
}

void deinitialize_hashmap(hashmap* hashmap_p)
{
	free(hashmap_p->holder);
}