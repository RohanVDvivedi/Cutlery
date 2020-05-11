#include<hashmap.h>

// to avoid name collision with functions of balancedbst
#define insert_entry	insert_entry_in_hash
#define find_value		find_value_from_hash
#define update_value	update_value_in_hash
#define delete_entry 	delete_entry_from_hash
#define for_each_entry 	for_each_entry_in_hash

hashmap* get_hashmap(unsigned long long int bucket_count, unsigned long long int (*hash_function)(const void* key), int (*key_compare)(const void* key1, const void* key2), collision_resolution_policy hashmap_policy)
{
	hashmap* hashmap_p = ((hashmap*) calloc(1, sizeof(hashmap)));
	hashmap_p->hashmap_policy = hashmap_policy;
	hashmap_p->hash_function = hash_function;
	hashmap_p->key_compare = key_compare;

	// initialize array of the hashmap, it is a buckets_holder
	initialize_array(&(hashmap_p->buckets_holder), bucket_count);

	hashmap_p->bucket_count = bucket_count;
	hashmap_p->bucket_occupancy = 0;
	return hashmap_p;
}

// utility :-> gets index after hashing and mod of the hash
// utility, O(1) operation
static unsigned long long int get_index_for_key(const hashmap* hashmap_p, const void* key)
{
	// calculate hash
	unsigned long long int hash = hashmap_p->hash_function(key);

	// calculate index
	unsigned long long int index = hash % hashmap_p->buckets_holder.total_size;

	return index;
}

// utility, O(1) operation
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
			case NO_POLICY :
			{
				// create a new bucket with key = NULL, value = NULL
				ds_p = get_bucket(NULL, NULL);
				break;
			}
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
		}

		// set it at the index there
		set_element((array*)(&(hashmap_p->buckets_holder)), ds_p, index);
	}

	// return the data structure at that index
	return ds_p;
}

// utility, O(1) operation
static const void* get_data_structure_for_key(const hashmap* hashmap_p, const void* key, int new_if_empty)
{
	// get index for that key
	unsigned long long int index = get_index_for_key(hashmap_p, key);

	// return the data structure at that index
	return get_data_structure_for_index(hashmap_p, index, new_if_empty);
}

static bucket* find_bucket_for_no_policy(const hashmap* hashmap_p, const void* key)
{
	// to store pointer to the bucket once it is found
	bucket* found_bucket_p = NULL;

	// get the data structure for that key, without any new creation
	const void* ds_p = get_data_structure_for_key(hashmap_p, key, 0);

	// if we found the under lying data struture, which infact handles collision, we find our bucket inside it
	if(ds_p != NULL && hashmap_p->hashmap_policy == NO_POLICY)
	{
		// if no policy ther is no middle ware data structure handling collision just bucket
		found_bucket_p = hashmap_p->key_compare(key, ((bucket*)ds_p)->key) == 0 ? ((bucket*)(ds_p)) : NULL;
	}

	// return the bucket that we found
	return found_bucket_p;
}

const void* find_value(const hashmap* hashmap_p, const void* key)
{
	switch(hashmap_p->hashmap_policy)
	{
		case NO_POLICY :
		{
			bucket* found_bucket_p = find_bucket_for_no_policy(hashmap_p, key);
			return ((found_bucket_p == NULL) ? NULL : found_bucket_p->value);
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
	}
}

void insert_entry(hashmap* hashmap_p, const void* key, const void* value)
{
	int inserted_entry = 0;

	switch(hashmap_p->hashmap_policy)
	{
		case NO_POLICY :
		{
			// retrieve data structure for that key
			void* ds_p = (void*)get_data_structure_for_key(hashmap_p, key, 1);

			// if no policy ther is no middle ware data structure handling collision just bucket, so update its key and value
			if(((bucket*)ds_p)->key == NULL)
			{
				inserted_entry = 1;
			}
			((bucket*)ds_p)->key = key;
			((bucket*)ds_p)->value = value;
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
		case NO_POLICY :
		{
			// find the bucket in the hashmap, which has the same key as this bucket
			bucket* found_bucket_p = find_bucket_for_no_policy(hashmap_p, key);

			if(found_bucket_p != NULL)
			{
				// if no policy ther is no middle ware data structure handling collision just bucket, so update its key and value
				if(return_value != NULL)
				{
					(*return_value) = found_bucket_p->value;
				}
				found_bucket_p->value = value;
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
	// this is where the bucket to be deleted gets stored
	bucket* found_bucket_p = NULL;

	// we are suppossed to return the number of entries we delete
	int has_been_deleted = 0;

	switch(hashmap_p->hashmap_policy)
	{
		case NO_POLICY :
		{
			found_bucket_p = find_bucket_for_no_policy(hashmap_p, key);
			// if no policy, ds_p is only the bucket
			if(found_bucket_p != NULL)
			{
				// set element at index in array of buckets in the hashmap to NULL
				set_element(&(hashmap_p->buckets_holder), NULL, get_index_for_key(hashmap_p, key));
				has_been_deleted = 1;

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
	// iterate over all the buckets in the hashmap_p
	for(unsigned long long int index = 0; index < hashmap_p->buckets_holder.total_size; index++)
	{
		// get the datastructure to be print for that index
		const void* ds_p = get_data_structure_for_index(hashmap_p, index, 0);

		if(ds_p != NULL)
		{
			switch(hashmap_p->hashmap_policy)
			{
				case NO_POLICY :
				{
					operation(((bucket*)(ds_p))->key, ((bucket*)(ds_p))->value, additional_params);
					break;
				}
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
			}
		}
	}
}

void print_hashmap(const hashmap* hashmap_p, void (*print_key)(const void* key), void (*print_value)(const void* value))
{
	printf("HASHMAP : ");
	switch(hashmap_p->hashmap_policy)
	{
		case NO_POLICY :
		{
			printf("NO_POLICY\n");
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
				case NO_POLICY :
				{
					print_bucket(((bucket*)(ds_p)), print_key, print_value);
					break;
				}
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
			}
		}
		else
		{
			printf("\tEMPTY\n");
		}

	}
	printf("\n\n");
}

void delete_hashmap(hashmap* hashmap_p)
{
	for(unsigned long long int index = 0; index < hashmap_p->buckets_holder.total_size; index++)
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
			set_element(&(hashmap_p->buckets_holder), NULL, index);
		}
	}
	deinitialize_array(&(hashmap_p->buckets_holder));
	free(hashmap_p);
}

#undef insert_entry
#undef find_value
#undef update_value
#undef delete_entry
#undef for_each_entry