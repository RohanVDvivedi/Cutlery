#include<heap.h>

void* get_heap(unsigned long long int expected_size, heap_type type, int (*key_compare)(const void* key0, const void* key1))
{
	heap* heap_p = ((heap*)(malloc(sizeof(heap))));
	heap_p->type = type;
	heap_p->key_compare = key_compare;
	heap_p->heap_size = 0;
	heap_p->heap_holder = get_array(expected_size);
	return heap_p;
}

// utility : gets index of the bucket, that is the parent to the given bucket at index = child
unsigned long long int get_parent_index(unsigned long long int child)
{
	return (child-1)/2;
}

// utility : gets index of the bucket, that is the left child to the given parent bucket at index = child
unsigned long long int get_left_child_index(unsigned long long int parent)
{
	return (2 * parent) + 1;
}

// utility : gets index of the bucket, that is the right child to the given parent bucket at index = child
unsigned long long int get_right_child_index(unsigned long long int parent)
{
	return (2 * parent) + 2;
}

void push(heap* heap_p, void* key, void* value)
{

}

const void* get_top(heap* heap_p)
{
	if(heap_p->heap_size == 0)
	{
		return NULL;
	}
	return ((bucket*)get_element(heap_p->heap_holder, 0))->value;
}

void pop(heap* heap_p)
{

}

void delete_bucket_wrapper(void* data_p, unsigned long long int index, const void* additional_params)
{
	if(data_p != NULL)
	{
		delete_bucket(data_p);
	}
}

void delete_heap(heap* heap_p)
{
	for_each_in_array(heap_p->heap_holder, delete_bucket_wrapper, NULL);
	delete_array(heap_p->heap_holder);
	free(heap_p);
}