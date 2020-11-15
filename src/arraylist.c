#include<arraylist.h>

void intialize_arraylist(arraylist* al, unsigned int initial_size)
{
	initialize_array(&(al->arraylist_holder), initial_size);
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
		al->first_index = ((al->first_index + al->arraylist_holder.total_size) - 1);

	// push to front of array list
	set_element(&(al->arraylist_holder), data_p, (al->first_index % al->arraylist_holder.total_size));

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
	unsigned int end_index = (al->first_index + al->element_count + al->arraylist_holder.total_size);

	// push to back of array list
	set_element(&(al->arraylist_holder), data_p, (end_index % al->arraylist_holder.total_size));

	// increment the element counter
	al->element_count++;

	return 1;
}

int pop_front(arraylist* al)
{
	// if empty you can't pop from the arraylist
	if(is_arraylist_empty(al))
		return 0;

	// pop an element from front of the arraylist - TODO

	return 1;
}

int pop_back(arraylist* al)
{
	// if empty you can't pop from the arraylist
	if(is_arraylist_empty(al))
		return 0;

	// pop an element from back of the arraylist - TODO

	return 1;
}

const void* get_front(const arraylist* al)
{
	// if empty, return NULL
	if(is_arraylist_empty(al))
		return NULL;

	// find front element of the arraylist - TODO

	return 1;
}

const void* get_back(const arraylist* al)
{
	// if empty, return NULL
	if(is_arraylist_empty(al))
		return NULL;

	// find back element of the arraylist - TODO

	return 1;
}

const void* get_nth_from_front(const arraylist* al, unsigned int n)
{
	// if there are lesser than n elements return NULL
	if(get_arraylist_element_count(al) < n)
		return NULL;

	// find nth element from front of the arraylist - TODO

	return 1;
}

const void* get_nth_from_back(const arraylist* al, unsigned int n)
{
	// if there are lesser than n elements return NULL
	if(get_arraylist_element_count(al) < n)
		return NULL;

	// find nth element from back of the arraylist - TODO

	return 1;
}

unsigned int get_arraylist_element_count(const arraylist* al)
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

int expand_arraylist(arraylist* al);

int shrink_arraylist(arraylist* al);

const void* find_equals_in_arraylist(const arraylist* ll, const void* data, int (*compare)(const void* al_data, const void* data));

void for_each_in_arraylist(const arraylist* al, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params)
{
	for_each_non_null_in_array(&(al->arraylist_holder), operation, additional_params);
}

void print_arraylist(const arraylist* al);

void deintialize_arraylist(arraylist* al)
{
	deinitialize_array(&(al->arraylist_holder));
}