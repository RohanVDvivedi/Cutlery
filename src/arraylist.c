#include<arraylist.h>

void intialize_arraylist(arraylist* al, unsigned int initial_size)
{
	initialize_array(&(al->arraylist_holder), initial_size);
	al->first_index = 0;
	al->element_count = 0;
}

int push_front(arraylist* al, const void* data_p);

int push_back(arraylist* al, const void* data_p);

int pop_front(arraylist* al);

int pop_back(arraylist* al);

const void* get_front(const arraylist* al);

const void* get_back(const arraylist* al);

const void* get_nth_from_front(const arraylist* al, unsigned int n);

const void* get_nth_from_back(const arraylist* al, unsigned int n);

unsigned int get_arraylist_element_count(const arraylist* al)
{
	return al->element_count; 
}

int is_arraylist_full(const arraylist* al);

int is_arraylist_empty(const arraylist* al);

int expand_arraylist(arraylist* al);

int shrink_arraylist(arraylist* al);

const void* find_equals_in_arraylist(const arraylist* ll, const void* data, int (*compare)(const void* al_data, const void* data));

void for_each_in_arraylist(const arraylist* al, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params);

void print_arraylist(const arraylist* al);

void deintialize_arraylist(arraylist* al)
{
	deinitialize_array(&(al->arraylist_holder));
}