#include<arraylist.h>

#include<stdio.h>

void initialize_arraylist(arraylist* al, unsigned int initial_size)
{
	initialize_array(&(al->arraylist_holder), initial_size);
	al->first_index = 0;
	al->element_count = 0;
}

int push_front(arraylist* al, const void* data_p)
{
	// if full, you can't push to arraylist
	if(is_arraylist_full(al))
		return 0;

	// update the first_index, to its new place where it will hold the new element
	// if empty push the element at 0th index
	if(is_arraylist_empty(al))
		al->first_index = 0;
	else // else an index prior to the first index in the circular index scheme
		al->first_index = ((al->first_index + al->arraylist_holder.total_size) - 1) % al->arraylist_holder.total_size;

	// push to front of array list
	set_element(&(al->arraylist_holder), data_p, al->first_index);

	// increment the element counter
	al->element_count++;

	return 1;
}

int push_back(arraylist* al, const void* data_p)
{
	// if full, you can't push to arraylist
	if(is_arraylist_full(al))
		return 0;

	// if empty push the element at 0th index
	if(is_arraylist_empty(al))
		al->first_index = 0;

	// end_index is the index to the position on the circular buffer, that is immediately after the last element
	unsigned int end_index = (al->first_index + al->element_count) % al->arraylist_holder.total_size;

	// push to back of array list
	set_element(&(al->arraylist_holder), data_p, end_index);

	// increment the element counter
	al->element_count++;

	return 1;
}

int pop_front(arraylist* al)
{
	// if empty you can't pop from the arraylist
	if(is_arraylist_empty(al))
		return 0;

	// pop an element from front of the arraylist
	set_element(&(al->arraylist_holder), NULL, al->first_index);

	// update the first index
	al->first_index = (al->first_index + 1) % al->arraylist_holder.total_size;

	// decrement the element counter
	al->element_count--;

	return 1;
}

int pop_back(arraylist* al)
{
	// if empty you can't pop from the arraylist
	if(is_arraylist_empty(al))
		return 0;

	// find the index to the last element in the arraylist
	unsigned int back_index = ((al->first_index + al->element_count) - 1) % al->arraylist_holder.total_size;

	// pop an element from front of the arraylist
	set_element(&(al->arraylist_holder), NULL, back_index);

	// decrement the element counter
	al->element_count--;

	return 1;
}

const void* get_front(const arraylist* al)
{
	// if empty, return NULL
	if(is_arraylist_empty(al))
		return NULL;

	// find front element of the arraylist, and return it
	return get_element(&(al->arraylist_holder), al->first_index);
}

const void* get_back(const arraylist* al)
{
	// if empty, return NULL
	if(is_arraylist_empty(al))
		return NULL;

	// find back element of the arraylist
	return get_element(&(al->arraylist_holder), (al->first_index + al->element_count - 1) % al->arraylist_holder.total_size);
}

const void* get_nth_from_front(const arraylist* al, unsigned int n)
{
	// arraylist must not be empty and the index-n must be lesser than the element-count
	if(is_arraylist_empty(al) || n >= al->element_count)
		return NULL;

	// find nth element from front of the arraylist, and return it
	return get_element(&(al->arraylist_holder), (al->first_index + n) % al->arraylist_holder.total_size);
}

const void* get_nth_from_back(const arraylist* al, unsigned int n)
{
	// arraylist must not be empty and the index-n must be lesser than the element-count
	if(is_arraylist_empty(al) || n >= al->element_count)
		return NULL;

	// find nth element from back of the arraylist, and return it
	return get_element(&(al->arraylist_holder), (((al->first_index + al->element_count) - 1) - n) % al->arraylist_holder.total_size);
}

unsigned int get_total_size_arraylist(arraylist* al)
{
	return al->arraylist_holder.total_size;
}

unsigned int get_element_count_arraylist(arraylist* al)
{
	return al->element_count;
}

int is_arraylist_full(const arraylist* al)
{
	return al->element_count == al->arraylist_holder.total_size;
}

int is_arraylist_empty(const arraylist* al)
{
	return al->element_count == 0;
}

int expand_arraylist(arraylist* al)
{
	int data_movement_will_be_required = 1;

	// on this condition, we can expand without any data movement
	if(is_arraylist_empty(al) || (al->first_index + al->element_count) <= al->arraylist_holder.total_size)
		data_movement_will_be_required = 0;

	// record total size and first index for further use
	unsigned int old_first_index = al->first_index;
	unsigned int old_total_size = al->arraylist_holder.total_size;

	// expand the holder fearlessly
	int has_holder_expanded = expand_array(&(al->arraylist_holder));

	// move data if necessary conditions meet
	if(data_movement_will_be_required && has_holder_expanded)
	{
		// move partial data, that was at the end of the array
		unsigned int elements_to_move = old_total_size - old_first_index;

		// calculate the new first index
		unsigned int new_first_index = al->arraylist_holder.total_size - elements_to_move;

		// move data
		memory_move(al->arraylist_holder.data_p_p + new_first_index,
					al->arraylist_holder.data_p_p + old_first_index,
					elements_to_move * sizeof(void*));

		// update the new first_index
		al->first_index = new_first_index;
	}

	return has_holder_expanded;
}

int shrink_arraylist(arraylist* al)
{
	int has_holder_shrunk = 0;

	// to be able to shrink an array, it must have a non-zero total size
	if((al->arraylist_holder.total_size > 0) && (is_arraylist_empty(al) || (al->first_index + al->element_count) <= al->arraylist_holder.total_size))
		has_holder_shrunk = shrink_array(&(al->arraylist_holder), al->first_index, al->first_index + al->element_count - 1);

	// if the arraylist_holder had shrunk, the new first_index has to be at 0ss
	if(has_holder_shrunk)
		al->first_index = 0;

	return has_holder_shrunk;
}

const void* find_equals_in_arraylist(const arraylist* al, const void* data, int (*compare)(const void* al_data, const void* data))
{
	for(unsigned int i = 0, index = al->first_index; i < al->element_count; i++, index++)
	{
		const void* found = get_element(&(al->arraylist_holder), index % al->arraylist_holder.total_size);
		if(0 == compare(found, data))
			return found;
	}

	return NULL;
}

void for_each_in_arraylist(const arraylist* al, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params)
{
	for_each_non_null_in_array(&(al->arraylist_holder), operation, additional_params);
}

void print_arraylist(const arraylist* al, void (*print_element)(const void* data_p))
{
	printf("arraylist : \n");
	printf("\tfirst_index : %u\n", al->first_index);
	printf("\telement_count : %u\n", al->element_count);
	printf("\tarraylist_holder : ");print_array(&(al->arraylist_holder), print_element);printf("\n");
	
	const void* front_element = get_front(al);
	printf("\tthe front element : ");
	(front_element != NULL) ? print_element(front_element) : printf("NULL");
	printf("\n");

	const void* back_element = get_back(al);
	printf("\tthe back element : ");
	(back_element != NULL) ? print_element(back_element) : printf("NULL");
	printf("\n");
}

void deinitialize_arraylist(arraylist* al)
{
	deinitialize_array(&(al->arraylist_holder));
}