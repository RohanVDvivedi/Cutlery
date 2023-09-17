#include<arraylist_sort.h>

#include<heap.h>
#include<circular_buffer_array_util.h>

int heap_sort_arraylist(arraylist* al, cy_uint start_index, cy_uint last_index, const comparator_interface* comparator)
{
	if(start_index > last_index || last_index >= get_element_count_arraylist(al))
		return 0;

	// compute the number of elements to sort; 0 or 1 number of elements do not need sorting
	cy_uint total_elements = last_index - start_index + 1;
	if(total_elements <= 1)
		return 1;

	// create a max heap that points to the array_p's contents that need to be sorted
	heap sort_heap;
	initialize_heap_with_memory(&sort_heap, get_capacity_arraylist(al), MAX_HEAP, BINARY_HEAP_DEGREE, comparator, NO_HEAP_NODE_OFFSET, al->arraylist_holder.data_p_p);
	sort_heap.heap_holder.first_index = add_circularly(al->first_index, start_index, get_capacity_arraylist(al));
	sort_heap.heap_holder.element_count = total_elements;

	// now max heapify all elements that we need to sort
	heapify_all(&sort_heap);

	// place the top of the heap element to the beginning of the sorted region of the arraylist, then pop heap
	for(cy_uint i = total_elements; i > 1; i--)
	{
		const void* max_data = get_top_of_heap(&sort_heap);
		pop_from_heap(&sort_heap);
		set_in_arraylist(al, max_data, start_index + i - 1);
	}

	// deinitialize the sort_heap
	deinitialize_heap(&sort_heap);

	return 1;
}