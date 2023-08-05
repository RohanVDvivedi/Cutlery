#include<array.h>
// TODO this needs to be swapped
#include<indexed_search_sort.h>

#include<heap.h>
#include<arraylist.h>

#include<cutlery_stds.h>
#include<memory_allocator_interface.h>

int merge_sort_array(array* array_p, cy_uint start_index, cy_uint last_index, int (*compare)(const void* data1, const void* data2))
{
	if(start_index > last_index || last_index >= get_capacity_array(array_p))
		return 0;

	// compute the number of elements to sort; 0 or 1 number of elements do not need sorting
	cy_uint total_elements = last_index - start_index + 1;
	if(total_elements <= 1)
		return 1;

	// we iteratively merge adjacent sorted chunks from src and store them in dest
	const void** src  = array_p->data_p_p + start_index;

	cy_uint dest_bytes = sizeof(void*) * total_elements;
	const void** dest = aligned_allocate(array_p->mem_allocator, &dest_bytes, _Alignof(void*));
	if(dest == NULL)
		return 0;

	// start with sorted chunk size equals 1, (a single element is always sorted)
	cy_uint sort_chunk_size = 1;
	while(sort_chunk_size <= total_elements)
	{

		// in each iteration of the internal loop
		// merge 2 adjacent sorted chunks of src array
		// to form 1 chunk of twice the size in dest array
		cy_uint dest_index = 0;
		while(dest_index < total_elements)
		{
			// start and last indices of chunk 1
			cy_uint a_start = dest_index;
			cy_uint a_last = a_start + sort_chunk_size - 1;

			// start and last indices of chunk 2
			cy_uint b_start = a_last + 1;
			cy_uint b_last = b_start + sort_chunk_size - 1;

			// *_start and *_last are both inclusive indices

			if(b_start > total_elements - 1)
			{
				if(a_last > total_elements - 1)
					a_last = total_elements - 1;

				memory_move(dest + dest_index, src + a_start, (a_last - a_start + 1) * sizeof(void*));
				break;
			}
			else
			{
				if(b_last > total_elements - 1)
					b_last = total_elements - 1;

				while(dest_index <= b_last)
				{
					if((b_start > b_last) || (a_start <= a_last && compare(src[a_start], src[b_start]) < 0))
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
		deallocate(array_p->mem_allocator, dest, dest_bytes);
	else
	{
		memory_move(array_p->data_p_p + start_index, src, total_elements * sizeof(void*));
		deallocate(array_p->mem_allocator, src, dest_bytes);
	}

	return 1;
}

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

int quick_sort_iai(index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, int (*compare)(const void* data1, const void* data2))
{
	while(1)
	{
		if(start_index > last_index || last_index >= iai_p->get_element_count(iai_p->ds_p))
			return 0;

		// compute the number of elements to sort; 0 or 1 number of elements do not need sorting
		cy_uint total_elements = last_index - start_index + 1;
		if(total_elements <= 1)
			return 1;

		// always picking the last element as the pivot
		const void* pivot = iai_p->get_element(iai_p->ds_p, last_index);

		cy_uint all_greater_than_pivot_start_index = start_index;

		// position pivot element at its correct index
		for(cy_uint i = start_index; i <= last_index; i++)
		{
			if(compare(iai_p->get_element(iai_p->ds_p, i), pivot) <= 0)
				iai_p->swap_elements(iai_p->ds_p, all_greater_than_pivot_start_index++, i);
		}

		// index of the pivot element
		cy_uint pivot_index = all_greater_than_pivot_start_index - 1;

		cy_uint size_of_part_before_pivot = pivot_index - start_index;
		cy_uint size_of_part_after_pivot = last_index - pivot_index;

		if(size_of_part_before_pivot < size_of_part_after_pivot)
		{
			// recurse for before part and loop for after part, in order to keep stack usage to minimum
			if(size_of_part_before_pivot > 0)
				quick_sort_iai(iai_p, start_index, pivot_index - 1, compare);

			// no elements after pivot to loop for
			if(size_of_part_after_pivot == 0)
				return 1;

			start_index = pivot_index + 1;
		}
		else
		{
			// recurse for after part and loop for before part, in order to keep stack usage to minimum
			if(size_of_part_after_pivot > 0)
				quick_sort_iai(iai_p, pivot_index + 1, last_index, compare);

			// no elements before pivot to loop for
			if(size_of_part_before_pivot == 0)
				return 1;

			last_index = pivot_index - 1;
		}
	}
}

int radix_sort_iai(index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, unsigned long long int (*get_sort_attribute)(const void* data), memory_allocator mem_allocator)
{
	if(mem_allocator == NULL || start_index > last_index || last_index >= iai_p->get_element_count(iai_p->ds_p))
		return 0;

	// compute the number of elements to sort; 0 or 1 number of elements do not need sorting
	cy_uint total_elements = last_index - start_index + 1;
	if(total_elements <= 1)
		return 1;

	// construct temporary queues for 0 and 1 bit containing elements
	arraylist sort_queue[2];
	if(!initialize_arraylist_with_allocator(&(sort_queue[0]), total_elements, mem_allocator))
		return 0;
	if(!initialize_arraylist_with_allocator(&(sort_queue[1]), total_elements, mem_allocator))
	{
		deinitialize_arraylist(&(sort_queue[0]));
		return 0;
	}

	for(cy_uint i = 0; i < (sizeof(unsigned long long int) * CHAR_BIT); i++)
	{
		cy_uint index = start_index;
		while(index <= last_index)
		{
			const void* data = iai_p->get_element(iai_p->ds_p, index++);
			unsigned int queue_index = ((get_sort_attribute(data) >> i) & 1ULL);
			push_back_to_arraylist(&(sort_queue[queue_index]), data);
		}

		index = start_index;
		while(!is_empty_arraylist(&(sort_queue[0])))
		{
			const void* data = get_front_of_arraylist(&(sort_queue[0]));
			pop_front_from_arraylist(&(sort_queue[0]));
			iai_p->set_element(iai_p->ds_p, data, index++);
		}
		while(!is_empty_arraylist(&(sort_queue[1])))
		{
			const void* data = get_front_of_arraylist(&(sort_queue[1]));
			pop_front_from_arraylist(&(sort_queue[1]));
			iai_p->set_element(iai_p->ds_p, data, index++);
		}
	}

	deinitialize_arraylist(&(sort_queue[0]));
	deinitialize_arraylist(&(sort_queue[1]));

	return 1;
}


cy_uint linear_search_in_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, int (*compare)(const void* data1, const void* data2), search_occurence occurence_type)
{
	// check for valid start and last indexes
	if(start_index > last_index || last_index >= iai_p->get_element_count(iai_p->ds_p))
		return INVALID_INDEX;

	switch(occurence_type)
	{
		case FIRST_OCCURENCE:
		{
			for(cy_uint i = start_index; i <= last_index; i++)
			{
				if(compare(iai_p->get_element(iai_p->ds_p, i), data_p) == 0)
					return i;
			}
			break;
		}
		case LAST_OCCURENCE:
		{
			for(cy_uint i = last_index; i != (start_index - 1); i--)
			{
				if(compare(iai_p->get_element(iai_p->ds_p, i), data_p) == 0)
					return i;
			}
			break;
		}
	}

	// we return answer or return an element out of bounds
	return INVALID_INDEX;
}

cy_uint binary_search_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, int (*compare)(const void* data1, const void* data2), search_occurence occurence_type)
{
	// check for valid start and last indexes
	if(start_index > last_index || last_index >= iai_p->get_element_count(iai_p->ds_p))
		return INVALID_INDEX;

	switch(occurence_type)
	{
		case FIRST_OCCURENCE :
		{
			// take care of conditions when we might go start_index - 1
			if(compare(iai_p->get_element(iai_p->ds_p, start_index), data_p) == 0)
				return start_index;
			else if(compare(iai_p->get_element(iai_p->ds_p, start_index), data_p) > 0)
				return INVALID_INDEX;
			break;
		}
		case LAST_OCCURENCE :
		{
			// take care of conditions when we might go start_index - 1
			if(compare(iai_p->get_element(iai_p->ds_p, start_index), data_p) > 0)
				return INVALID_INDEX;
			break;
		}
	}

	// binary search low and high range variables
	cy_uint l = start_index;
	cy_uint h = last_index;

	// result from performing binary search
	cy_uint result_index = INVALID_INDEX;

	// perform binary search for first or last occurence
	while(l <= h)
	{
		cy_uint m = l + ((h - l) / 2);
		if(compare(iai_p->get_element(iai_p->ds_p, m), data_p) > 0)
			h = m - 1;
		else if(compare(iai_p->get_element(iai_p->ds_p, m), data_p) < 0)
			l = m + 1;
		else
		{
			result_index = m;
			switch(occurence_type)
			{
				case FIRST_OCCURENCE:
				{
					h = m - 1;
					break;
				}
				case LAST_OCCURENCE:
				{
					l = m + 1;
					break;
				}
			}
		}
	}

	return result_index;
}

cy_uint find_preceding_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, int (*compare)(const void* data1, const void* data2))
{
	// check for valid start and last indexes
	if(start_index > last_index || last_index >= iai_p->get_element_count(iai_p->ds_p))
		return INVALID_INDEX;

	// take care of conditions when we might go start_index - 1
	// if the element is lesser than or equal to the element at the start_index
	// then there is no element that is lesser than it, so return INVALID_INDEX
	if(compare(iai_p->get_element(iai_p->ds_p, start_index), data_p) >= 0)
		return INVALID_INDEX;

	// binary search low and high range variables
	cy_uint l = start_index;
	cy_uint h = last_index;

	// result from performing binary search
	cy_uint result_index = INVALID_INDEX;

	// perform binary search for first or last occurence
	while(l <= h)
	{
		cy_uint m = l + ((h - l) / 2);
		if(compare(iai_p->get_element(iai_p->ds_p, m), data_p) < 0)
		{
			result_index = m;
			l = m + 1;
		}
		else
			h = m - 1;
	}

	return result_index;
}

cy_uint find_preceding_or_equals_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, int (*compare)(const void* data1, const void* data2))
{
	// check for valid start and last indexes
	if(start_index > last_index || last_index >= iai_p->get_element_count(iai_p->ds_p))
		return INVALID_INDEX;

	// take care of conditions when we might go start_index - 1
	// if the element is lesser than or equal to the element at the start_index
	// then there is no element that is lesser than it, so return INVALID_INDEX
	if(compare(iai_p->get_element(iai_p->ds_p, start_index), data_p) > 0)
		return INVALID_INDEX;

	// binary search low and high range variables
	cy_uint l = start_index;
	cy_uint h = last_index;

	// result from performing binary search
	cy_uint result_index = INVALID_INDEX;

	// perform binary search for first or last occurence
	while(l <= h)
	{
		cy_uint m = l + ((h - l) / 2);
		if(compare(iai_p->get_element(iai_p->ds_p, m), data_p) > 0)
			h = m - 1;
		else
		{
			result_index = m;
			l = m + 1;
		}
	}

	return result_index;
}

cy_uint find_succeeding_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, int (*compare)(const void* data1, const void* data2))
{
	// check for valid start and last indexes
	if(start_index > last_index || last_index >= iai_p->get_element_count(iai_p->ds_p))
		return INVALID_INDEX;

	// take care of conditions when we might go start_index - 1
	// if the element is lesser than the element at the start_index
	// then its succeeding is the start_indexed element, so return start_index
	if(compare(iai_p->get_element(iai_p->ds_p, start_index), data_p) > 0)
		return start_index;

	// binary search low and high range variables
	cy_uint l = start_index;
	cy_uint h = last_index;

	// result from performing binary search
	cy_uint result_index = INVALID_INDEX;

	// perform binary search for first or last occurence
	while(l <= h)
	{
		cy_uint m = l + ((h - l) / 2);
		if(compare(iai_p->get_element(iai_p->ds_p, m), data_p) > 0)
		{
			result_index = m;
			h = m - 1;
		}
		else
			l = m + 1;
	}

	return result_index;
}

cy_uint find_succeeding_or_equals_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, int (*compare)(const void* data1, const void* data2))
{
	// check for valid start and last indexes
	if(start_index > last_index || last_index >= iai_p->get_element_count(iai_p->ds_p))
		return INVALID_INDEX;

	// take care of conditions when we might go start_index - 1
	// if the element is lesser than the element at the start_index
	// then its succeeding is the start_indexed element, so return start_index
	if(compare(iai_p->get_element(iai_p->ds_p, start_index), data_p) >= 0)
		return start_index;

	// binary search low and high range variables
	cy_uint l = start_index;
	cy_uint h = last_index;

	// result from performing binary search
	cy_uint result_index = INVALID_INDEX;

	// perform binary search for first or last occurence
	while(l <= h)
	{
		cy_uint m = l + ((h - l) / 2);
		if(compare(iai_p->get_element(iai_p->ds_p, m), data_p) < 0)
			l = m + 1;
		else
		{
			result_index = m;
			h = m - 1;
		}
	}

	return result_index;
}

cy_uint find_insertion_index_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, int (*compare)(const void* data1, const void* data2))
{
	// check for valid start and last indexes
	if(start_index > last_index || last_index >= iai_p->get_element_count(iai_p->ds_p))
		return INVALID_INDEX;

	// take care of conditions when we might go start_index - 1
	// if the element is lesser than the element at the start_index, then return start_index
	if(compare(iai_p->get_element(iai_p->ds_p, start_index), data_p) > 0)
		return start_index;

	// binary search low and high range variables
	cy_uint l = start_index;
	cy_uint h = last_index + 1;

	// result from performing binary search
	cy_uint result_index = INVALID_INDEX;

	// perform binary search for first or last occurence
	while(l <= h)
	{
		cy_uint m = l + ((h - l) / 2);

		// if the mid has crossed the last_index,
		// then the last element is more than or equal to the data,
		// that we are comparing with
		if(m == last_index + 1)
		{
			result_index = m;
			break;
		}

		// if the element at m in the array is greater,
		// then update the result_index (since it could be the answer) and shorten the search range
		if(compare(iai_p->get_element(iai_p->ds_p, m), data_p) > 0)
		{
			result_index = m;
			h = m - 1;
		}
		// if the element at m in the array is lesser than or equal to given parameter (data),
		// then just shorten the search range on the left, since no element at index lesser than m can be the answer
		else
			l = m + 1;
	}

	return result_index;
}