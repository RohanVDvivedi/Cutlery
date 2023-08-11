#include<arraylist_sort.h>

#include<heap.h>

int heap_sort_arraylist(arraylist* al, cy_uint start_index, cy_uint last_index, int (*compare)(const void* data1, const void* data2));

/*
int heap_sort_array(array* array_p, cy_uint start_index, cy_uint last_index, int (*compare)(const void* data1, const void* data2))
{
	if(start_index > last_index || last_index >= get_capacity_array(array_p))
		return 0;

	// compute the number of elements to sort; 0 or 1 number of elements do not need sorting
	cy_uint total_elements = last_index - start_index + 1;
	if(total_elements <= 1)
		return 1;

	// create a max heap that points to the array_p's contents that need to be sorted
	heap sort_heap;
	initialize_heap_with_memory(&sort_heap, total_elements, MAX_HEAP, BINARY_HEAP_DEGREE, compare, NO_HEAP_NODE_OFFSET, array_p->data_p_p + start_index);
	sort_heap.element_count = total_elements;

	// now max heapify all elements that we need to sort
	heapify_all(&sort_heap);

	// place the top of the heap element in the array, then pop heap
	for(cy_uint i = total_elements; i > 0; i--)
	{
		const void* max_data = get_top_of_heap(&sort_heap);
		pop_from_heap(&sort_heap);
		set_in_array(array_p, max_data, start_index + i - 1);
	}

	// deinitialize the sort_heap
	deinitialize_heap(&sort_heap);

	return 1;
}
*/