#include<index_accessed_search_sort.h>

#include<cutlery_math.h>

#include<array.h>
#include<heap.h>
#include<arraylist.h>

#include<memory_allocator_interface.h>

// below is a utility required only by the merge sort functionality

typedef struct iai_offsetted iai_offsetted;
struct iai_offsetted
{
	index_accessed_interface* iai_p;
	cy_uint offset_index;
};

static const void* get_element_iai_offsetted(const void* ds_p, cy_uint index)
{
	const iai_offsetted* io_p = ds_p;
	return io_p->iai_p->get_element(io_p->iai_p->ds_p, io_p->offset_index + index);
}

static int set_element_iai_offsetted(void* ds_p, const void* data_p, cy_uint index)
{
	iai_offsetted* io_p = ds_p;
	return io_p->iai_p->set_element(io_p->iai_p->ds_p, data_p, io_p->offset_index + index);
}

static int swap_elements_iai_offsetted(void* ds_p, cy_uint i1, cy_uint i2)
{
	iai_offsetted* io_p = ds_p;
	return io_p->iai_p->swap_elements(io_p->iai_p->ds_p, io_p->offset_index + i1, io_p->offset_index + i1);
}

static cy_uint get_element_count_iai_offsetted(const void* ds_p)
{
	const iai_offsetted* io_p = ds_p;
	return io_p->iai_p->get_element_count(io_p->iai_p->ds_p) - io_p->offset_index;
}

#define get_index_accessed_interface_ofsetted(iai_p, offset) \
											(index_accessed_interface){ \
												.ds_p = &((iai_offsetted){.iai_p = (iai_p), .offset_index = (offset)}),\
												.get_element = get_element_iai_offsetted,\
												.set_element = set_element_iai_offsetted,\
												.swap_elements = swap_elements_iai_offsetted,\
												.get_element_count = get_element_count_iai_offsetted,\
											}

// -----------------------------------------------------------------

int is_sorted_iai(index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const comparator_interface* comparator)
{
	if(start_index > last_index || last_index >= iai_p->get_element_count(iai_p->ds_p))
		return 0;

	cy_uint total_elements = last_index - start_index + 1;
	if(total_elements <= 1)
		return 1;

	//check that all the adjacent elements are in correct order
	for(cy_uint i = start_index; i < last_index; i++)
	{
		// if the adjacent of i is not correctly ordered, then fail with a 0
		if(compare_with_comparator(comparator, iai_p->get_element(iai_p->ds_p, i), iai_p->get_element(iai_p->ds_p, i + 1)) > 0)
			return 0;
	}

	return 1;
}

int merge_sort_iai(index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const comparator_interface* comparator, memory_allocator mem_allocator)
{
	if(start_index > last_index || last_index >= iai_p->get_element_count(iai_p->ds_p))
		return 0;

	// compute the number of elements to sort; 0 or 1 number of elements do not need sorting
	cy_uint total_elements = last_index - start_index + 1;
	if(total_elements <= 1)
		return 1;

	// generate a new index_accessed_interface to access elements from 0, instead of start_index
	index_accessed_interface src_iai = get_index_accessed_interface_ofsetted(iai_p, start_index);

	// generate an auxilary array
	array aux_array;
	if(!initialize_array_with_allocator(&aux_array, total_elements, mem_allocator))
		return 0;
	index_accessed_interface aux_array_iface = get_index_accessed_interface_for_array(&aux_array);

	// we iteratively merge adjacent sorted chunks from src and store them in dest
	index_accessed_interface* src_iai_p = &src_iai;
	index_accessed_interface* dest_iai_p = &aux_array_iface;

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

				// copy all elements of the a-section to the destination as is
				for(cy_uint a_i = a_start; a_i <= a_last; a_i++, dest_index++)
					dest_iai_p->set_element(dest_iai_p->ds_p, src_iai_p->get_element(src_iai_p->ds_p, a_i), dest_index);

				break;
			}
			else
			{
				if(b_last > total_elements - 1)
					b_last = total_elements - 1;

				while(dest_index <= b_last)
				{
					if((b_start > b_last) || (a_start <= a_last && compare_with_comparator(comparator, src_iai_p->get_element(src_iai_p->ds_p, a_start), src_iai_p->get_element(src_iai_p->ds_p, b_start)) < 0))
						dest_iai_p->set_element(dest_iai_p->ds_p, src_iai_p->get_element(src_iai_p->ds_p, a_start++), dest_index++);
					else
						dest_iai_p->set_element(dest_iai_p->ds_p, src_iai_p->get_element(src_iai_p->ds_p, b_start++), dest_index++);
				}
			}
		}

		// src becomes dest, and dest becomes src
		void* temp = src_iai_p;
		src_iai_p = dest_iai_p;
		dest_iai_p = temp;

		// now the sorted chunk size is twice, so we need to multiple sort_chunk_size with 2
		// we need to make sure that the multiplication does not overflow,
		// if it does then this implies that the sort_chunk_size > maximum possible capacity of an index_accessed_interface, and hence > the total_elements
		if(will_unsigned_mul_overflow(cy_uint, sort_chunk_size, 2))
			break;

		// double the chunk size, for next iteration
		sort_chunk_size = sort_chunk_size * 2;
	}

	// at the end of every iteration the result is in src_iai_p

	// if the src_iai_p != &src_iai
	// then tranfer result to the original src

	if(src_iai_p != &src_iai)
	{
		// then copy all elements (0 to total_elements) from src_iai_p to src_iai
		for(cy_uint i = 0; i < total_elements; i++)
			src_iai.set_element(src_iai.ds_p, src_iai_p->get_element(src_iai_p->ds_p, i), i);
	}

	// deinitialize auxilary array
	deinitialize_array(&aux_array);

	return 1;
}

#include<n-ary_tree_as_array_util.h>
#include<heap_info.h>

// fails and returns 0, only if swap fails, on success returns 1
static int bubble_down_for_max_heap_for_heap_sort(index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const comparator_interface* comparator, cy_uint degree, cy_uint index)
{
	cy_uint element_count = last_index - start_index + 1;
	if(element_count <= 1)
		return 1;

	// make this index provided relative to the start_index
	index -= start_index;

	// use this macro to access the iai_p
	#define GET_ACTUAL_INDEX(relative_index) ((relative_index) + start_index)

	// there is max_heap in the name of this function
	heap_info max_heap_info = (heap_info){.type = MAX_HEAP, .comparator = (*comparator)};

	// we can not bubble down the last node
	while(can_have_any_children_N(index, degree) && index < element_count)
	{
		// if no reordering is required then the element at the index position remains as the parent
		cy_uint new_parent_index = index;

		// iterate over all the children of the parent at index
		// find the one that if made the new parent, will not require reordering with any of its siblings
		for(cy_uint i = 0, child_index = get_index_of_ith_child_N(index, 0, degree); i < degree && child_index < element_count; i++, child_index++)
		{
			if(is_reordering_required( 
				iai_p->get_element(iai_p->ds_p, GET_ACTUAL_INDEX(new_parent_index)), 
				iai_p->get_element(iai_p->ds_p, GET_ACTUAL_INDEX(child_index)), 
				&max_heap_info))
				new_parent_index = child_index;
		}

		// if this condition becomes true, then the index stays the parent of all its siblings, hence no reordering required
		// and if no reordering was required, exit the loop
		if(new_parent_index == index)
			break;

		int swapped = iai_p->swap_elements(iai_p->ds_p, GET_ACTUAL_INDEX(new_parent_index), GET_ACTUAL_INDEX(index));
		if(!swapped)
			return 0;

		index = new_parent_index;
	}

	return 1;
}

int heap_sort_iai(index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const comparator_interface* comparator, cy_uint degree)
{
	if(start_index > last_index || last_index >= iai_p->get_element_count(iai_p->ds_p))
		return 0;

	// degree has to be atleast 1, 2 for binary heap
	if(degree == 0)
		return 0;

	// compute the number of elements to sort; 0 or 1 number of elements do not need sorting
	cy_uint total_elements = last_index - start_index + 1;
	if(total_elements <= 1)
		return 1;

	// heapify
	// bubble_down from (start_index-1, last_index] in reverse
	for(cy_uint i = last_index; i != (start_index-1); i--)
	{
		if(!bubble_down_for_max_heap_for_heap_sort(iai_p, start_index, last_index, comparator, degree, i))
			return 0;
	}

	// again iterate in reverse for i being, (start_index, last_index] putting the elements at their ith correct index
	// pick the heap's max element and swap it with ith element
	for(cy_uint i = last_index; i != start_index; i--)
	{
		// swap ith element with the top of the heap, putting the ith element at its right index
		int swapped = iai_p->swap_elements(iai_p->ds_p, start_index, i);
		if(!swapped)
			return 0;

		// now start_index is not at its correct position for the max heap in range [start_index, i-1]
		// bubble down the start_index
		if(!bubble_down_for_max_heap_for_heap_sort(iai_p, start_index, i-1, comparator, degree, start_index))
			return 0;
	}

	return 1;
}

int quick_sort_iai(index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const comparator_interface* comparator)
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
			if(compare_with_comparator(comparator, iai_p->get_element(iai_p->ds_p, i), pivot) <= 0)
			{
				int swapped = iai_p->swap_elements(iai_p->ds_p, all_greater_than_pivot_start_index++, i);
				if(!swapped)
					return 0;
			}
		}

		// index of the pivot element
		cy_uint pivot_index = all_greater_than_pivot_start_index - 1;

		cy_uint size_of_part_before_pivot = pivot_index - start_index;
		cy_uint size_of_part_after_pivot = last_index - pivot_index;

		if(size_of_part_before_pivot < size_of_part_after_pivot)
		{
			// recurse for before part and loop for after part, in order to keep stack usage to minimum
			if(size_of_part_before_pivot > 0)
			{
				int sorted = quick_sort_iai(iai_p, start_index, pivot_index - 1, comparator);
				if(!sorted)
					return 0;
			}

			// no elements after pivot to loop for
			if(size_of_part_after_pivot == 0)
				return 1;

			start_index = pivot_index + 1;
		}
		else
		{
			// recurse for after part and loop for before part, in order to keep stack usage to minimum
			if(size_of_part_after_pivot > 0)
			{
				int sorted = quick_sort_iai(iai_p, pivot_index + 1, last_index, comparator);
				if(!sorted)
					return 0;
			}

			// no elements before pivot to loop for
			if(size_of_part_before_pivot == 0)
				return 1;

			last_index = pivot_index - 1;
		}
	}

	return 1;
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

int bubble_sort_iai(index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const comparator_interface* comparator)
{
	if(start_index > last_index || last_index >= iai_p->get_element_count(iai_p->ds_p))
		return 0;

	// compute the number of elements to sort; 0 or 1 number of elements do not need sorting
	cy_uint total_elements = last_index - start_index + 1;
	if(total_elements <= 1)
		return 1;

	// do while there are any swaps performed in the previous iteration
	int swaps_performed = 0;
	cy_uint loop_until = last_index; // this is the earliest index from the right that does not have its designated sorted element
	do
	{
		swaps_performed = 0;

		// loop over the entire array, from start_index to loop_until, swapping adjacent elements until loop_until has the rightfully designated element
		for(cy_uint i = start_index; i < loop_until; i++)
		{
			if(compare_with_comparator(comparator, iai_p->get_element(iai_p->ds_p, i), iai_p->get_element(iai_p->ds_p, i + 1)) > 0)
			{
				int swapped = iai_p->swap_elements(iai_p->ds_p, i, i + 1);
				if(!swapped)
					return 0;
				swaps_performed = 1;
			}
		}

		loop_until -= 1;
	}
	while(swaps_performed != 0);

	return 1;
}

int insertion_sort_iai(index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const comparator_interface* comparator)
{
	if(start_index > last_index || last_index >= iai_p->get_element_count(iai_p->ds_p))
		return 0;

	// compute the number of elements to sort; 0 or 1 number of elements do not need sorting
	cy_uint total_elements = last_index - start_index + 1;
	if(total_elements <= 1)
		return 1;

	// all elements from start_index (inclusive) to i (exclusive) are sorted
	// so in the loop's ith iteration we only need to find the right position for the current ith element
	for(cy_uint i = start_index; i <= last_index; i++)
	{
		for(cy_uint j = i; j > start_index; j--)
		{
			// check if j-1 and j are rightly ordered, if not swap them
			if(compare_with_comparator(comparator, iai_p->get_element(iai_p->ds_p, j - 1), iai_p->get_element(iai_p->ds_p, j)) > 0)
			{
				int swapped = iai_p->swap_elements(iai_p->ds_p, j - 1, j);
				if(!swapped)
					return 0;
			}
		}
	}

	return 1;
}

cy_uint linear_search_in_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, const comparator_interface* comparator, search_occurence occurence_type)
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
				if(compare_with_comparator(comparator, iai_p->get_element(iai_p->ds_p, i), data_p) == 0)
					return i;
			}
			break;
		}
		case LAST_OCCURENCE:
		{
			for(cy_uint i = last_index; i != (start_index - 1); i--)
			{
				if(compare_with_comparator(comparator, iai_p->get_element(iai_p->ds_p, i), data_p) == 0)
					return i;
			}
			break;
		}
	}

	// we return answer or return an element out of bounds
	return INVALID_INDEX;
}

cy_uint binary_search_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, const comparator_interface* comparator, search_occurence occurence_type)
{
	// check for valid start and last indexes
	if(start_index > last_index || last_index >= iai_p->get_element_count(iai_p->ds_p))
		return INVALID_INDEX;

	switch(occurence_type)
	{
		case FIRST_OCCURENCE :
		{
			// take care of conditions when we might go start_index - 1
			if(compare_with_comparator(comparator, iai_p->get_element(iai_p->ds_p, start_index), data_p) == 0)
				return start_index;
			else if(compare_with_comparator(comparator, iai_p->get_element(iai_p->ds_p, start_index), data_p) > 0)
				return INVALID_INDEX;
			break;
		}
		case LAST_OCCURENCE :
		{
			// take care of conditions when we might go start_index - 1
			if(compare_with_comparator(comparator, iai_p->get_element(iai_p->ds_p, start_index), data_p) > 0)
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
		if(compare_with_comparator(comparator, iai_p->get_element(iai_p->ds_p, m), data_p) > 0)
			h = m - 1;
		else if(compare_with_comparator(comparator, iai_p->get_element(iai_p->ds_p, m), data_p) < 0)
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

cy_uint find_preceding_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, const comparator_interface* comparator)
{
	// check for valid start and last indexes
	if(start_index > last_index || last_index >= iai_p->get_element_count(iai_p->ds_p))
		return INVALID_INDEX;

	// take care of conditions when we might go start_index - 1
	// if the element is lesser than or equal to the element at the start_index
	// then there is no element that is lesser than it, so return INVALID_INDEX
	if(compare_with_comparator(comparator, iai_p->get_element(iai_p->ds_p, start_index), data_p) >= 0)
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
		if(compare_with_comparator(comparator, iai_p->get_element(iai_p->ds_p, m), data_p) < 0)
		{
			result_index = m;
			l = m + 1;
		}
		else
			h = m - 1;
	}

	return result_index;
}

cy_uint find_preceding_or_equals_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, const comparator_interface* comparator)
{
	// check for valid start and last indexes
	if(start_index > last_index || last_index >= iai_p->get_element_count(iai_p->ds_p))
		return INVALID_INDEX;

	// take care of conditions when we might go start_index - 1
	// if the element is lesser than or equal to the element at the start_index
	// then there is no element that is lesser than it, so return INVALID_INDEX
	if(compare_with_comparator(comparator, iai_p->get_element(iai_p->ds_p, start_index), data_p) > 0)
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
		if(compare_with_comparator(comparator, iai_p->get_element(iai_p->ds_p, m), data_p) > 0)
			h = m - 1;
		else
		{
			result_index = m;
			l = m + 1;
		}
	}

	return result_index;
}

cy_uint find_succeeding_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, const comparator_interface* comparator)
{
	// check for valid start and last indexes
	if(start_index > last_index || last_index >= iai_p->get_element_count(iai_p->ds_p))
		return INVALID_INDEX;

	// take care of conditions when we might go start_index - 1
	// if the element is lesser than the element at the start_index
	// then its succeeding is the start_indexed element, so return start_index
	if(compare_with_comparator(comparator, iai_p->get_element(iai_p->ds_p, start_index), data_p) > 0)
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
		if(compare_with_comparator(comparator, iai_p->get_element(iai_p->ds_p, m), data_p) > 0)
		{
			result_index = m;
			h = m - 1;
		}
		else
			l = m + 1;
	}

	return result_index;
}

cy_uint find_succeeding_or_equals_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, const comparator_interface* comparator)
{
	// check for valid start and last indexes
	if(start_index > last_index || last_index >= iai_p->get_element_count(iai_p->ds_p))
		return INVALID_INDEX;

	// take care of conditions when we might go start_index - 1
	// if the element is lesser than the element at the start_index
	// then its succeeding is the start_indexed element, so return start_index
	if(compare_with_comparator(comparator, iai_p->get_element(iai_p->ds_p, start_index), data_p) >= 0)
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
		if(compare_with_comparator(comparator, iai_p->get_element(iai_p->ds_p, m), data_p) < 0)
			l = m + 1;
		else
		{
			result_index = m;
			h = m - 1;
		}
	}

	return result_index;
}

cy_uint find_insertion_index_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, const comparator_interface* comparator)
{
	// check for valid start and last indexes
	if(start_index > last_index || last_index >= iai_p->get_element_count(iai_p->ds_p))
		return INVALID_INDEX;

	// take care of conditions when we might go start_index - 1
	// if the element is lesser than the element at the start_index, then return start_index
	if(compare_with_comparator(comparator, iai_p->get_element(iai_p->ds_p, start_index), data_p) > 0)
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
		if(compare_with_comparator(comparator, iai_p->get_element(iai_p->ds_p, m), data_p) > 0)
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