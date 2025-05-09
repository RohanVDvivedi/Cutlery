#include<cutlery/heap.h>

#include<cutlery/cutlery_node.h>
#include<cutlery/cutlery_stds.h>

#include<cutlery/n-ary_tree_as_array_util.h>

// utility : interchanges data elements at indices i1 and i2
static void inter_change_elements_for_indexes(heap* heap_p, cy_uint i1, cy_uint i2)
{
	swap_in_arraylist(&(heap_p->heap_holder), i1, i2);

	// once the elements have been interchanged we update the heap_index of those elements
	// heap_index is in their embedded nodes (hpnode)
	if(heap_p->node_offset != NO_HEAP_NODE_OFFSET)
	{
		((hpnode*)(get_node(get_from_arraylist(&(heap_p->heap_holder), i1), heap_p)))->heap_index = i1;
		((hpnode*)(get_node(get_from_arraylist(&(heap_p->heap_holder), i2), heap_p)))->heap_index = i2;
	}
}

// returns true (1) if, the reordering is required, else 0
// we do not check if parent index is actually the parent of the child
// hence, this function can be used to test if the order could be made correct
static int is_reordering_required_for_indexes(const heap* heap_p, cy_uint parent_index, cy_uint child_index)
{
	// we dont allow reordering if, parent index or child index are out of bounds of element_count
	if(parent_index >= get_element_count_heap(heap_p) || child_index >= get_element_count_heap(heap_p))
		return 0;

	// retrieve parent and child pointers
	const void* parent = get_from_arraylist(&(heap_p->heap_holder), parent_index);
	const void* child  = get_from_arraylist(&(heap_p->heap_holder), child_index );

	return is_reordering_required(parent, child, &(heap_p->info));
}

static void bubble_up(heap* heap_p, cy_uint index)
{
	// exit at index 0, or thew index is out of range
	while(has_parent_N(index) && index < get_element_count_heap(heap_p))
	{
		cy_uint parent_index = get_parent_index_N(index, heap_p->degree);

		// exit, if reordering is not required
		if(!is_reordering_required_for_indexes(heap_p, parent_index, index))
			break;

		inter_change_elements_for_indexes(heap_p, parent_index, index);

		index = parent_index;
	}
}

static void bubble_down(heap* heap_p, cy_uint index)
{
	// we can not bubble down the last node
	while(can_have_any_children_N(index, heap_p->degree) && index < get_element_count_heap(heap_p))
	{
		// if no reordering is required then the element at the index position remains as the parent
		cy_uint new_parent_index = index;

		// iterate over all the children of the parent at index
		// find the one that if made the new parent, will not require reordering with any of its siblings
		for(cy_uint i = 0, child_index = get_index_of_ith_child_N(index, 0, heap_p->degree); i < heap_p->degree && child_index < get_element_count_heap(heap_p); i++, child_index++)
		{
			if(is_reordering_required_for_indexes(heap_p, new_parent_index, child_index))
				new_parent_index = child_index;
		}

		// if this condition becomes true, then the index stays the parent of all its siblings, hence no reordering required
		// and if no reordering was required, exit the loop
		if(new_parent_index == index)
			break;

		inter_change_elements_for_indexes(heap_p, new_parent_index, index);

		index = new_parent_index;
	}
}

void initialize_hpnode(hpnode* node_p)
{
	// reset the heap_index to an INVALID_INDEX, i.e. out of bounds for any heap
	node_p->heap_index = INVALID_INDEX;
}

int is_free_floating_hpnode(const hpnode* node_p)
{
	return node_p->heap_index == INVALID_INDEX;
}

int initialize_heap(heap* heap_p, cy_uint capacity, heap_type type, cy_uint degree, const comparator_interface* comparator, cy_uint node_offset)
{
	if(degree == 0)
		return 0;

	heap_p->info = (heap_info){.type = type, .comparator = (*comparator)};
	heap_p->degree = degree;
	heap_p->node_offset = node_offset;
	return initialize_arraylist(&(heap_p->heap_holder), capacity);
}

int initialize_heap_with_allocator(heap* heap_p, cy_uint capacity, heap_type type, cy_uint degree, const comparator_interface* comparator, cy_uint node_offset, memory_allocator mem_allocator)
{
	if(degree == 0)
		return 0;

	heap_p->info = (heap_info){.type = type, .comparator = (*comparator)};
	heap_p->degree = degree;
	heap_p->node_offset = node_offset;
	return initialize_arraylist_with_allocator(&(heap_p->heap_holder), capacity, mem_allocator);
}

int initialize_heap_with_memory(heap* heap_p, cy_uint capacity, heap_type type, cy_uint degree, const comparator_interface* comparator, cy_uint node_offset, const void* data_ps[])
{
	if(degree == 0)
		return 0;

	heap_p->info = (heap_info){.type = type, .comparator = (*comparator)};
	heap_p->degree = degree;
	heap_p->node_offset = node_offset;
	return initialize_arraylist_with_memory(&(heap_p->heap_holder), capacity, data_ps);
}

int push_to_heap(heap* heap_p, const void* data)
{
	// fail, heap is full
	if(is_full_heap(heap_p))
		return 0;

	// if embedded hpnode is being used, then the hpnode must be a free floating node, i.e. it must not exist in any heap
	if(heap_p->node_offset != NO_HEAP_NODE_OFFSET && !is_free_floating_hpnode(get_node(data, heap_p)))
		return 0;

	// push new element to the heap_holder at it's back, i.e. as its last element
	// since we already ensured that there is enough capacity in the heap_holder, this call must not fail
	// if this call fails you are doing something wrong!!
	if(!push_back_to_arraylist(&(heap_p->heap_holder), data))
		return 0;

	// update its heap index
	if(heap_p->node_offset != NO_HEAP_NODE_OFFSET)
		((hpnode*)(get_node(get_from_arraylist(&(heap_p->heap_holder), get_element_count_heap(heap_p) - 1), heap_p)))->heap_index = get_element_count_heap(heap_p) - 1;

	// bubble up the newly added element at last index (element_count-1), to its desired place
	bubble_up(heap_p, get_element_count_heap(heap_p) - 1);

	return 1;
}

int push_all_to_heap(heap* heap_p, index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index)
{
	// fail if the indexes provided in the array are invalid
	if(start_index > last_index || last_index >= iai_p->get_element_count(iai_p->ds_p))
		return 0;

	// number of elements to be inserted from start_index to last_index (both inclusive)
	cy_uint elements_to_insert = last_index - start_index + 1;

	// if the capacity of heap is not enough to hold all the elements, then return with a failure
	if(get_capacity_heap(heap_p) - get_element_count_heap(heap_p) < elements_to_insert)
		return 0;

	// if a node_offset is to be maintained i.e. heap_p->node_offset != NO_HEAP_NODE_OFFSET
	// then we must ensure that all the elements have embedded hpnode that are free floating
	if(heap_p->node_offset != NO_HEAP_NODE_OFFSET)
	{
		for(cy_uint i = 0; i < elements_to_insert; i++)
		{
			// get the data element that needs to be analyzed
			const void* data = iai_p->get_element(iai_p->ds_p, start_index + i);

			// we fail, if embedded hpnode of data is not free floating
			if(!is_free_floating_hpnode(get_node(data, heap_p)))
				return 0;
		}
	}

	// insert all the elements from array [start_index to last_index] to the heap_p
	for(cy_uint i = 0; i < elements_to_insert; i++)
	{
		// get the data element that needs to be inserted
		const void* data = iai_p->get_element(iai_p->ds_p, start_index + i);

		// we already made sure that all the elements (if they have heap_p->node_offset), have hpnode that are free floating

		// push data to the back of the heap_holder, i.e. at its back, i.e. last element from front
		// since we already ensured that there is enough capacity in the heap_holder, this call must not fail
		// if this call fails you are doing something wrong!!
		// so we are not handling the failure to push_back here
		push_back_to_arraylist(&(heap_p->heap_holder), data);
		
		// update its heap index
		if(heap_p->node_offset != NO_HEAP_NODE_OFFSET)
			((hpnode*)(get_node(get_from_arraylist(&(heap_p->heap_holder), get_element_count_heap(heap_p) - 1), heap_p)))->heap_index = get_element_count_heap(heap_p) - 1;
	}

	// heapify all the elements of the heap
	heapify_all(heap_p);

	return 1;
}

int pop_from_heap(heap* heap_p)
{
	// can not pop, if there are no elements in the heap
	if(is_empty_heap(heap_p))
		return 0;

	// remove the 0th element from the heap
	return remove_at_index_from_heap(heap_p, 0);
}

const void* get_top_of_heap(const heap* heap_p)
{
	// there is no top element, if there are no elements in the heap
	if(is_empty_heap(heap_p))
		return NULL;

	// return front of the arraylist
	return get_front_of_arraylist(&(heap_p->heap_holder));
}

int remove_from_heap(heap* heap_p, const void* data)
{
	// can not remove from heap if the heap is empty OR if we can not get index for the provided data
	if(is_empty_heap(heap_p) || heap_p->node_offset == NO_HEAP_NODE_OFFSET)
		return 0;

	// figure out the index to call remove at
	cy_uint index_to_remove_at = ((hpnode*)(get_node(data, heap_p)))->heap_index;

	// we can not remove if the data is a free floating node (i.e. not existing in any heap) OR the data does not exist in heap at the desired index (as dictated by hpnode)
	if(heap_p->node_offset != NO_HEAP_NODE_OFFSET && 
		(is_free_floating_hpnode(get_node(data, heap_p)) || data != get_from_arraylist(&(heap_p->heap_holder), index_to_remove_at)))
			return 0;

	// remove the ith element from the heap
	return remove_at_index_from_heap(heap_p, index_to_remove_at);
}

int remove_at_index_from_heap(heap* heap_p, cy_uint index)
{
	// an element can be removed, only if heap is not empty, and the index provided is within bounds
	if(is_empty_heap(heap_p) || index >= get_element_count_heap(heap_p))
		return 0;

	// put the indexed element at last and last element to indexed place
	inter_change_elements_for_indexes(heap_p, index, get_element_count_heap(heap_p) - 1);

	// re-initialize heap_index of the (last) element that we are going to remove
	if(heap_p->node_offset != NO_HEAP_NODE_OFFSET)
		initialize_hpnode(get_node(get_from_arraylist(&(heap_p->heap_holder), get_element_count_heap(heap_p) - 1), heap_p));

	// pop the last_element of the arraytlist, this operation is expected to never if fail
	// if it fails you are doing something wrong, since we already checked that the heap_p is not empty
	if(!pop_back_from_arraylist(&(heap_p->heap_holder)))
	{
		// undo - the initialize_hpnode at index (element_count-1)
		((hpnode*)get_node(get_from_arraylist(&(heap_p->heap_holder), get_element_count_heap(heap_p) - 1), heap_p))->heap_index = get_element_count_heap(heap_p) - 1;
		// undo - interchange of the index and (element_count-1)
		inter_change_elements_for_indexes(heap_p, index, get_element_count_heap(heap_p) - 1);
		return 0;
	}

	// call heapify at index, to appropriately call bubble up or bubble down
	// only if the index (the old last element) is still within bounds
	// index will be out of bounds, if the index == original element_count - 1
	if(index < get_element_count_heap(heap_p))
		heapify_at(heap_p, index);

	return 1;
}

void heapify_for(heap* heap_p, const void* data)
{
	// can not heapify at an index, if the heap is empty OR if we can not get index for the provided data
	if(is_empty_heap(heap_p) || heap_p->node_offset == NO_HEAP_NODE_OFFSET)
		return ;

	// figure out the index to call heapify at
	cy_uint index_to_heapify_at = ((hpnode*)(get_node(data, heap_p)))->heap_index;

	// we can not heapify if the data is a free floating node (i.e. not existing in any heap) OR if the data does not exist in heap at the desired index
	if(heap_p->node_offset != NO_HEAP_NODE_OFFSET && 
		(is_free_floating_hpnode(get_node(data, heap_p)) || data != get_from_arraylist(&(heap_p->heap_holder), index_to_heapify_at)))
			return ;

	// heapify at the ith element of the heap
	return heapify_at(heap_p, index_to_heapify_at);
}

void heapify_at(heap* heap_p, cy_uint index)
{
	// return if index is out-of-bounds
	if(index >= get_element_count_heap(heap_p))
		return;

	// if re-ordering is required at the parent side, we bubble up
	if(has_parent_N(index) && is_reordering_required_for_indexes(heap_p, get_parent_index_N(index, heap_p->degree), index))
		bubble_up(heap_p, index);

	// else we attempt a bubble down, only if it can have children
	else if(can_have_any_children_N(index, heap_p->degree))
		bubble_down(heap_p, index);
}

void heapify_all(heap* heap_p)
{
	// heapify_all is not required, if the element_count is 0 OR 1
	if(get_element_count_heap(heap_p) <= 1)
		return;

	// bubble_down at all the possible-parent elements in reverse order
	// this is because a bubble down at any leaf node is just a NOP, hence we are saving almost half of the loop iterations
	for(cy_uint index = get_parent_index_N(get_element_count_heap(heap_p) - 1, heap_p->degree); index != -1; index--)
		bubble_down(heap_p, index);
}

void remove_all_from_heap(heap* heap_p)
{
	// re initialize all the hpnodes
	if(heap_p->node_offset != NO_HEAP_NODE_OFFSET)
	{
		for(cy_uint i = 0; i < get_element_count_heap(heap_p); i++)
			initialize_hpnode(get_node(get_from_arraylist(&(heap_p->heap_holder), i), heap_p));
	}

	// then remove all the elements from the holder
	remove_all_from_arraylist(&(heap_p->heap_holder));
}

void deinitialize_heap(heap* heap_p)
{
	deinitialize_arraylist(&(heap_p->heap_holder));
	heap_p->node_offset = NO_HEAP_NODE_OFFSET;
	heap_p->degree = 0;
	heap_p->info = (heap_info){};
}

cy_uint get_capacity_heap(const heap* heap_p)
{
	return get_capacity_arraylist(&(heap_p->heap_holder));
}

cy_uint get_element_count_heap(const heap* heap_p)
{
	return get_element_count_arraylist(&(heap_p->heap_holder));
}

int is_full_heap(const heap* heap_p)
{
	return is_full_arraylist(&(heap_p->heap_holder));
}

int is_empty_heap(const heap* heap_p)
{
	return is_empty_arraylist(&(heap_p->heap_holder));
}

int expand_heap(heap* heap_p)
{
	return expand_arraylist(&(heap_p->heap_holder));
}

int shrink_heap(heap* heap_p)
{
	return shrink_arraylist(&(heap_p->heap_holder));
}

int reserve_capacity_for_heap(heap* heap_p, cy_uint atleast_capacity)
{
	return reserve_capacity_for_arraylist(&(heap_p->heap_holder), atleast_capacity);
}

void for_each_in_heap(const heap* heap_p, void (*operation)(void* data, cy_uint heap_index, const void* additional_params), const void* additional_params)
{
	for_each_in_arraylist(&(heap_p->heap_holder), operation, additional_params);
}

void sprint_heap(dstring* append_str, const heap* heap_p, void (*sprint_element)(dstring* append_str, const void* data, unsigned int tabs), unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs++);
	switch(heap_p->info.type)
	{
		case MIN_HEAP :
		{snprintf_dstring(append_str, "heap (MIN_HEAP) :\n");break;}
		case MAX_HEAP :
		{snprintf_dstring(append_str, "heap (MAX_HEAP) :\n");break;}
	}

	sprint_chars(append_str, '\t', tabs);
	snprintf_dstring(append_str, "degree : %" PRIu_cy_uint "\n", heap_p->degree);

	sprint_chars(append_str, '\t', tabs);
	snprintf_dstring(append_str, "element_count : %" PRIu_cy_uint "\n", get_element_count_heap(heap_p));

	sprint_chars(append_str, '\t', tabs);
	snprintf_dstring(append_str, "heap_holder : \n");
	sprint_arraylist(append_str, &(heap_p->heap_holder), sprint_element, tabs + 1);
	snprintf_dstring(append_str, "\n");

	sprint_chars(append_str, '\t', tabs);
	snprintf_dstring(append_str, "top : ");
	if(get_top_of_heap(heap_p) != NULL)
		sprint_element(append_str, get_top_of_heap(heap_p), 0);
	else
		snprintf_dstring(append_str, "NULL");
	snprintf_dstring(append_str, "\n");
}