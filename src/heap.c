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

void push(heap* heap_p, void* key, void* value)
{

}

const void* get_top(heap* heap_p)
{
	return NULL;
}

void pop(heap* heap_p)
{

}

void delete_bucket_wrapper(void* data_p, unsigned long long int index, const void* additional_params)
{
	delete_bucket(data_p);
}

void delete_heap(heap* heap_p)
{
	for_each_in_array(heap_p->heap_holder, delete_bucket_wrapper, NULL);
	delete_array(heap_p->heap_holder);
	free(heap_p);
}