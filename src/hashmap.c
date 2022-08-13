#include<hashmap.h>

#include<linkedlist.h>
#include<bst.h>
#include<queue.h>

#include<dstring.h>

#include<cutlery_stds.h>

void initialize_hashmap(hashmap* hashmap_p, collision_resolution_policy hashmap_policy, unsigned int bucket_count, unsigned int (*hash_function)(const void* key), int (*compare)(const void* data1, const void* data2), unsigned int node_offset)
{
	hashmap_p->hashmap_policy = hashmap_policy;
	hashmap_p->hash_function = hash_function;
	hashmap_p->compare = compare;
	initialize_array(&(hashmap_p->hashmap_holder), bucket_count);
	hashmap_p->node_offset = node_offset;
	hashmap_p->element_count = 0;
}

void initialize_hashmap_with_allocator(hashmap* hashmap_p, collision_resolution_policy hashmap_policy, unsigned int bucket_count, unsigned int (*hash_function)(const void* key), int (*compare)(const void* data1, const void* data2), unsigned int node_offset, memory_allocator mem_allocator)
{
	hashmap_p->hashmap_policy = hashmap_policy;
	hashmap_p->hash_function = hash_function;
	hashmap_p->compare = compare;
	initialize_array_with_allocator(&(hashmap_p->hashmap_holder), bucket_count, mem_allocator);
	hashmap_p->node_offset = node_offset;
	hashmap_p->element_count = 0;
}

unsigned int get_bucket_count_hashmap(const hashmap* hashmap_p)
{
	return get_capacity_array(&(hashmap_p->hashmap_holder));
}

unsigned int get_element_count_hashmap(const hashmap* hashmap_p)
{
	return hashmap_p->element_count;
}

int is_empty_hashmap(const hashmap* hashmap_p)
{
	return (hashmap_p->element_count == 0);
}

// utility :-> gets plausible index after hashing and mod of the hash
static unsigned int get_index(const hashmap* hashmap_p, const void* data)
{
	// calculate hash
	unsigned int hash = hashmap_p->hash_function(data);

	// calculate index
	unsigned int index = hash % get_bucket_count_hashmap(hashmap_p);

	return index;
}

static int is_hashmap_with_ZERO_buckets(const hashmap* hashmap_p)
{
	return get_bucket_count_hashmap(hashmap_p) == 0;
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
			initialize_bst((bst*)ds_p, RED_BLACK_TREE, hashmap_p->compare, hashmap_p->node_offset);
			break;
		}
		case ELEMENTS_AS_AVL_BST :
		{
			initialize_bst((bst*)ds_p, AVL_TREE, hashmap_p->compare, hashmap_p->node_offset);
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
		return index_actual + get_bucket_count_hashmap(hashmap_p) - index_expected;
	}
}

// function used for ROBINHOOD_HASHING only
static unsigned int get_actual_index(const hashmap* hashmap_p, const void* data)
{
	unsigned int index = get_index(hashmap_p, data);
	unsigned int probe_sequence_length = 0;

	const void* data_at_index = NULL;

	while(probe_sequence_length < get_bucket_count_hashmap(hashmap_p))
	{
		data_at_index = get_from_array(&(hashmap_p->hashmap_holder), index);

		// bucket is empty
		// find : the data is not in the hashmap
		if(data_at_index == NULL)
			break;

		// data and the data_at_index compares equals
		// find : the data we are looking for has been found
		if(data == data_at_index || hashmap_p->compare(data, data_at_index) == 0)
			break;

		// we have a greater probe sequence length for data
		// find : this element if not found until now, can not be any further than this index
		if(probe_sequence_length > get_probe_sequence_length(hashmap_p, data_at_index, index))
			break;

		index = (index + 1) % get_bucket_count_hashmap(hashmap_p);
		probe_sequence_length++;
	}

	return index;
}

const void* find_equals_in_hashmap(const hashmap* hashmap_p, const void* data)
{
	// can not find in a hashmap that has no (0) buckets OR if it is empty
	if(is_hashmap_with_ZERO_buckets(hashmap_p) || is_empty_hashmap(hashmap_p))
		return NULL;

	switch(hashmap_p->hashmap_policy)
	{
		case ROBINHOOD_HASHING :
		{
			unsigned int index = get_actual_index(hashmap_p, data);

			const void* data_at_index = get_from_array(&(hashmap_p->hashmap_holder), index);
			
			if(data_at_index != NULL && hashmap_p->compare(data, data_at_index) == 0)
				return data_at_index;

			return NULL;
		}
		case ELEMENTS_AS_LINKEDLIST :
		{
			unsigned int index = get_index(hashmap_p, data);
			linkedlist ll; init_data_structure(hashmap_p, &ll);
			
			ll.head = (llnode*) get_from_array(&(hashmap_p->hashmap_holder), index);
			return find_equals_in_linkedlist(&ll, data, hashmap_p->compare);
		}
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			unsigned int index = get_index(hashmap_p, data);
			bst bstt; init_data_structure(hashmap_p, &bstt);
			
			bstt.root = (bstnode*) get_from_array(&(hashmap_p->hashmap_holder), index);
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

	// 0 (= inserted) elements inserted, :> can not insert anything, if the hashmap has no buckets
	if(is_hashmap_with_ZERO_buckets(hashmap_p))
		return inserted;

	switch(hashmap_p->hashmap_policy)
	{
		case ROBINHOOD_HASHING :
		{
			// if the hashmap is full
			if(hashmap_p->element_count == get_bucket_count_hashmap(hashmap_p))
				break;

			unsigned int expected_index = get_index(hashmap_p, data);

			unsigned int index = expected_index;
			unsigned int probe_sequence_length = 0;

			while(1)
			{
				const void* data_at_index = get_from_array(&(hashmap_p->hashmap_holder), index);
			
				if(data_at_index == NULL)
				{
					set_in_array(&(hashmap_p->hashmap_holder), data, index);
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
						set_in_array(&(hashmap_p->hashmap_holder), data, index);
						data = data_at_index;
						probe_sequence_length = probe_sequence_length_data_at_index;
					}

					// prepare for next iteration
					index = (index + 1) % get_bucket_count_hashmap(hashmap_p);
					probe_sequence_length++;
				}
			}

			break;
		}
		case ELEMENTS_AS_LINKEDLIST :
		{
			unsigned int index = get_index(hashmap_p, data);
			linkedlist ll; init_data_structure(hashmap_p, &ll);

			ll.head = (llnode*) get_from_array(&(hashmap_p->hashmap_holder), index);
			inserted = insert_head_in_linkedlist(&ll, data);
			set_in_array(&(hashmap_p->hashmap_holder), ll.head, index);
			break;
		}
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			unsigned int index = get_index(hashmap_p, data);
			bst bstt; init_data_structure(hashmap_p, &bstt);
			
			bstt.root = (bstnode*) get_from_array(&(hashmap_p->hashmap_holder), index);
			inserted = insert_in_bst(&bstt, data);
			set_in_array(&(hashmap_p->hashmap_holder), bstt.root, index);
			break;
		}
	}

	// increment the element_count, if inserted successfully
	hashmap_p->element_count += inserted;

	return inserted;
}

int remove_from_hashmap(hashmap* hashmap_p, const void* data)
{
	int deleted = 0;

	// 0 (= deleted) elements deleted, :> can not delete anything from the hashmap that has no buckets OR if the hashmap is empty
	if(is_hashmap_with_ZERO_buckets(hashmap_p) || is_empty_hashmap(hashmap_p))
		return deleted;

	switch(hashmap_p->hashmap_policy)
	{
		case ROBINHOOD_HASHING :
		{
			unsigned int index = get_actual_index(hashmap_p, data);

			const void* data_at_index = get_from_array(&(hashmap_p->hashmap_holder), index);
			
			// if the given element does not exist in the hashmap we can not remove it
			if(data_at_index != data)
				break;

			set_in_array(&(hashmap_p->hashmap_holder), NULL, index);
			deleted = 1;

			unsigned int previousIndex = index;
			index = (index + 1) % get_bucket_count_hashmap(hashmap_p);
			data_at_index = get_from_array(&(hashmap_p->hashmap_holder), index);
			while(data_at_index != NULL && get_probe_sequence_length(hashmap_p, data_at_index, index) != 0)
			{
				// shift null downwards, and data_at_index to previousIndex
				{
					set_in_array(&(hashmap_p->hashmap_holder), data_at_index, previousIndex);
					set_in_array(&(hashmap_p->hashmap_holder), NULL, index);
				}
				previousIndex = index;
				index = (index + 1) % get_bucket_count_hashmap(hashmap_p);
				data_at_index = get_from_array(&(hashmap_p->hashmap_holder), index);
			}

			break;
		}
		case ELEMENTS_AS_LINKEDLIST :
		{
			unsigned int index = get_index(hashmap_p, data);
			linkedlist ll; init_data_structure(hashmap_p, &ll);

			ll.head = (llnode*) get_from_array(&(hashmap_p->hashmap_holder), index);
			deleted = remove_from_linkedlist(&ll, data);
			set_in_array(&(hashmap_p->hashmap_holder), ll.head, index);
			break;
		}
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			unsigned int index = get_index(hashmap_p, data);
			bst bstt; init_data_structure(hashmap_p, &bstt);
			
			bstt.root = (bstnode*) get_from_array(&(hashmap_p->hashmap_holder), index);
			deleted = remove_from_bst(&bstt, data);
			set_in_array(&(hashmap_p->hashmap_holder), bstt.root, index);
			break;
		}
	}

	// decrement the element_count, if inserted successfully
	hashmap_p->element_count -= deleted;

	return deleted;
}

void remove_all_from_hashmap(hashmap* hashmap_p)
{
	// iterate over all the buckets in the hashmap_p and removing all the elements
	switch(hashmap_p->hashmap_policy)
	{
		case ROBINHOOD_HASHING :
		{
			// nothing needs to be done here
			break;
		}
		case ELEMENTS_AS_LINKEDLIST :
		{
			linkedlist ll; init_data_structure(hashmap_p, &ll);
			for(unsigned int index = 0; index < get_bucket_count_hashmap(hashmap_p); index++)
			{
				ll.head = (llnode*) get_from_array(&(hashmap_p->hashmap_holder), index);

				// remove all elements from the linkedlist bucket
				remove_all_from_linkedlist(&ll);
			}
			break;
		}
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			bst bstt; init_data_structure(hashmap_p, &bstt);
			for(unsigned int index = 0; index < get_bucket_count_hashmap(hashmap_p); index++)
			{
				bstt.root = (bstnode*) get_from_array(&(hashmap_p->hashmap_holder), index);

				// remove all elements from the bst bucket
				remove_all_from_bst(&bstt);
			}
			break;
		}
		default :
		{
			break;
		}
	}

	// mark all the buckets as NULL
	remove_all_from_array(&(hashmap_p->hashmap_holder));

	// then update element_count to 0
	hashmap_p->element_count = 0;
}

void deinitialize_hashmap(hashmap* hashmap_p)
{
	deinitialize_array(&(hashmap_p->hashmap_holder));
	hashmap_p->element_count = 0;
	hashmap_p->hash_function = NULL;
	hashmap_p->compare = NULL;
	hashmap_p->node_offset = 0;
}

void for_each_in_hashmap(const hashmap* hashmap_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params)
{
	// iterate over all the buckets in the hashmap_p
	switch(hashmap_p->hashmap_policy)
	{
		case ROBINHOOD_HASHING :
		{
			for(unsigned int index = 0; index < get_bucket_count_hashmap(hashmap_p); index++)
			{
				if(get_from_array(&(hashmap_p->hashmap_holder), index) != NULL)
					operation(get_from_array(&(hashmap_p->hashmap_holder), index), additional_params);
			}
			break;
		}
		case ELEMENTS_AS_LINKEDLIST :
		{
			linkedlist ll; init_data_structure(hashmap_p, &ll);
			for(unsigned int index = 0; index < get_bucket_count_hashmap(hashmap_p); index++)
			{
				ll.head = (llnode*) get_from_array(&(hashmap_p->hashmap_holder), index);
				for_each_in_linkedlist(&ll, operation, additional_params);
			}
			break;
		}
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			bst bstt; init_data_structure(hashmap_p, &bstt);
			for(unsigned int index = 0; index < get_bucket_count_hashmap(hashmap_p); index++)
			{
				bstt.root = (bstnode*) get_from_array(&(hashmap_p->hashmap_holder), index);
				for_each_in_bst(&bstt, POST_ORDER, operation, additional_params);
			}
			break;
		}
		default :
		{
			break;
		}
	}
}

// utility function used by resize_hashmap function only
static void push_to_queue_wrapper(const void* hashmap_data, const void* queue_p) {	push_to_queue((queue*)(queue_p), hashmap_data);	}

int resize_hashmap(hashmap* hashmap_p, unsigned int new_bucket_count)
{
	// if the memory allocator is NULL, we can not resize the hashmap
	if(hashmap_p->hashmap_holder.mem_allocator == NULL)
		return 0;

	// resizing to the same bucket count; i.e. nothing needs to be done
	if(get_bucket_count_hashmap(hashmap_p) == new_bucket_count)
		return 1;

	// a hashmap with non zero number of elements, can never be resized to 0 bucketed hashmap
	if((get_element_count_hashmap(hashmap_p) > 0) && (new_bucket_count == 0))
		return 0;

	// for ROBINHOOD_HASHING, the element_count must always be lesser than or equal to the bucket_count
	if(		(hashmap_p->hashmap_policy == ROBINHOOD_HASHING)
		&& (get_element_count_hashmap(hashmap_p) > new_bucket_count)	)
		return 0;

	// create a new hashmap indentical to the hashmap_p with new_bucket_count number of buckets
	hashmap new_hashmap;
	initialize_hashmap_with_allocator(&new_hashmap, hashmap_p->hashmap_policy, new_bucket_count, hashmap_p->hash_function, hashmap_p->compare, hashmap_p->node_offset, hashmap_p->hashmap_holder.mem_allocator);

	{
		// create a temporary queue variable, and push all the hashmap_p elements into it
		queue q;
		initialize_queue_with_allocator(&q, get_element_count_hashmap(hashmap_p), hashmap_p->hashmap_holder.mem_allocator);
		for_each_in_hashmap(hashmap_p, push_to_queue_wrapper, &q);

		// pop elements in the queue 1 by 1 and transfer them from one hashmap to another
		while(!is_empty_queue(&q))
		{
			const void* data = get_top_of_queue(&q);

			remove_from_hashmap(hashmap_p, data);
			insert_in_hashmap(&new_hashmap, data);

			pop_from_queue(&q);
		}

		deinitialize_queue(&q);
	}

	// deinitialize hashmap_p (old empty hashmap), and shallow copy the new_hashmap in to it
	deinitialize_hashmap(hashmap_p);
	(*hashmap_p) = new_hashmap;

	return 1;
}

int expand_hashmap(hashmap* hashmap_p, float expand_factor)
{
	// if expansion factor is lesser than 1 then it is not an expansion
	if(expand_factor <= 1.0)
		return 0;

	// calculate the new bucket count
	unsigned int new_bucket_count = expand_factor * get_bucket_count_hashmap(hashmap_p);
	// handle over flow, of unsigned int
	if(new_bucket_count < get_bucket_count_hashmap(hashmap_p))
		new_bucket_count = (~((unsigned int)(0)));

	// if new_bucket_count is not greater than the current bucket_count, then this is no longer an expansion
	if(new_bucket_count <= get_bucket_count_hashmap(hashmap_p))
		return 0;

	// call resize_hashmap and see if it succeeds
	return resize_hashmap(hashmap_p, new_bucket_count);
}

void sprint_hashmap(dstring* append_str, const hashmap* hashmap_p, void (*sprint_element)(dstring* append_str, const void* data, unsigned int tabs), unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs++);
	snprintf_dstring(append_str, "hashmap ");
	switch(hashmap_p->hashmap_policy)
	{
		case ROBINHOOD_HASHING :
		{
			snprintf_dstring(append_str, "(robinhood_hashing) :\n");
			break;
		}
		case ELEMENTS_AS_LINKEDLIST :
		{
			snprintf_dstring(append_str, "(elements_as_linkedlist) :\n");
			break;
		}
		case ELEMENTS_AS_AVL_BST :
		{
			snprintf_dstring(append_str, "(elements_as_avl_bst) :\n");
			break;
		}
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			snprintf_dstring(append_str, "(elements_as_red_black_bst) :\n");
			break;
		}
	}

	sprint_chars(append_str, '\t', tabs);
	snprintf_dstring(append_str, "node_offset : %u\n", hashmap_p->node_offset);

	sprint_chars(append_str, '\t', tabs);
	snprintf_dstring(append_str, "element_count : %u\n", hashmap_p->element_count);

	sprint_chars(append_str, '\t', tabs);
	snprintf_dstring(append_str, "bucket_count : %u\n", get_bucket_count_hashmap(hashmap_p));

	linkedlist ll; init_data_structure(hashmap_p, &ll);
	bst bstt; init_data_structure(hashmap_p, &bstt);

	// iterate over all the buckets in the hashmap_p
	for(unsigned int index = 0; index < get_bucket_count_hashmap(hashmap_p); index++)
	{
		sprint_chars(append_str, '\t', tabs + 1);
		snprintf_dstring(append_str, "bucket_id = %u\n", index);

		if(get_from_array(&(hashmap_p->hashmap_holder), index) != NULL)
		{
			switch(hashmap_p->hashmap_policy)
			{
				case ROBINHOOD_HASHING :
				{
					sprint_element(append_str, get_from_array(&(hashmap_p->hashmap_holder), index), tabs + 2);
					snprintf_dstring(append_str, "\n");
					break;
				}
				case ELEMENTS_AS_LINKEDLIST :
				{
					ll.head = (llnode*) get_from_array(&(hashmap_p->hashmap_holder), index);
					sprint_linkedlist(append_str, &ll, sprint_element, tabs + 2);
					break;
				}
				case ELEMENTS_AS_AVL_BST :
				case ELEMENTS_AS_RED_BLACK_BST :
				{
					bstt.root = (bstnode*) get_from_array(&(hashmap_p->hashmap_holder), index);
					sprint_bst(append_str, &bstt, sprint_element, tabs + 2);
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
			sprint_chars(append_str, '\t', tabs + 2);
			snprintf_dstring(append_str, "EMPTY\n");
		}
	}
}