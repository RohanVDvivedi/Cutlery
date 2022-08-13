#include<arraylist.h>

#include<cutlery_stds.h>

#include<circular_buffer_array_util.h>

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

void initialize_arraylist_with_memory(arraylist* al, unsigned int capacity, const void* data_ps[])
{
	initialize_array_with_memory(&(al->arraylist_holder), capacity, data_ps);
	al->first_index = 0;
	al->element_count = 0;
}

int push_front_to_arraylist(arraylist* al, const void* data_p)
{
	// if full, you can't push to arraylist
	if(is_full_arraylist(al))
		return 0;

	// update the first_index, to its new place where it will hold the new element
	// if empty push the element at 0th index
	if(is_empty_arraylist(al))
		al->first_index = 0;
	else // else an index prior to the first index in the circular index scheme
		al->first_index = get_circular_prev(al->first_index, get_capacity_arraylist(al));

	// push to front of array list
	set_in_array(&(al->arraylist_holder), data_p, al->first_index);

	// increment the element counter
	al->element_count++;

	return 1;
}

int push_back_to_arraylist(arraylist* al, const void* data_p)
{
	// if full, you can't push to arraylist
	if(is_full_arraylist(al))
		return 0;

	// if empty push the element at 0th index
	if(is_empty_arraylist(al))
	{
		al->first_index = 0;

		// push to back of array list
		set_in_array(&(al->arraylist_holder), data_p, al->first_index);
	}
	else
	{
		// end_index is the index to the position on the circular buffer, that is immediately after the last element
		unsigned int end_index = get_end_index(al->first_index, al->element_count, get_capacity_arraylist(al));

		// push to back of array list
		set_in_array(&(al->arraylist_holder), data_p, end_index);
	}

	// increment the element counter
	al->element_count++;

	return 1;
}

int pop_front_from_arraylist(arraylist* al)
{
	// if empty you can't pop from the arraylist
	if(is_empty_arraylist(al))
		return 0;

	// pop an element from front of the arraylist
	set_in_array(&(al->arraylist_holder), NULL, al->first_index);

	// update the first index
	al->first_index = get_circular_next(al->first_index, get_capacity_arraylist(al));

	// decrement the element counter
	al->element_count--;

	return 1;
}

int pop_back_from_arraylist(arraylist* al)
{
	// if empty you can't pop from the arraylist
	if(is_empty_arraylist(al))
		return 0;

	// find the index to the last element in the arraylist
	unsigned int last_index = get_last_index(al->first_index, al->element_count, get_capacity_arraylist(al));

	// pop an element from back of the arraylist
	set_in_array(&(al->arraylist_holder), NULL, last_index);

	// decrement the element counter
	al->element_count--;

	return 1;
}

const void* get_front_of_arraylist(const arraylist* al)
{
	// if empty, return NULL
	if(is_empty_arraylist(al))
		return NULL;

	// find front element of the arraylist, and return it
	return get_from_array(&(al->arraylist_holder), al->first_index);
}

const void* get_back_of_arraylist(const arraylist* al)
{
	// if empty, return NULL
	if(is_empty_arraylist(al))
		return NULL;

	// find back element of the arraylist
	return get_from_array(&(al->arraylist_holder), get_last_index(al->first_index, al->element_count, get_capacity_arraylist(al)));
}

const void* get_nth_from_front_of_arraylist(const arraylist* al, unsigned int n)
{
	// arraylist must not be empty and the index-n must be lesser than the element-count
	if(is_empty_arraylist(al) || n >= al->element_count)
		return NULL;

	// find the index of our concern
	unsigned int index_concerned = add_indexes(al->first_index, n, get_capacity_arraylist(al));

	// find the concerned element and return it
	return get_from_array(&(al->arraylist_holder), index_concerned);
}

const void* get_nth_from_back_of_arraylist(const arraylist* al, unsigned int n)
{
	// arraylist must not be empty and the index-n must be lesser than the element-count
	if(is_empty_arraylist(al) || n >= al->element_count)
		return NULL;

	// find the index of our concern
	unsigned int last_index = get_last_index(al->first_index, al->element_count, get_capacity_arraylist(al));
	unsigned int index_concerned = sub_indexes(last_index , n, get_capacity_arraylist(al));

	// find the concerned element and return it
	return get_from_array(&(al->arraylist_holder), index_concerned);
}

int set_nth_from_front_in_arraylist(arraylist* al, const void* data_p, unsigned int n)
{
	// arraylist must not be empty and the index-n must be lesser than the element-count
	if(is_empty_arraylist(al) || n >= al->element_count)
		return 0;

	// find the index of our concern
	unsigned int index_concerned = add_indexes(al->first_index, n, get_capacity_arraylist(al));

	// find the concerned element and set it
	return set_in_array(&(al->arraylist_holder), data_p, index_concerned);
}

int set_nth_from_back_in_arraylist(arraylist* al, const void* data_p, unsigned int n)
{
	// arraylist must not be empty and the index-n must be lesser than the element-count
	if(is_empty_arraylist(al) || n >= al->element_count)
		return 0;

	// find the index of our concern
	unsigned int last_index = get_last_index(al->first_index, al->element_count, get_capacity_arraylist(al));
	unsigned int index_concerned = sub_indexes(last_index , n, get_capacity_arraylist(al));

	// find the concerned element and return it
	return set_in_array(&(al->arraylist_holder), data_p, index_concerned);
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

static void linearlize_arraylist_upon_expansion(arraylist* al, unsigned int old_capacity)
{
	// element_count remains the same, it is indifferent to re-linearizing the data in this function
	unsigned int element_count = get_element_count_arraylist(al);

	// quit if there are no elements to be linearized
	if(element_count == 0)
		return ;

	// this is the first index before we started our data management and linearization task
	unsigned int old_first_index = al->first_index;

	// check if the arraylist does need linearization
	// i.e. linearization is required only if the arraylist was wrapping arround its holder
	if(old_first_index <= get_last_index(old_first_index, element_count, old_capacity))
		return ;

	// reaching this place implies that the arraylist is wrapped around and has 2 parts: a head and a tail
	// lets calculate the number of elements in the head and tail
	unsigned int elements_in_head = old_capacity - old_first_index;   // elements from old_first_index until old_capacity - 1
	unsigned int elements_in_tail = element_count - elements_in_head; // elements from 0 to last_index == element_count - elements_in_head

	if(elements_in_head <= elements_in_tail)
	{
		// in this case
		// we move the elements in the head to the end of the arraylist holder

		// calculate the new first index
		unsigned int new_first_index = get_capacity_arraylist(al) - elements_in_head;

		// move all elements in head to the end of the arraylist holder
		copy_elements_from_array(&(al->arraylist_holder), new_first_index, &(al->arraylist_holder), old_first_index, elements_in_head);

		// mem set all old unused positions in the array as NULL (only if they previously were in use and are not in use now)
		unsigned int elements_to_NULL = new_first_index - old_first_index;
		elements_to_NULL = (elements_to_NULL > elements_in_head) ? elements_in_head : elements_to_NULL;
		set_NULLs_in_array(&(al->arraylist_holder), old_first_index, elements_to_NULL);

		// update the new first_index
		al->first_index = new_first_index;
	}
	else
	{
		// in this case
		// we move as many elements as we can to the new slots available at the end
		// then we move the remaining tail elements to the index 0
		// and now we can set the unused slots to NULLs

		// this is the number of elements by which the capacity of arraylist increased
		// this is also the maximum number of elements that can be moved from tail to bottom part of head
		unsigned int new_slots_added = get_capacity_arraylist(al) - old_capacity;

		unsigned int tail_elements_to_relocate = new_slots_added;
		if(elements_in_tail < new_slots_added)
			tail_elements_to_relocate = elements_in_tail;

		copy_elements_from_array(&(al->arraylist_holder), old_capacity, &(al->arraylist_holder), 0, tail_elements_to_relocate);

		unsigned int tail_elements_to_shift = elements_in_tail - tail_elements_to_relocate;
		if(tail_elements_to_shift > 0)
			copy_elements_from_array(&(al->arraylist_holder), 0, &(al->arraylist_holder), tail_elements_to_relocate, tail_elements_to_shift);

		set_NULLs_in_array(&(al->arraylist_holder), tail_elements_to_shift, tail_elements_to_relocate);
	}
}

int expand_arraylist(arraylist* al)
{
	int data_movement_will_be_required = 1;

	// on this condition, we can expand without any data movement
	if(is_empty_arraylist(al) || al->first_index <= get_last_index(al->first_index, al->element_count, get_capacity_arraylist(al)))
		data_movement_will_be_required = 0;

	// record old_capacity for future use
	unsigned int old_capacity = get_capacity_arraylist(al);

	// expand the holder fearlessly
	int has_holder_expanded = expand_array(&(al->arraylist_holder));

	// move data if necessary conditions meet
	if(data_movement_will_be_required && has_holder_expanded)
		linearlize_arraylist_upon_expansion(al, old_capacity);

	return has_holder_expanded;
}

int reserve_capacity_for_arraylist(arraylist* al, unsigned int atleast_capacity)
{
	int data_movement_will_be_required = 1;

	// on this condition, we can expand without any data movement
	if(is_empty_arraylist(al) || al->first_index <= get_last_index(al->first_index, al->element_count, get_capacity_arraylist(al)))
		data_movement_will_be_required = 0;

	// record old_capacity for future use
	unsigned int old_capacity = get_capacity_arraylist(al);

	// expand the holder fearlessly
	int has_holder_expanded = reserve_capacity_for_array(&(al->arraylist_holder), atleast_capacity);

	// move data if necessary conditions meet
	if(data_movement_will_be_required && has_holder_expanded)
		linearlize_arraylist_upon_expansion(al, old_capacity);

	return has_holder_expanded;
}

int shrink_arraylist(arraylist* al)
{
	int has_holder_shrunk = 0;

	if(get_capacity_arraylist(al) == 0)
		return has_holder_shrunk;

	// to be able to shrink an arraylist, it must have a non-zero size
	// and there is no rotation, i.e. elements are all contiguously placed at index after the first index
	if(is_empty_arraylist(al) || al->first_index <= get_last_index(al->first_index, al->element_count, get_capacity_arraylist(al)))
	{
		if(!is_empty_arraylist(al) && al->first_index > 0)
		{
			// move the arraylist to the complete front, i.e. first_index = 0
			memory_move(al->arraylist_holder.data_p_p,
						al->arraylist_holder.data_p_p + al->first_index,
						al->element_count * sizeof(void*));

			// update first_index of arraylist to 0
			al->first_index = 0;
		}

		has_holder_shrunk = shrink_array(&(al->arraylist_holder), al->element_count);
	}

	return has_holder_shrunk;
}

const void* find_equals_in_arraylist(const arraylist* al, const void* data, int (*compare)(const void* data1, const void* data2))
{
	for(unsigned int i = 0; i < al->element_count; i++)
	{
		unsigned int al_index = add_indexes(al->first_index, i, get_capacity_arraylist(al));
		const void* found = get_from_array(&(al->arraylist_holder), al_index);
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
	
	const void* front_element = get_front_of_arraylist(al);
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "front : ");
	(front_element != NULL) ? sprint_element(append_str, front_element, 0) : snprintf_dstring(append_str, "NULL");
	snprintf_dstring(append_str, "\n");

	const void* back_element = get_back_of_arraylist(al);
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