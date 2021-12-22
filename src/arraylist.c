#include<arraylist.h>

#include<cutlery_stds.h>

void initialize_arraylist(arraylist* al, unsigned int capacity)
{
	initialize_array(&(al->arraylist_holder), capacity);
	al->first_index = 0;
	al->element_count = 0;
}

void initialize_arraylist_with_allocator(arraylist* al, unsigned int capacity, memory_allocator mem_allocator)
{
	initialize_array_with_allocator(&(al->arraylist_holder), capacity, mem_allocator);
	al->first_index = 0;
	al->element_count = 0;
}

int push_front(arraylist* al, const void* data_p)
{
	// if full, you can't push to arraylist
	if(is_full_arraylist(al))
		return 0;

	// update the first_index, to its new place where it will hold the new element
	// if empty push the element at 0th index
	if(is_empty_arraylist(al))
		al->first_index = 0;
	else // else an index prior to the first index in the circular index scheme
		al->first_index = ((al->first_index + get_capacity_arraylist(al)) - 1) % get_capacity_arraylist(al);

	// push to front of array list
	set_element(&(al->arraylist_holder), data_p, al->first_index);

	// increment the element counter
	al->element_count++;

	return 1;
}

int push_back(arraylist* al, const void* data_p)
{
	// if full, you can't push to arraylist
	if(is_full_arraylist(al))
		return 0;

	// if empty push the element at 0th index
	if(is_empty_arraylist(al))
		al->first_index = 0;

	// end_index is the index to the position on the circular buffer, that is immediately after the last element
	unsigned int end_index = (al->first_index + al->element_count) % get_capacity_arraylist(al);

	// push to back of array list
	set_element(&(al->arraylist_holder), data_p, end_index);

	// increment the element counter
	al->element_count++;

	return 1;
}

int pop_front(arraylist* al)
{
	// if empty you can't pop from the arraylist
	if(is_empty_arraylist(al))
		return 0;

	// pop an element from front of the arraylist
	set_element(&(al->arraylist_holder), NULL, al->first_index);

	// update the first index
	al->first_index = (al->first_index + 1) % get_capacity_arraylist(al);

	// decrement the element counter
	al->element_count--;

	return 1;
}

int pop_back(arraylist* al)
{
	// if empty you can't pop from the arraylist
	if(is_empty_arraylist(al))
		return 0;

	// find the index to the last element in the arraylist
	unsigned int back_index = ((al->first_index + al->element_count) - 1) % get_capacity_arraylist(al);

	// pop an element from front of the arraylist
	set_element(&(al->arraylist_holder), NULL, back_index);

	// decrement the element counter
	al->element_count--;

	return 1;
}

const void* get_front(const arraylist* al)
{
	// if empty, return NULL
	if(is_empty_arraylist(al))
		return NULL;

	// find front element of the arraylist, and return it
	return get_element(&(al->arraylist_holder), al->first_index);
}

const void* get_back(const arraylist* al)
{
	// if empty, return NULL
	if(is_empty_arraylist(al))
		return NULL;

	// find back element of the arraylist
	return get_element(&(al->arraylist_holder), (al->first_index + al->element_count - 1) % get_capacity_arraylist(al));
}

const void* get_nth_from_front(const arraylist* al, unsigned int n)
{
	// arraylist must not be empty and the index-n must be lesser than the element-count
	if(is_empty_arraylist(al) || n >= al->element_count)
		return NULL;

	// find nth element from front of the arraylist, and return it
	return get_element(&(al->arraylist_holder), (al->first_index + n) % get_capacity_arraylist(al));
}

const void* get_nth_from_back(const arraylist* al, unsigned int n)
{
	// arraylist must not be empty and the index-n must be lesser than the element-count
	if(is_empty_arraylist(al) || n >= al->element_count)
		return NULL;

	// find nth element from back of the arraylist, and return it
	return get_element(&(al->arraylist_holder), (((al->first_index + al->element_count) - 1) - n) % get_capacity_arraylist(al));
}

int set_nth_from_front(arraylist* al, const void* data_p, unsigned int n)
{
	// arraylist must not be empty and the index-n must be lesser than the element-count
	if(is_empty_arraylist(al) || n >= al->element_count)
		return 0;

	// set nth element from front of the arraylist to data_p
	return set_element(&(al->arraylist_holder), data_p, (al->first_index + n) % get_capacity_arraylist(al));
}

int set_nth_from_back(arraylist* al, const void* data_p, unsigned int n)
{
	// arraylist must not be empty and the index-n must be lesser than the element-count
	if(is_empty_arraylist(al) || n >= al->element_count)
		return 0;

	// set nth element from back of the arraylist to data_p
	return set_element(&(al->arraylist_holder), data_p, (((al->first_index + al->element_count) - 1) - n) % get_capacity_arraylist(al));
}

unsigned int get_capacity_arraylist(const arraylist* al)
{
	return get_capacity_array(&(al->arraylist_holder));
}

unsigned int get_element_count_arraylist(const arraylist* al)
{
	return al->element_count;
}

int is_full_arraylist(const arraylist* al)
{
	return al->element_count == get_capacity_arraylist(al);
}

int is_empty_arraylist(const arraylist* al)
{
	return al->element_count == 0;
}

int expand_arraylist(arraylist* al)
{
	int data_movement_will_be_required = 1;

	// on this condition, we can expand without any data movement
	if(is_empty_arraylist(al) || (al->first_index + al->element_count) <= get_capacity_arraylist(al))
		data_movement_will_be_required = 0;

	// record size and first index for further use
	unsigned int old_first_index = al->first_index;
	unsigned int old_capacity = get_capacity_arraylist(al);

	// expand the holder fearlessly
	int has_holder_expanded = expand_array(&(al->arraylist_holder));

	// move data if necessary conditions meet
	if(data_movement_will_be_required && has_holder_expanded)
	{
		// in this if condition 
		// we move elements that were at old_first_index to (old_capacity - 1) (both inclusive)
		// to the indices at new_first_index to (new_capacity - 1) (both inclusive),
		// only then we update the al->first_index to new_first_index
		// additionally we NULL all the old_indices that are not used

		// move partial data, that was at the end of the array
		unsigned int elements_to_move = old_capacity - old_first_index;

		// calculate the new first index
		unsigned int new_first_index = get_capacity_arraylist(al) - elements_to_move;

		// move data
		memory_move(al->arraylist_holder.data_p_p + new_first_index,
					al->arraylist_holder.data_p_p + old_first_index,
					elements_to_move * sizeof(void*));

		// mem set all old unused positions in the array as NULL (only if they previously were in use and are not in use now)
		unsigned int elements_to_NULL = new_first_index - old_first_index;
		elements_to_NULL = (elements_to_NULL > elements_to_move) ? elements_to_move : elements_to_NULL;
		memory_set(al->arraylist_holder.data_p_p + old_first_index, 0,
					elements_to_NULL * sizeof(void*));

		// update the new first_index
		al->first_index = new_first_index;
	}

	return has_holder_expanded;
}

int shrink_arraylist(arraylist* al)
{
	int has_holder_shrunk = 0;

	// to be able to shrink an array, it must have a non-zero size
	// and there is no rotation, i.e. elements are all contiguously placed at index after the first index
	if((get_capacity_arraylist(al) > 0) && (is_empty_arraylist(al) || (al->first_index + al->element_count) <= get_capacity_arraylist(al)))
	{
		if(!is_empty_arraylist(al) && al->first_index > 0)
		{
			// move the arraylist to the complete front, i.e. first_index = 0
			memory_move(al->arraylist_holder.data_p_p,
						al->arraylist_holder.data_p_p + al->first_index,
						al->element_count * sizeof(void*));

			// end_index = index that comes after the last index
			unsigned int elements_to_NULL_from = (al->first_index > al->element_count) ? al->first_index : al->element_count;
			unsigned int elements_to_NULL_to   = al->first_index + al->element_count - 1;
			unsigned int elements_to_NULL = elements_to_NULL_to - elements_to_NULL_from + 1;
			memory_set(al->arraylist_holder.data_p_p + elements_to_NULL_from, 0,
						elements_to_NULL * sizeof(void*));

			al->first_index = 0;
		}

		has_holder_shrunk = shrink_array(&(al->arraylist_holder), al->element_count);
	}

	// if the arraylist_holder had shrunk, the new first_index has to be at 0ss
	if(has_holder_shrunk)
		al->first_index = 0;

	return has_holder_shrunk;
}

const void* find_equals_in_arraylist(const arraylist* al, const void* data, int (*compare)(const void* data1, const void* data2))
{
	for(unsigned int i = 0, index = al->first_index; i < al->element_count; i++, index++)
	{
		const void* found = get_element(&(al->arraylist_holder), index % get_capacity_arraylist(al));
		if(0 == compare(found, data))
			return found;
	}

	return NULL;
}

void for_each_in_arraylist(const arraylist* al, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params)
{
	for_each_non_null_in_array(&(al->arraylist_holder), operation, additional_params);
}

void sprint_arraylist(dstring* append_str, const arraylist* al, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs++); snprintf_dstring(append_str, "arraylist :\n");
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "first_index : %u\n", al->first_index);
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "element_count : %u\n", al->element_count);
	
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "arraylist_holder : \n");
	sprint_array(append_str, &(al->arraylist_holder), sprint_element, tabs + 1);
	snprintf_dstring(append_str, "\n");
	
	const void* front_element = get_front(al);
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "front : ");
	(front_element != NULL) ? sprint_element(append_str, front_element, 0) : snprintf_dstring(append_str, "NULL");
	snprintf_dstring(append_str, "\n");

	const void* back_element = get_back(al);
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "back  : ");
	(back_element != NULL) ? sprint_element(append_str, back_element, 0) : snprintf_dstring(append_str, "NULL");
	snprintf_dstring(append_str, "\n");
}

void remove_all_from_arraylist(arraylist* al)
{
	al->first_index = 0;
	al->element_count = 0;
	
	// then NULL out all the array -> this step can be ignored
	remove_all_from_array(&(al->arraylist_holder));
}

void deinitialize_arraylist(arraylist* al)
{
	deinitialize_array(&(al->arraylist_holder));
	al->first_index = 0;
	al->element_count = 0;
}