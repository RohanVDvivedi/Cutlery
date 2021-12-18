#include<array.h>
#include<heap.h>
#include<queue.h>

#include<cutlery_stds.h>
#include<memory_allocator_interface.h>

void merge_sort_array(array* array_p, unsigned int start_index, unsigned int last_index, int (*compare)(const void* data1, const void* data2))
{
	if(start_index > last_index || last_index >= array_p->capacity)
		return;

	// compute the number of elements to sort; 0 or 1 number of elements do not need sorting
	unsigned int total_elements = last_index - start_index + 1;
	if(total_elements <= 1)
		return;

	// we iteratively merge adjacent sorted chunks from src and store them in dest
	const void** src  = array_p->data_p_p + start_index;
	const void** dest = allocate(array_p->mem_allocator, sizeof(void*) * total_elements);

	// start with sorted chunk size equals 1, (a single element is always sorted)
	unsigned int sort_chunk_size = 1;
	while(sort_chunk_size <= total_elements)
	{

		// in each iteration of the internal loop
		// merge 2 adjacent sorted chunks of src array
		// to form 1 chunk of twice the size in dest array
		unsigned int dest_index = 0;
		while(dest_index < total_elements)
		{
			// start and end indices of chunk 1
			unsigned int a_start = dest_index;
			unsigned int a_end = a_start + sort_chunk_size - 1;

			// start and end indices of chunk 2
			unsigned int b_start = a_end + 1;
			unsigned int b_end = b_start + sort_chunk_size - 1;

			// *_start and *_end are both inclusive indices

			if(b_start > total_elements - 1)
			{
				if(a_end > total_elements - 1)
					a_end = total_elements - 1;

				memory_move(dest + dest_index, src + a_start, (a_end - a_start + 1) * sizeof(void*));
				break;
			}
			else
			{
				if(b_end > total_elements - 1)
					b_end = total_elements - 1;

				while(dest_index <= b_end)
				{
					if((b_start > b_end) || (a_start <= a_end && compare(src[a_start], src[b_start]) < 0))
						dest[dest_index++] = src[a_start++];
					else
						dest[dest_index++] = src[b_start++];
				}
			}
		}

		// src becomes dest, and dest becomes src
		const void** temp = src;
		src = dest;
		dest = temp;

		// double the chunk size, for next iteration
		sort_chunk_size = sort_chunk_size * 2;
	}

	// free the extra memory
	if((array_p->data_p_p + start_index) == src)
		deallocate(array_p->mem_allocator, dest, sizeof(void*) * total_elements);
	else
	{
		memory_move(array_p->data_p_p + start_index, src, total_elements * sizeof(void*));
		deallocate(array_p->mem_allocator, src, sizeof(void*) * total_elements);
	}
}

void heap_sort_array(array* array_p, unsigned int start_index, unsigned int last_index, int (*compare)(const void* data1, const void* data2))
{
	if(start_index > last_index || last_index >= array_p->capacity)
		return;

	// compute the number of elements to sort; 0 or 1 number of elements do not need sorting
	unsigned int total_elements = last_index - start_index + 1;
	if(total_elements <= 1)
		return;

	// create a max heap that points to the array_p's contents that need to be sorted
	heap sort_heap;
	initialize_heap_with_allocator(&sort_heap, 0, MAX_HEAP, compare, NULL, NULL, NULL);
	sort_heap.heap_holder.data_p_p = array_p->data_p_p + start_index;
	sort_heap.heap_holder.capacity = total_elements;
	sort_heap.element_count = total_elements;

	// now max heapify all elements that we need to sort
	heapify_all(&sort_heap);

	// place the top of the heap element in the array, then pop heap
	for(unsigned int i = total_elements; i > 0; i--)
	{
		const void* max_data = get_top_heap(&sort_heap);
		pop_heap(&sort_heap);
		set_element(array_p, max_data, start_index + i - 1);
	}

	// deinitialize the sort_heap
	deinitialize_heap(&sort_heap);
}

void quick_sort_array(array* array_p, unsigned int start_index, unsigned int last_index, int (*compare)(const void* data1, const void* data2))
{
	if(start_index > last_index || last_index >= array_p->capacity)
		return;

	// compute the number of elements to sort; 0 or 1 number of elements do not need sorting
	unsigned int total_elements = last_index - start_index + 1;
	if(total_elements <= 1)
		return;

	// always picking the last element as the pivot
	const void* pivot = get_element(array_p, last_index);

	unsigned int all_greater_than_pivot_start_index = start_index;

	// position pivot element at its correct index
	for(unsigned int i = start_index; i <= last_index; i++)
	{
		if(compare(get_element(array_p, i), pivot) <= 0)
			swap_elements(array_p, all_greater_than_pivot_start_index++, i);
	}

	// index of the pivot element
	unsigned int pivot_index = all_greater_than_pivot_start_index - 1;

	// perform recursive quick sort on the smaller arrays excluding the pivot element
	if(pivot_index > start_index)
		quick_sort_array(array_p, start_index, pivot_index - 1, compare);
	if(pivot_index < last_index)
		quick_sort_array(array_p, pivot_index + 1, last_index, compare);
}

void radix_sort_array(array* array_p, unsigned int start_index, unsigned int last_index, unsigned int (*get_sort_attribute)(const void* data))
{
	if(start_index > last_index || last_index >= array_p->capacity)
		return;

	// compute the number of elements to sort; 0 or 1 number of elements do not need sorting
	unsigned int total_elements = last_index - start_index + 1;
	if(total_elements <= 1)
		return;

	// construct temporary queues for 0 and 1 bit containing elements
	queue sort_queue[2];
	initialize_queue_with_allocator(&(sort_queue[0]), total_elements, array_p->mem_allocator);
	initialize_queue_with_allocator(&(sort_queue[1]), total_elements, array_p->mem_allocator);

	for(unsigned int i = 0; i < 32; i++)
	{
		unsigned index = start_index;
		while(index <= last_index)
		{
			const void* data = get_element(array_p, index++);
			unsigned int queue_index = (get_sort_attribute(data) >> i) & 1;
			push_queue(&(sort_queue[queue_index]), data);
		}

		index = start_index;
		while(!is_empty_queue(&(sort_queue[0])))
		{
			const void* data = get_top_queue(&(sort_queue[0]));
			set_element(array_p, data, index++);
			pop_queue(&(sort_queue[0]));
		}
		while(!is_empty_queue(&(sort_queue[1])))
		{
			const void* data = get_top_queue(&(sort_queue[1]));
			set_element(array_p, data, index++);
			pop_queue(&(sort_queue[1]));
		}
	}

	deinitialize_queue(&(sort_queue[0]));
	deinitialize_queue(&(sort_queue[1]));
}


unsigned int linear_search_in_array(const array* array_p, unsigned int start_index, unsigned int last_index, const void* data_p, int (*compare)(const void* data1, const void* data2), search_occurence occurence_type)
{
	// check for valid start and end indexes
	if(start_index > last_index || last_index >= array_p->capacity)
		return INVALID_INDEX;

	switch(occurence_type)
	{
		case FIRST_OCCURENCE:
		{
			for(unsigned int i = start_index; i <= last_index; i++)
			{
				if(compare(get_element(array_p, i), data_p) == 0)
					return i;
			}
			break;
		}
		case LAST_OCCURENCE:
		{
			for(unsigned int i = last_index; ; i--)
			{
				if(compare(get_element(array_p, i), data_p) == 0)
					return i;
				if(i == start_index)
					break;
			}
			break;
		}
	}

	// we return answer or return an element out of bounds
	return INVALID_INDEX;
}

unsigned int binary_search_in_sorted_array(const array* array_p, unsigned int start_index, unsigned int last_index, const void* data_p, int (*compare)(const void* data1, const void* data2), search_occurence occurence_type)
{
	// check for valid start and end indexes
	if(start_index > last_index || last_index >= array_p->capacity)
		return INVALID_INDEX;

	// if the element is lesser than the element at the start_index
	// OR is greater than the element at the last_index, then return INVALID_INDEX
	if(compare(get_element(array_p, start_index), data_p) > 0 || compare(get_element(array_p, last_index), data_p) < 0)
		return INVALID_INDEX;

	// binary search low and high range variables
	unsigned int l = start_index;
	unsigned int h = last_index;

	// result from performing binary search
	unsigned int result_index = INVALID_INDEX;

	// perform binary search for first or last occurence
	while(l <= h)
	{
		unsigned int m = l + ((h - l) / 2);
		if(compare(get_element(array_p, m), data_p) > 0)
			h = m - 1;
		else if(compare(get_element(array_p, m), data_p) < 0)
			l = m + 1;
		else
		{
			int break_loop = 0;
			switch(occurence_type)
			{
				case FIRST_OCCURENCE:
				{
					result_index = m;
					if(m == start_index)
						break_loop = 1;
					else
						h = m - 1;
					break;
				}
				case LAST_OCCURENCE:
				{
					result_index = m;
					if(m == last_index)
						break_loop = 1;
					else
						l = m + 1;
					break;
				}
			}
			if(break_loop)
				break;
		}
	}

	return result_index;
}

unsigned int find_insertion_index_in_sorted_array(const array* array_p, unsigned int start_index, unsigned int last_index, const void* data_p, int (*compare)(const void* data1, const void* data2))
{
	// check for valid start and end indexes
	if(start_index > last_index || last_index >= array_p->capacity)
		return INVALID_INDEX;

	// if the element is lesser than the element at the start_index, then return start_index
	if(compare(get_element(array_p, start_index), data_p) > 0)
		return start_index;

	// binary search low and high range variables
	unsigned int l = start_index;
	unsigned int h = last_index + 1;

	// result from performing binary search
	unsigned int result_index = INVALID_INDEX;

	// perform binary search for first or last occurence
	while(l <= h)
	{
		unsigned int m = l + ((h - l) / 2);

		// if the mid has crossed the last_index,
		// then the last element is more than or equal to the data,
		// that we are comparing with
		if(m == last_index + 1)
		{
			result_index = m;
			break;
		}

		// if the element at m in the array is lesser,
		// then update the result_index (since it could be the answer) and shorten the search range
		if(compare(get_element(array_p, m), data_p) > 0)
		{
			result_index = m;
			h = m - 1;
		}
		// if the element at m in the array is more than or equal to given parameter (data),
		// then just shorten the search range on the left, since no element at index lesser than m can be the answer
		else
			l = m + 1;
	}

	return result_index;
}