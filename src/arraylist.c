#include<arraylist.h>

#include<cutlery_math.h>
#include<cutlery_stds.h>

#include<circular_buffer_array_util.h>

int initialize_arraylist(arraylist* al, cy_uint capacity)
{
	al->first_index = 0;
	al->element_count = 0;
	return initialize_array(&(al->arraylist_holder), capacity);
}

int initialize_arraylist_with_allocator(arraylist* al, cy_uint capacity, memory_allocator mem_allocator)
{
	al->first_index = 0;
	al->element_count = 0;
	return initialize_array_with_allocator(&(al->arraylist_holder), capacity, mem_allocator);
}

int initialize_arraylist_with_memory(arraylist* al, cy_uint capacity, const void* data_ps[])
{
	al->first_index = 0;
	al->element_count = 0;
	return initialize_array_with_memory(&(al->arraylist_holder), capacity, data_ps);
}

// below utility function, NULLs element_count_to_NULL number of elements starting at first_index in the provided array
// for using the below utility function, following conditions must be met
// first_index < get_capacity_arraylist(al)
// element_count_to_NULL <= get_capacity_arraylist(al)
static void circularly_NULL_elements_in_arraylist_holder(array* arraylist_holder, cy_uint first_index, cy_uint element_count_to_NULL)
{
	// if no elements are to be NULL-ed, then quit
	if(element_count_to_NULL == 0)
		return;

	// if all the elements in the arraylist are to be NULL-ed, then do it
	if(element_count_to_NULL == get_capacity_array(arraylist_holder))
	{
		set_all_NULL_in_array(arraylist_holder);
		return;
	}

	// else do it selectively and circularly
	{
		cy_uint elements_to_NULL = min(get_capacity_array(arraylist_holder) - first_index, element_count_to_NULL);
		set_NULLs_in_array(arraylist_holder, first_index, elements_to_NULL);

		// calculate remaining elements to be NULL-ed, that are at the begining of the array
		elements_to_NULL = element_count_to_NULL - elements_to_NULL;
		if(elements_to_NULL > 0)
			set_NULLs_in_array(arraylist_holder, 0, elements_to_NULL);
	}
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

	// if the arraylist is empty, then reset its first_index before the insertion
	if(is_empty_arraylist(al))
		al->first_index = 0;

	// end_index is the index to the position on the circular buffer, that is immediately after the last element
	cy_uint end_index = get_end_index(al->first_index, al->element_count, get_capacity_arraylist(al));

	// push to back of array list
	set_in_array(&(al->arraylist_holder), data_p, end_index);

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

	// reset the first index if this pop, made the arraylist empty
	if(is_empty_arraylist(al))
		al->first_index = 0;

	return 1;
}

int pop_back_from_arraylist(arraylist* al)
{
	// if empty you can't pop from the arraylist
	if(is_empty_arraylist(al))
		return 0;

	// find the index to the last element in the arraylist
	cy_uint last_index = get_last_index(al->first_index, al->element_count, get_capacity_arraylist(al));

	// pop an element from back of the arraylist
	set_in_array(&(al->arraylist_holder), NULL, last_index);

	// decrement the element counter
	al->element_count--;

	// reset the first index if this pop, made the arraylist empty
	if(is_empty_arraylist(al))
		al->first_index = 0;

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

const void* get_from_front_of_arraylist(const arraylist* al, cy_uint index)
{
	// arraylist must not be empty and the index must be lesser than the element-count
	if(is_empty_arraylist(al) || index >= al->element_count)
		return NULL;

	// find the index of our concern
	cy_uint index_concerned = add_circularly(al->first_index, index, get_capacity_arraylist(al));

	// find the concerned element and return it
	return get_from_array(&(al->arraylist_holder), index_concerned);
}

const void* get_from_back_of_arraylist(const arraylist* al, cy_uint index)
{
	// arraylist must not be empty and the index must be lesser than the element-count
	if(is_empty_arraylist(al) || index >= al->element_count)
		return NULL;

	// find the index of our concern
	cy_uint last_index = get_last_index(al->first_index, al->element_count, get_capacity_arraylist(al));
	cy_uint index_concerned = sub_circularly(last_index, index, get_capacity_arraylist(al));

	// find the concerned element and return it
	return get_from_array(&(al->arraylist_holder), index_concerned);
}

int set_from_front_in_arraylist(arraylist* al, const void* data_p, cy_uint index)
{
	// arraylist must not be empty and the index must be lesser than the element-count
	if(is_empty_arraylist(al) || index >= al->element_count)
		return 0;

	// find the index of our concern
	cy_uint index_concerned = add_circularly(al->first_index, index, get_capacity_arraylist(al));

	// find the concerned element and set it
	return set_in_array(&(al->arraylist_holder), data_p, index_concerned);
}

int set_from_back_in_arraylist(arraylist* al, const void* data_p, cy_uint index)
{
	// arraylist must not be empty and the index must be lesser than the element-count
	if(is_empty_arraylist(al) || index >= al->element_count)
		return 0;

	// find the index of our concern
	cy_uint last_index = get_last_index(al->first_index, al->element_count, get_capacity_arraylist(al));
	cy_uint index_concerned = sub_circularly(last_index, index, get_capacity_arraylist(al));

	// find the concerned element and return it
	return set_in_array(&(al->arraylist_holder), data_p, index_concerned);
}

int swap_from_front_in_arraylist(arraylist* al, cy_uint i1, cy_uint i2)
{
	if(i1 >= get_element_count_arraylist(al) || i2 >= get_element_count_arraylist(al))
		return 0;

	// convert them to index that we would be concerned with in the arraylist_holder
	cy_uint i1_concerned = add_circularly(al->first_index, i1, get_capacity_arraylist(al));
	cy_uint i2_concerned = add_circularly(al->first_index, i2, get_capacity_arraylist(al));

	return swap_in_array(&(al->arraylist_holder), i1_concerned, i2_concerned);
}

int swap_from_back_in_arraylist(arraylist* al, cy_uint i1, cy_uint i2)
{
	if(i1 >= get_element_count_arraylist(al) || i2 >= get_element_count_arraylist(al))
		return 0;

	cy_uint last_index = get_last_index(al->first_index, al->element_count, get_capacity_arraylist(al));

	// convert them to index that we would be concerned with in the arraylist_holder
	cy_uint i1_concerned = sub_circularly(last_index, i1, get_capacity_arraylist(al));
	cy_uint i2_concerned = sub_circularly(last_index, i2, get_capacity_arraylist(al));

	return swap_in_array(&(al->arraylist_holder), i1_concerned, i2_concerned);
}

// the below internal function assumes that
// index is valid [0 to element_count, both inclusive], and NULL_count_to_insert > 0
// and get_capacity - get_element_count >= NULL_count_to_insert
static void insert_NULLs_from_front_in_arraylist_INTERNAL(arraylist* al, cy_uint index, cy_uint NULL_count_to_insert)
{
	cy_uint elements_before_NULLs = index;
	cy_uint elements_after_NULLs = get_element_count_arraylist(al) - index;

	if(get_element_count_arraylist(al) == 0) // nothing to be moved
	{
		al->first_index = 0;
		al->element_count = NULL_count_to_insert;
	}
	else if(elements_before_NULLs <= elements_after_NULLs) // make space by moving before elements
	{
		// move before elements
		cy_uint elements_to_be_moved = elements_before_NULLs;
		cy_uint index_to_move_from = al->first_index;
		cy_uint index_to_move_to = sub_circularly(index_to_move_from, NULL_count_to_insert, get_capacity_arraylist(al));
		while(elements_to_be_moved)
		{
			set_in_array(&(al->arraylist_holder), get_from_array(&(al->arraylist_holder), index_to_move_from), index_to_move_to);
			index_to_move_from = add_circularly(index_to_move_from, 1, get_capacity_arraylist(al));
			index_to_move_to = add_circularly(index_to_move_to, 1, get_capacity_arraylist(al));
			elements_to_be_moved--;
		}

		// we only need to NULL out these many elements
		cy_uint non_NULL_elements_to_NULL = min(elements_before_NULLs, NULL_count_to_insert);
		cy_uint non_NULL_elements_to_NULL_first_index = add_circularly(al->first_index, elements_before_NULLs - non_NULL_elements_to_NULL, get_capacity_arraylist(al));;

		// now we only need to NULL those non-NULL slots, that we created
		circularly_NULL_elements_in_arraylist_holder(&(al->arraylist_holder), non_NULL_elements_to_NULL_first_index, non_NULL_elements_to_NULL);

		// now we only need to fix the first_index and element_count of the arraylist

		// increment element_count
		al->element_count += NULL_count_to_insert;

		// decrement the arraylist's first_index
		al->first_index = sub_circularly(al->first_index, NULL_count_to_insert, get_capacity_arraylist(al));
	}
	else // make space by moving after elements
	{
		// move after elements
		cy_uint elements_to_be_moved = elements_after_NULLs;
		cy_uint index_to_move_from = get_end_index(al->first_index, get_element_count_arraylist(al), get_capacity_arraylist(al));
		cy_uint index_to_move_to = add_circularly(index_to_move_from, NULL_count_to_insert, get_capacity_arraylist(al));
		while(elements_to_be_moved)
		{
			index_to_move_from = sub_circularly(index_to_move_from, 1, get_capacity_arraylist(al));
			index_to_move_to = sub_circularly(index_to_move_to, 1, get_capacity_arraylist(al));
			set_in_array(&(al->arraylist_holder), get_from_array(&(al->arraylist_holder), index_to_move_from), index_to_move_to);
			elements_to_be_moved--;
		}

		// we only need to NULL out these many elements
		cy_uint non_NULL_elements_to_NULL = min(elements_after_NULLs, NULL_count_to_insert);
		cy_uint non_NULL_elements_to_NULL_first_index = add_circularly(al->first_index, elements_before_NULLs, get_capacity_arraylist(al));

		// now we only need to NULL those non-NULL slots, that we created
		circularly_NULL_elements_in_arraylist_holder(&(al->arraylist_holder), non_NULL_elements_to_NULL_first_index, non_NULL_elements_to_NULL);

		// now we only need to fix the first_index and element_count of the arraylist

		// increment element_count
		al->element_count += NULL_count_to_insert;
	}
}

int insert_NULLs_from_front_in_arraylist(arraylist* al, cy_uint index, cy_uint NULL_count_to_insert)
{
	// fail if the index is not lesser than equal to element_count OR there isn't enough space to accomodate NULL_count_to_insert number of new NULLs
	if(index > get_element_count_arraylist(al) || NULL_count_to_insert > (get_capacity_arraylist(al) - get_element_count_arraylist(al)))
		return 0;

	// nothing to insert
	if(NULL_count_to_insert == 0)
		return 1;

	insert_NULLs_from_front_in_arraylist_INTERNAL(al, index, NULL_count_to_insert);

	return 1;
}

int insert_NULLs_from_back_in_arraylist(arraylist* al, cy_uint index, cy_uint NULL_count_to_insert)
{
	// fail if the index is not lesser than equal to element_count OR there isn't enough space to accomodate NULL_count_to_insert number of new NULLs
	if(index > get_element_count_arraylist(al) || NULL_count_to_insert > (get_capacity_arraylist(al) - get_element_count_arraylist(al)))
		return 0;

	// nothing to insert
	if(NULL_count_to_insert == 0)
		return 1;

	// compute corresponding front_index, to get the same result, as if done from front
	cy_uint front_index = get_element_count_arraylist(al) - index;

	insert_NULLs_from_front_in_arraylist_INTERNAL(al, front_index, NULL_count_to_insert);

	return 1;
}

// the below internal function assumes that
// index is valid, and there are atleast non-zero number of element_count_to_remove number of elements after it
static void remove_elements_from_front_of_arraylist_INTERNAL(arraylist* al, cy_uint index, cy_uint element_count_to_remove)
{
	// corresponding actual index of element at index from front
	cy_uint index_concerned = add_circularly(al->first_index, index, get_capacity_arraylist(al));

	// calculate the number of elements before and after the to-be removed elements
	cy_uint existing_elements_before_to_be_removed_ones = index;
	cy_uint existing_elements_after_to_be_removed_ones = al->element_count - (index + element_count_to_remove);

	if(existing_elements_before_to_be_removed_ones <= existing_elements_after_to_be_removed_ones) // move the front elements to the vacant positions
	{
		// move elements one by one
		cy_uint elements_to_be_moved = existing_elements_before_to_be_removed_ones;
		cy_uint index_to_move_from = index_concerned;
		cy_uint index_to_move_to = add_circularly(index_concerned, element_count_to_remove, get_capacity_arraylist(al));
		while(elements_to_be_moved > 0)
		{
			index_to_move_from = sub_circularly(index_to_move_from, 1, get_capacity_arraylist(al));
			index_to_move_to = sub_circularly(index_to_move_to, 1, get_capacity_arraylist(al));
			set_in_array(&(al->arraylist_holder), get_from_array(&(al->arraylist_holder), index_to_move_from), index_to_move_to);
			elements_to_be_moved--;
		}

		// reset `element_count_to_remove number` of elements to NULL from current first_index
		circularly_NULL_elements_in_arraylist_holder(&(al->arraylist_holder), al->first_index, element_count_to_remove);

		// handling post the movement of the front elements
		// decrement the element_count
		al->element_count -= element_count_to_remove;

		// if we had to remove all the elements then, reset the first_index
		if(al->element_count == 0)
			al->first_index = 0;
		else
			// move first_index forward by `element_count_to_remove` number of indices, and decrement the element_count
			al->first_index = add_circularly(al->first_index, element_count_to_remove, get_capacity_arraylist(al));
	}
	else // move the back elements to the vacant positions
	{
		// move elements one by one
		cy_uint elements_to_be_moved = existing_elements_after_to_be_removed_ones;
		cy_uint index_to_move_from = add_circularly(index_concerned, element_count_to_remove, get_capacity_arraylist(al));
		cy_uint index_to_move_to = index_concerned;
		while(elements_to_be_moved > 0)
		{
			set_in_array(&(al->arraylist_holder), get_from_array(&(al->arraylist_holder), index_to_move_from), index_to_move_to);
			index_to_move_from = add_circularly(index_to_move_from, 1, get_capacity_arraylist(al));
			index_to_move_to = add_circularly(index_to_move_to, 1, get_capacity_arraylist(al));
			elements_to_be_moved--;
		}

		// reset `element_count_to_remove` number of elements to NULL from the index = `index_concerned + existing_elements_after_to_be_removed_ones` (i.e. the index immediately after the new lst index)
		circularly_NULL_elements_in_arraylist_holder(&(al->arraylist_holder), get_end_index(index_concerned, existing_elements_after_to_be_removed_ones, get_capacity_arraylist(al)), element_count_to_remove);

		// handling post the movement of the back elements
		// decrement the element_count
		al->element_count -= element_count_to_remove;

		// if we had to remove all the elements then, reset the first_index
		if(al->element_count == 0)
			al->first_index = 0;
	}
}

int remove_elements_from_front_of_arraylist(arraylist* al, cy_uint index, cy_uint element_count_to_remove)
{
	// if the arraylist is empty OR the index is out of range OR there aren't enough elements (at and) after index
	// then we fail with 0
	if(is_empty_arraylist(al) || (index >= al->element_count) || ((al->element_count - index) < element_count_to_remove))
		return 0;

	// nothing to remove
	if(element_count_to_remove == 0)
		return 1;

	remove_elements_from_front_of_arraylist_INTERNAL(al, index, element_count_to_remove);

	return 1;
}

int remove_elements_from_back_of_arraylist(arraylist* al, cy_uint index, cy_uint element_count_to_remove)
{
	// if the arraylist is empty OR the index is out of range OR there aren't enough elements (at and) after index
	// then we fail with 0
	if(is_empty_arraylist(al) || (index >= al->element_count) || ((al->element_count - index) < element_count_to_remove))
		return 0;

	// nothing to remove
	if(element_count_to_remove == 0)
		return 1;

	// compute corresponding front_index, to get the same result, as if done from front
	cy_uint front_index = al->element_count - index - element_count_to_remove;

	remove_elements_from_front_of_arraylist_INTERNAL(al, front_index, element_count_to_remove);

	return 1;
}

cy_uint get_capacity_arraylist(const arraylist* al)
{
	return get_capacity_array(&(al->arraylist_holder));
}

cy_uint get_element_count_arraylist(const arraylist* al)
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

static void linearlize_arraylist_upon_expansion(arraylist* al, cy_uint old_capacity)
{
	// element_count remains the same, it is indifferent to re-linearizing the data in this function
	cy_uint element_count = get_element_count_arraylist(al);

	// quit if there are no elements to be linearized
	if(element_count == 0)
		return ;

	// this is the first index before we started our data management and linearization task
	cy_uint old_first_index = al->first_index;

	// check if the arraylist does need linearization
	// i.e. linearization is required only if the arraylist was wrapping arround its holder
	if(old_first_index <= get_last_index(old_first_index, element_count, old_capacity))
		return ;

	// reaching this place implies that the arraylist is wrapped around and has 2 parts: a head and a tail
	// lets calculate the number of elements in the head and tail
	cy_uint elements_in_head = old_capacity - old_first_index;   // elements from old_first_index until old_capacity - 1
	cy_uint elements_in_tail = element_count - elements_in_head; // elements from 0 to last_index == element_count - elements_in_head

	if(elements_in_head <= elements_in_tail)
	{
		// in this case
		// we move the elements in the head to the end of the arraylist holder

		// calculate the new first index
		cy_uint new_first_index = get_capacity_arraylist(al) - elements_in_head;

		// move all elements in head to the end of the arraylist holder
		copy_elements_from_array(&(al->arraylist_holder), new_first_index, &(al->arraylist_holder), old_first_index, elements_in_head);

		// mem set all old unused positions in the array as NULL (only if they previously were in use and are not in use now)
		cy_uint elements_to_NULL = new_first_index - old_first_index;
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
		cy_uint new_slots_added = get_capacity_arraylist(al) - old_capacity;

		cy_uint tail_elements_to_relocate = new_slots_added;
		if(elements_in_tail < new_slots_added)
			tail_elements_to_relocate = elements_in_tail;

		copy_elements_from_array(&(al->arraylist_holder), old_capacity, &(al->arraylist_holder), 0, tail_elements_to_relocate);

		cy_uint tail_elements_to_shift = elements_in_tail - tail_elements_to_relocate;
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
	cy_uint old_capacity = get_capacity_arraylist(al);

	// expand the holder fearlessly
	int has_holder_expanded = expand_array(&(al->arraylist_holder));

	// move data if necessary conditions meet
	if(data_movement_will_be_required && has_holder_expanded)
		linearlize_arraylist_upon_expansion(al, old_capacity);

	return has_holder_expanded;
}

int reserve_capacity_for_arraylist(arraylist* al, cy_uint atleast_capacity)
{
	int data_movement_will_be_required = 1;

	// on this condition, we can expand without any data movement
	if(is_empty_arraylist(al) || al->first_index <= get_last_index(al->first_index, al->element_count, get_capacity_arraylist(al)))
		data_movement_will_be_required = 0;

	// record old_capacity for future use
	cy_uint old_capacity = get_capacity_arraylist(al);

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
			copy_elements_from_array(&(al->arraylist_holder), 0, &(al->arraylist_holder), al->first_index, al->element_count);

			// update first_index of arraylist to 0
			al->first_index = 0;
		}

		has_holder_shrunk = shrink_array(&(al->arraylist_holder), al->element_count);
	}

	return has_holder_shrunk;
}

const void* find_equals_in_arraylist(const arraylist* al, const void* data, int (*compare)(const void* data1, const void* data2))
{
	for(cy_uint i = 0; i < al->element_count; i++)
	{
		cy_uint al_index = add_circularly(al->first_index, i, get_capacity_arraylist(al));
		const void* found = get_from_array(&(al->arraylist_holder), al_index);
		if(0 == compare(found, data))
			return found;
	}

	return NULL;
}

void for_each_in_arraylist(const arraylist* al, void (*operation)(void* data_p, cy_uint index, const void* additional_params), const void* additional_params)
{
	for_each_non_null_in_array(&(al->arraylist_holder), operation, additional_params);
}

void sprint_arraylist(dstring* append_str, const arraylist* al, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs++); snprintf_dstring(append_str, "arraylist :\n");
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "first_index : %" PRIu_cy_uint "\n", al->first_index);
	sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "element_count : %" PRIu_cy_uint "\n", al->element_count);
	
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
	// NULL all existing elements in arraylist
	// we use the below function to selectively NULL only the indexes at which elements existed
	circularly_NULL_elements_in_arraylist_holder(&(al->arraylist_holder), al->first_index, al->element_count);

	// reset arraylist
	al->first_index = 0;
	al->element_count = 0;
}

void deinitialize_arraylist(arraylist* al)
{
	deinitialize_array(&(al->arraylist_holder));
	al->first_index = 0;
	al->element_count = 0;
}

index_accessed_interface get_index_accessed_interface_for_front_of_arraylist(arraylist* al)
{
	return (index_accessed_interface){
										.ds_p = al,
										.get_element = (const void* (*)(const void*, cy_uint))get_from_front_of_arraylist,
										.set_element = (int (*)(void*, const void*, cy_uint))set_from_front_in_arraylist,
										.swap_elements = (int (*)(void*, cy_uint, cy_uint))swap_from_front_in_arraylist,
										.get_element_count = (cy_uint (*)(const void*))get_element_count_arraylist,
									};
}

index_accessed_interface get_index_accessed_interface_for_back_of_arraylist(arraylist* al)
{
	return (index_accessed_interface){
										.ds_p = al,
										.get_element = (const void* (*)(const void*, cy_uint))get_from_back_of_arraylist,
										.set_element = (int (*)(void*, const void*, cy_uint))set_from_back_in_arraylist,
										.swap_elements = (int (*)(void*, cy_uint, cy_uint))swap_from_back_in_arraylist,
										.get_element_count = (cy_uint (*)(const void*))get_element_count_arraylist,
									};
}

int get_slice_as_arraylist_from_array(arraylist* slice, array* array_p, cy_uint first_index, cy_uint element_count)
{
	// first_index must be a valid array index, and element_count must be lesser than the capacity of the array
	if(first_index >= get_capacity_array(array_p) || element_count > get_capacity_array(array_p))
		return 0;

	// initialize arraylist pointing to the same memory contents as that of the array
	if(!initialize_arraylist_with_memory(slice, get_capacity_array(array_p), array_p->data_p_p))
		return 0;

	// blindly make the arraylist point to it
	slice->first_index = first_index;
	slice->element_count = element_count;

	return 1;
}