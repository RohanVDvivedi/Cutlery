#include<hashmap.h>

#include<linkedlist.h>
#include<bst.h>
#include<circular_buffer_array_util.h>

#include<dstring.h>

#include<cutlery_node.h>
#include<cutlery_stds.h>

int initialize_hashmap(hashmap* hashmap_p, collision_resolution_policy hashmap_policy, cy_uint bucket_count, const hasher_interface* hasher, const comparator_interface* comparator, cy_uint node_offset)
{
	hashmap_p->hashmap_policy = hashmap_policy;
	hashmap_p->hasher = (*hasher);
	hashmap_p->comparator = (*comparator);
	hashmap_p->node_offset = node_offset;
	hashmap_p->element_count = 0;
	return initialize_array(&(hashmap_p->hashmap_holder), bucket_count);
}

int initialize_hashmap_with_allocator(hashmap* hashmap_p, collision_resolution_policy hashmap_policy, cy_uint bucket_count, const hasher_interface* hasher, const comparator_interface* comparator, cy_uint node_offset, memory_allocator mem_allocator)
{
	hashmap_p->hashmap_policy = hashmap_policy;
	hashmap_p->hasher = (*hasher);
	hashmap_p->comparator = (*comparator);
	hashmap_p->node_offset = node_offset;
	hashmap_p->element_count = 0;
	return initialize_array_with_allocator(&(hashmap_p->hashmap_holder), bucket_count, mem_allocator);
}

int initialize_hashmap_with_memory(hashmap* hashmap_p, collision_resolution_policy hashmap_policy, cy_uint bucket_count, const hasher_interface* hasher, const comparator_interface* comparator, cy_uint node_offset, const void* bucket_memory[])
{
	hashmap_p->hashmap_policy = hashmap_policy;
	hashmap_p->hasher = (*hasher);
	hashmap_p->comparator = (*comparator);
	hashmap_p->node_offset = node_offset;
	hashmap_p->element_count = 0;
	return initialize_array_with_memory(&(hashmap_p->hashmap_holder), bucket_count, bucket_memory);
}

void initialize_rbhnode(rbhnode* node_p)
{
	node_p->position_index = INVALID_INDEX;
}

int is_free_floating_rbhnode(const rbhnode* node_p)
{
	return node_p->position_index == INVALID_INDEX;
}

cy_uint get_bucket_count_hashmap(const hashmap* hashmap_p)
{
	return get_capacity_array(&(hashmap_p->hashmap_holder));
}

cy_uint get_element_count_hashmap(const hashmap* hashmap_p)
{
	return hashmap_p->element_count;
}

int is_empty_hashmap(const hashmap* hashmap_p)
{
	return (hashmap_p->element_count == 0);
}

// utility :-> gets plausible index after hashing and mod of the hash
static cy_uint get_bucket_index(const hashmap* hashmap_p, const void* data)
{
	return hash_with_hasher(&(hashmap_p->hasher), data) % get_bucket_count_hashmap(hashmap_p);
}

static int is_hashmap_with_ZERO_buckets(const hashmap* hashmap_p)
{
	return get_bucket_count_hashmap(hashmap_p) == 0;
}

static void init_data_structure(const hashmap* hashmap_p, void* ds_p)
{
	switch(hashmap_p->hashmap_policy)
	{
		case ELEMENTS_AS_LINKEDLIST_INSERT_AT_HEAD :
		case ELEMENTS_AS_LINKEDLIST_INSERT_AT_TAIL :
		{
			initialize_linkedlist((linkedlist*)ds_p, hashmap_p->node_offset);
			break;
		}
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			initialize_bst((bst*)ds_p, RED_BLACK_TREE, &(hashmap_p->comparator), hashmap_p->node_offset);
			break;
		}
		case ELEMENTS_AS_AVL_BST :
		{
			initialize_bst((bst*)ds_p, AVL_TREE, &(hashmap_p->comparator), hashmap_p->node_offset);
			break;
		}
		default :
		{
			break;
		}
	}
}

// function used for ROBINHOOD_HASHING only
static cy_uint get_probe_sequence_length(const hashmap* hashmap_p, const void* data)
{
	cy_uint bucket_index = get_bucket_index(hashmap_p, data);
	cy_uint position_index = ((rbhnode*)get_node(data, hashmap_p))->position_index;

	if(position_index >= bucket_index)
		return position_index - bucket_index;
	else
		return position_index + get_bucket_count_hashmap(hashmap_p) - bucket_index;
}

const void* find_equals_in_hashmap(const hashmap* hashmap_p, const void* data)
{
	// can not find in a hashmap that has no (0) buckets OR if it is empty
	if(is_hashmap_with_ZERO_buckets(hashmap_p) || is_empty_hashmap(hashmap_p))
		return NULL;

	// evaluate the index of the bucket that this element will go into
	// all elements that compare equal, must hash to the same bucket
	cy_uint bucket_index = get_bucket_index(hashmap_p, data);

	switch(hashmap_p->hashmap_policy)
	{
		case ROBINHOOD_HASHING :
		{
			cy_uint position_index = bucket_index;
			cy_uint probe_sequence_length = 0;

			const void* data_found = NULL;

			while(probe_sequence_length < get_bucket_count_hashmap(hashmap_p))
			{
				const void* data_at_position_index = get_from_array(&(hashmap_p->hashmap_holder), position_index);

				// bucket is empty OR data_at_position_index is at a lesser probe sequence length :: then quit the loop
				if(data_at_position_index == NULL || get_probe_sequence_length(hashmap_p, data_at_position_index) < probe_sequence_length)
					break;

				// data and the data_at_position_index compares equals :: the data we are looking for has been found
				if(compare_with_comparator(&(hashmap_p->comparator), data, data_at_position_index) == 0)
				{
					data_found = data_at_position_index;
					break;
				}

				position_index = get_circular_next(position_index, get_bucket_count_hashmap(hashmap_p)); // equivalent to ==> (position_index + 1) % bucket_count
				probe_sequence_length++;
			}

			return data_found;
		}
		case ELEMENTS_AS_LINKEDLIST_INSERT_AT_HEAD :
		case ELEMENTS_AS_LINKEDLIST_INSERT_AT_TAIL :
		{
			linkedlist ll; init_data_structure(hashmap_p, &ll);
			
			ll.head = (llnode*) get_from_array(&(hashmap_p->hashmap_holder), bucket_index);
			return find_equals_in_linkedlist(&ll, data, &(hashmap_p->comparator));
		}
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			bst bstt; init_data_structure(hashmap_p, &bstt);
			
			bstt.root = (bstnode*) get_from_array(&(hashmap_p->hashmap_holder), bucket_index);
			return find_equals_in_bst(&bstt, data, FIRST_OCCURENCE);
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

	// evaluate the index of the bucket that this element will go into
	cy_uint bucket_index = get_bucket_index(hashmap_p, data);

	switch(hashmap_p->hashmap_policy)
	{
		case ROBINHOOD_HASHING :
		{
			// only a free floating rbhnode can be inserted in to hashmap
			if(!is_free_floating_rbhnode(get_node(data, hashmap_p)))
				break;

			// if the hashmap is full
			if(hashmap_p->element_count == get_bucket_count_hashmap(hashmap_p))
				break;

			cy_uint position_index = bucket_index;
			cy_uint probe_sequence_length = 0;

			while(1)
			{
				const void* data_at_position_index = get_from_array(&(hashmap_p->hashmap_holder), position_index);
			
				// found an empty bucket, insert the element here, while updating its position_index
				if(data_at_position_index == NULL)
				{
					set_in_array(&(hashmap_p->hashmap_holder), data, position_index);
					((rbhnode*)get_node(data, hashmap_p))->position_index = position_index;

					inserted = 1;
					break;
				}
				else
				{
					cy_uint probe_sequence_length_data_at_position_index = get_probe_sequence_length(hashmap_p, data_at_position_index);
					if(probe_sequence_length_data_at_position_index < probe_sequence_length)
					{
						// steal the slot, and update position_index of data
						set_in_array(&(hashmap_p->hashmap_holder), data, position_index);
						((rbhnode*)get_node(data, hashmap_p))->position_index = position_index;

						// now we need to iterate to reinsert the data_at_position_index
						data = data_at_position_index;
						probe_sequence_length = probe_sequence_length_data_at_position_index;
					}
				}

				// prepare for next iteration
				position_index = get_circular_next(position_index, get_bucket_count_hashmap(hashmap_p)); // equivalent to ==> (position_index + 1) % bucket_count
				probe_sequence_length++;
			}

			break;
		}
		case ELEMENTS_AS_LINKEDLIST_INSERT_AT_HEAD :
		{
			linkedlist ll; init_data_structure(hashmap_p, &ll);

			ll.head = (llnode*) get_from_array(&(hashmap_p->hashmap_holder), bucket_index);
			inserted = insert_head_in_linkedlist(&ll, data);
			set_in_array(&(hashmap_p->hashmap_holder), ll.head, bucket_index);
			break;
		}
		case ELEMENTS_AS_LINKEDLIST_INSERT_AT_TAIL :
		{
			linkedlist ll; init_data_structure(hashmap_p, &ll);

			ll.head = (llnode*) get_from_array(&(hashmap_p->hashmap_holder), bucket_index);
			inserted = insert_tail_in_linkedlist(&ll, data);
			set_in_array(&(hashmap_p->hashmap_holder), ll.head, bucket_index);
			break;
		}
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			bst bstt; init_data_structure(hashmap_p, &bstt);
			
			bstt.root = (bstnode*) get_from_array(&(hashmap_p->hashmap_holder), bucket_index);
			inserted = insert_in_bst(&bstt, data);
			set_in_array(&(hashmap_p->hashmap_holder), bstt.root, bucket_index);
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
			// a free floating rbhnode, can not be in any hashmap, hence can not be removed from the hashmap
			if(is_free_floating_rbhnode(get_node(data, hashmap_p)))
				break;

			// get the position index of the data
			cy_uint position_index = ((rbhnode*)get_node(data, hashmap_p))->position_index;

			// if the data does not exist at position_index in hashmap_holder, as dictated by its rbhnode, then it can not be removed
			if(data != get_from_array(&(hashmap_p->hashmap_holder), position_index))
				break;

			// make the slot at the position_index (in hashmap holder) empty, and re intialize its rbhnode
			set_in_array(&(hashmap_p->hashmap_holder), NULL, position_index);
			initialize_rbhnode(get_node(data, hashmap_p));
			deleted = 1;

			// Now we loop and move all elements that have non zero probe sequence length, a slot backwards
			cy_uint previous_position_index = position_index;
			position_index = get_circular_next(position_index, get_bucket_count_hashmap(hashmap_p)); // equivalent to ==> (position_index + 1) % bucket_count
			const void* data_at_position_index = get_from_array(&(hashmap_p->hashmap_holder), position_index);

			while(data_at_position_index != NULL && get_probe_sequence_length(hashmap_p, data_at_position_index) > 0)
			{
				// shift NULL downwards, and data_at_position_index to previousIndex
				{
					set_in_array(&(hashmap_p->hashmap_holder), data_at_position_index, previous_position_index);
					set_in_array(&(hashmap_p->hashmap_holder), NULL, position_index);

					// update position_index of data_at_position_index
					((rbhnode*)get_node(data_at_position_index, hashmap_p))->position_index = previous_position_index;
				}

				// prepare for next iteration
				previous_position_index = position_index;
				position_index = get_circular_next(position_index, get_bucket_count_hashmap(hashmap_p)); // equivalent to ==> (position_index + 1) % bucket_count
				data_at_position_index = get_from_array(&(hashmap_p->hashmap_holder), position_index);
			}

			break;
		}
		case ELEMENTS_AS_LINKEDLIST_INSERT_AT_HEAD :
		case ELEMENTS_AS_LINKEDLIST_INSERT_AT_TAIL :
		{
			cy_uint index = get_bucket_index(hashmap_p, data);
			linkedlist ll; init_data_structure(hashmap_p, &ll);

			ll.head = (llnode*) get_from_array(&(hashmap_p->hashmap_holder), index);
			deleted = remove_from_linkedlist(&ll, data);
			set_in_array(&(hashmap_p->hashmap_holder), ll.head, index);
			break;
		}
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			cy_uint index = get_bucket_index(hashmap_p, data);
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

const void* get_first_of_in_hashmap(const hashmap* hashmap_p, cy_uint bucket_index)
{
	// if there are no elements in the hashmap then return NULL immediately
	if(get_element_count_hashmap(hashmap_p) == 0)
		return NULL;

	if(bucket_index < get_bucket_count_hashmap(hashmap_p))
	{
		switch(hashmap_p->hashmap_policy)
		{
			case ROBINHOOD_HASHING :
			{
				cy_uint position_index = bucket_index;
				cy_uint probe_sequence_length = 0;

				const void* data_found = NULL;

				while(probe_sequence_length < get_bucket_count_hashmap(hashmap_p))
				{
					const void* data_at_position_index = get_from_array(&(hashmap_p->hashmap_holder), position_index);

					// bucket is empty OR data_at_position_index is at a lesser probe sequence length :: then quit the loop
					if(data_at_position_index == NULL || get_probe_sequence_length(hashmap_p, data_at_position_index) < probe_sequence_length)
						break;

					// the data_at_position_index has the required bucket_index :: the data we are looking for has been found
					if(bucket_index == get_bucket_index(hashmap_p, data_at_position_index))
					{
						data_found = data_at_position_index;
						break;
					}

					position_index = get_circular_next(position_index, get_bucket_count_hashmap(hashmap_p)); // equivalent to ==> (position_index + 1) % bucket_count
					probe_sequence_length++;
				}

				return data_found;
			}
			case ELEMENTS_AS_LINKEDLIST_INSERT_AT_HEAD :
			case ELEMENTS_AS_LINKEDLIST_INSERT_AT_TAIL :
			{
				linkedlist ll; init_data_structure(hashmap_p, &ll);

				ll.head = (llnode*) get_from_array(&(hashmap_p->hashmap_holder), bucket_index);

				return get_head_of_linkedlist(&ll);
			}
			case ELEMENTS_AS_AVL_BST :
			case ELEMENTS_AS_RED_BLACK_BST :
			{
				bst bstt; init_data_structure(hashmap_p, &bstt);

				bstt.root = (bstnode*) get_from_array(&(hashmap_p->hashmap_holder), bucket_index);

				return find_smallest_in_bst(&bstt);
			}
		}
	}
	else
	{
		// loop over hashmap holder and find the first valid bucket_index
		// we know here that there is atleast an element in hashmap (check in the beginning of this function)
		// hence there must be a non-NULL bucket in the hashmap_holder
		cy_uint first_valid_position_index = 0;
		for(; first_valid_position_index < get_bucket_count_hashmap(hashmap_p) && get_from_array(&(hashmap_p->hashmap_holder), first_valid_position_index) == NULL; first_valid_position_index++){}

		switch(hashmap_p->hashmap_policy)
		{
			case ROBINHOOD_HASHING :
			{
				return get_from_array(&(hashmap_p->hashmap_holder), first_valid_position_index);
			}
			case ELEMENTS_AS_LINKEDLIST_INSERT_AT_HEAD :
			case ELEMENTS_AS_LINKEDLIST_INSERT_AT_TAIL :
			{
				linkedlist ll; init_data_structure(hashmap_p, &ll);

				ll.head = (llnode*) get_from_array(&(hashmap_p->hashmap_holder), first_valid_position_index);

				return get_head_of_linkedlist(&ll);
			}
			case ELEMENTS_AS_AVL_BST :
			case ELEMENTS_AS_RED_BLACK_BST :
			{
				bst bstt; init_data_structure(hashmap_p, &bstt);

				bstt.root = (bstnode*) get_from_array(&(hashmap_p->hashmap_holder), first_valid_position_index);

				return find_smallest_in_bst(&bstt);
			}
		}
	}

	return NULL;
}

static const void* get_next_of_in_hashmap_ANY_IN_SAME_BUCKET(const hashmap* hashmap_p, const void* data_xist)
{
	cy_uint bucket_index = get_bucket_index(hashmap_p, data_xist);

	switch(hashmap_p->hashmap_policy)
	{
		case ROBINHOOD_HASHING :
		{
			rbhnode* node_xist = get_node(data_xist, hashmap_p);
			// node_xist must not be a free floating node
			if(is_free_floating_rbhnode(node_xist))
				return NULL;

			// position_index, and probe_sequence_length of the data_xist
			cy_uint position_index = node_xist->position_index;
			cy_uint probe_sequence_length = sub_circularly(position_index, bucket_index, get_bucket_count_hashmap(hashmap_p));

			// get the position_index of the next element
			position_index = get_circular_next(position_index, get_bucket_count_hashmap(hashmap_p));
			probe_sequence_length++;

			const void* next = NULL;

			while(probe_sequence_length < get_bucket_count_hashmap(hashmap_p))
			{
				const void* data_at_position_index = get_from_array(&(hashmap_p->hashmap_holder), position_index);

				// quit the loop returning NULL, if the data_at_position_index is NULL OR is at a smaller probe_sequence_length than expected
				if(data_at_position_index == NULL || get_probe_sequence_length(hashmap_p, data_at_position_index) < probe_sequence_length)
					break;

				// we are looking for the next element who falls in the same bucket
				if(get_bucket_index(hashmap_p, data_at_position_index) == bucket_index)
				{
					next = data_at_position_index;
					break;
				}

				// prepare for next iteration
				position_index = get_circular_next(position_index, get_bucket_count_hashmap(hashmap_p));
				probe_sequence_length++;
			}

			return next;
		}
		case ELEMENTS_AS_LINKEDLIST_INSERT_AT_HEAD :
		case ELEMENTS_AS_LINKEDLIST_INSERT_AT_TAIL :
		{
			linkedlist ll; init_data_structure(hashmap_p, &ll);

			ll.head = (llnode*) get_from_array(&(hashmap_p->hashmap_holder), bucket_index);

			// tail node of the linkedlist (it being a circular doubly linkedlist), will have its next reference pointing to head of the linkedlist
			// hence we check for tail, and if data_xist is tail node, we return NULL
			if(data_xist == get_tail_of_linkedlist(&ll))
				return NULL;

			return get_next_of_in_linkedlist(&ll, data_xist);
		}
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			bst bstt; init_data_structure(hashmap_p, &bstt);

			bstt.root = (bstnode*) get_from_array(&(hashmap_p->hashmap_holder), bucket_index);

			return get_inorder_next_of_in_bst(&bstt, data_xist);
		}
	}

	return NULL;
}

static const void* get_next_of_in_hashmap_ANY_IN_HASHMAP(const hashmap* hashmap_p, const void* data_xist)
{
	switch(hashmap_p->hashmap_policy)
	{
		case ROBINHOOD_HASHING :
		{
			rbhnode* node_xist = get_node(data_xist, hashmap_p);
			// node_xist must not be a free floating node
			if(is_free_floating_rbhnode(node_xist))
				return NULL;

			cy_uint next_position_index = node_xist->position_index + 1;

			// loop until you find the next_position_index at which hashmap_holder is a non-NULL
			for(; next_position_index < get_bucket_count_hashmap(hashmap_p) && get_from_array(&(hashmap_p->hashmap_holder), next_position_index) == NULL; next_position_index++){}

			// if you reached the end of the hashmap_holder then return NULL
			if(next_position_index == get_bucket_count_hashmap(hashmap_p))
				return NULL;

			// else return the element at the next_position_index
			return get_from_array(&(hashmap_p->hashmap_holder), next_position_index);
		}
		case ELEMENTS_AS_LINKEDLIST_INSERT_AT_HEAD :
		case ELEMENTS_AS_LINKEDLIST_INSERT_AT_TAIL :
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			// attempt to find next in same bucket
			const void* next = get_next_of_in_hashmap_ANY_IN_SAME_BUCKET(hashmap_p, data_xist);

			// if found return it
			if(next != NULL)
				return next;

			cy_uint bucket_index = get_bucket_index(hashmap_p, data_xist);

			// loop over hashmap holder and find the next valid bucket_index
			cy_uint next_bucket_index = bucket_index + 1;
			for(; next_bucket_index < get_bucket_count_hashmap(hashmap_p) && get_from_array(&(hashmap_p->hashmap_holder), next_bucket_index) == NULL; next_bucket_index++){}

			// if no such next valid bucket_index then return NULL
			if(next_bucket_index == get_bucket_count_hashmap(hashmap_p))
				return NULL;

			// else return the first element for that bucket
			return get_first_of_in_hashmap(hashmap_p, next_bucket_index);
		}
	}

	return NULL;
}

static const void* get_next_of_in_hashmap_ANY_THAT_EQUALS(const hashmap* hashmap_p, const void* data_xist)
{
	switch(hashmap_p->hashmap_policy)
	{
		// here we use the concept that objects that compare equal must lie in the same bucket
		case ROBINHOOD_HASHING :
		case ELEMENTS_AS_LINKEDLIST_INSERT_AT_HEAD :
		case ELEMENTS_AS_LINKEDLIST_INSERT_AT_TAIL :
		{
			const void* curr = data_xist;
			const void* next = get_next_of_in_hashmap_ANY_IN_SAME_BUCKET(hashmap_p, curr);

			// we iterate over all the elements of the bucket, untill we reach the end or we find the element that equals data_xist
			while(next != NULL && compare_with_comparator(&(hashmap_p->comparator), next, data_xist) != 0)
			{
				curr = next;
				next = get_next_of_in_hashmap_ANY_IN_SAME_BUCKET(hashmap_p, curr);
			}

			return next;
		}
		// here we use the concept that objects that compare equal must lie in the same bucket
		// Additionally, they must be adjacent to each other in the bst, since bst is always ordered
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			const void* next = get_next_of_in_hashmap_ANY_IN_SAME_BUCKET(hashmap_p, data_xist);

			// if there is no other element in the bucket, or if the next does not compare equal to data_xist
			// then return NULL
			if(next == NULL || compare_with_comparator(&(hashmap_p->comparator), next, data_xist) != 0)
				return NULL;

			return next;
		}
	}

	return NULL;
}

const void* get_next_of_in_hashmap(const hashmap* hashmap_p, const void* data_xist, hashmap_next_type next_type)
{
	switch(next_type)
	{
		case ANY_IN_HASHMAP :
			return get_next_of_in_hashmap_ANY_IN_HASHMAP(hashmap_p, data_xist);
		case ANY_IN_SAME_BUCKET :
			return get_next_of_in_hashmap_ANY_IN_SAME_BUCKET(hashmap_p, data_xist);
		case ANY_THAT_EQUALS :
			return get_next_of_in_hashmap_ANY_THAT_EQUALS(hashmap_p, data_xist);
		default :
			return NULL;
	}
}

void remove_all_from_hashmap(hashmap* hashmap_p, notifier_interface* ni_p)
{
	// iterate over all the buckets in the hashmap_p and removing all the elements
	switch(hashmap_p->hashmap_policy)
	{
		case ROBINHOOD_HASHING :
		{
			for(cy_uint index = 0; index < get_bucket_count_hashmap(hashmap_p); index++)
			{
				const void* data = get_from_array(&(hashmap_p->hashmap_holder), index);
				if(data == NULL)
					continue;

				// initialize rbhnode of all elements
				initialize_rbhnode(get_node(data, hashmap_p));

				// notify the notifier_interface ni_p, (if any)
				if(ni_p != NULL)
					ni_p->notify(ni_p->resource_p, data);
			}
			break;
		}
		case ELEMENTS_AS_LINKEDLIST_INSERT_AT_HEAD :
		case ELEMENTS_AS_LINKEDLIST_INSERT_AT_TAIL :
		{
			linkedlist ll; init_data_structure(hashmap_p, &ll);
			for(cy_uint index = 0; index < get_bucket_count_hashmap(hashmap_p); index++)
			{
				ll.head = (llnode*) get_from_array(&(hashmap_p->hashmap_holder), index);

				// remove all elements from the linkedlist bucket
				remove_all_from_linkedlist(&ll, ni_p);
			}
			break;
		}
		case ELEMENTS_AS_AVL_BST :
		case ELEMENTS_AS_RED_BLACK_BST :
		{
			bst bstt; init_data_structure(hashmap_p, &bstt);
			for(cy_uint index = 0; index < get_bucket_count_hashmap(hashmap_p); index++)
			{
				bstt.root = (bstnode*) get_from_array(&(hashmap_p->hashmap_holder), index);

				// remove all elements from the bst bucket
				remove_all_from_bst(&bstt, ni_p);
			}

			break;
		}
		default :
		{
			break;
		}
	}

	// at this point all the buckets contain garbage data,
	// hence we must mark all the buckets as NULL
	remove_all_from_array(&(hashmap_p->hashmap_holder));

	// then update element_count to 0
	hashmap_p->element_count = 0;
}

void deinitialize_hashmap(hashmap* hashmap_p)
{
	deinitialize_array(&(hashmap_p->hashmap_holder));
	hashmap_p->element_count = 0;
	hashmap_p->hasher = simple_hasher(NULL);
	hashmap_p->comparator = simple_comparator(NULL);
	hashmap_p->node_offset = 0;
}

void for_each_in_hashmap(const hashmap* hashmap_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params)
{
	// iterate over all the buckets in the hashmap_p
	switch(hashmap_p->hashmap_policy)
	{
		case ROBINHOOD_HASHING :
		{
			for(cy_uint index = 0; index < get_bucket_count_hashmap(hashmap_p); index++)
			{
				const void* data = get_from_array(&(hashmap_p->hashmap_holder), index);
				if(data != NULL)
					operation(data, additional_params);
			}
			break;
		}
		case ELEMENTS_AS_LINKEDLIST_INSERT_AT_HEAD :
		case ELEMENTS_AS_LINKEDLIST_INSERT_AT_TAIL :
		{
			linkedlist ll; init_data_structure(hashmap_p, &ll);
			for(cy_uint index = 0; index < get_bucket_count_hashmap(hashmap_p); index++)
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
			for(cy_uint index = 0; index < get_bucket_count_hashmap(hashmap_p); index++)
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

int resize_hashmap(hashmap* hashmap_p, cy_uint new_bucket_count)
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
	if(!initialize_hashmap_with_allocator(&new_hashmap, hashmap_p->hashmap_policy, new_bucket_count, &(hashmap_p->hasher), &(hashmap_p->comparator), hashmap_p->node_offset, hashmap_p->hashmap_holder.mem_allocator))
		return 0;

	// we will use notifier_interface to remove all from hashmap_p and insert it into new_hashmap
	remove_all_from_hashmap(hashmap_p, &((notifier_interface){&new_hashmap, (void(*)(void*, const void*)) insert_in_hashmap}));

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

	// calculate new capacity
	cy_uint new_capacity;
	{
		double new_capacity_as_double = ((double)expand_factor) * get_bucket_count_hashmap(hashmap_p);
		if(new_capacity_as_double > MAX_ARRAY_CAPACITY)
			new_capacity = MAX_ARRAY_CAPACITY;
		else
			new_capacity = new_capacity_as_double;
	}

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
		case ELEMENTS_AS_LINKEDLIST_INSERT_AT_HEAD :
		{
			snprintf_dstring(append_str, "(elements_as_linkedlist_insert_at_head) :\n");
			break;
		}
		case ELEMENTS_AS_LINKEDLIST_INSERT_AT_TAIL :
		{
			snprintf_dstring(append_str, "(elements_as_linkedlist_insert_at_tail) :\n");
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
	snprintf_dstring(append_str, "node_offset : %" PRIu_cy_uint "\n", hashmap_p->node_offset);

	sprint_chars(append_str, '\t', tabs);
	snprintf_dstring(append_str, "element_count : %" PRIu_cy_uint "\n", hashmap_p->element_count);

	sprint_chars(append_str, '\t', tabs);
	snprintf_dstring(append_str, "bucket_count : %" PRIu_cy_uint "\n", get_bucket_count_hashmap(hashmap_p));

	linkedlist ll; init_data_structure(hashmap_p, &ll);
	bst bstt; init_data_structure(hashmap_p, &bstt);

	// iterate over all the buckets in the hashmap_p
	for(cy_uint index = 0; index < get_bucket_count_hashmap(hashmap_p); index++)
	{
		sprint_chars(append_str, '\t', tabs + 1);
		snprintf_dstring(append_str, "bucket_id = %" PRIu_cy_uint "\n", index);

		if(get_from_array(&(hashmap_p->hashmap_holder), index) != NULL)
		{
			switch(hashmap_p->hashmap_policy)
			{
				case ROBINHOOD_HASHING :
				{
					const void* data = get_from_array(&(hashmap_p->hashmap_holder), index);
					sprint_chars(append_str, '\t', tabs + 2);
					snprintf_dstring(append_str, "position_index = %" PRIu_cy_uint "\n", ((rbhnode*)get_node(data, hashmap_p))->position_index);
					sprint_element(append_str, data, tabs + 2);
					snprintf_dstring(append_str, "\n");
					break;
				}
				case ELEMENTS_AS_LINKEDLIST_INSERT_AT_HEAD :
				case ELEMENTS_AS_LINKEDLIST_INSERT_AT_TAIL :
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