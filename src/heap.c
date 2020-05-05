#include<heap.h>

#define push    push_heap
#define pop     pop_heap
#define get_top get_top_heap

heap* get_heap(unsigned long long int expected_size, heap_type type, int (*key_compare)(const void* key0, const void* key1))
{
	heap* heap_p = ((heap*)(malloc(sizeof(heap))));
	heap_p->type = type;
	heap_p->key_compare = key_compare;
	initialize_array(&(heap_p->heap_holder), expected_size + 1);
	heap_p->heap_size = 0;
	return heap_p;
}

// utility : gets index of the bucket, that is the parent to the given bucket at index = child
static unsigned long long int get_parent_index(unsigned long long int child)
{
	return (child-1)/2;
}

// utility : gets index of the bucket, that is the left child to the given parent bucket at index = child
static unsigned long long int get_left_child_index(unsigned long long int parent)
{
	return (2 * parent) + 1;
}

// utility : gets index of the bucket, that is the right child to the given parent bucket at index = child
static unsigned long long int get_right_child_index(unsigned long long int parent)
{
	return (2 * parent) + 2;
}

// utility : interchanges bucket at indices i1 and i2
static void inter_change_buckets_for_indexes(heap* heap_p, unsigned long long int i1, unsigned long long int i2)
{
	bucket* bucket_p_i1 = ((bucket*)get_element(&(heap_p->heap_holder), i1));
	bucket* bucket_p_i2 = ((bucket*)get_element(&(heap_p->heap_holder), i2));
	set_element(&(heap_p->heap_holder), bucket_p_i1, i2);
	set_element(&(heap_p->heap_holder), bucket_p_i2, i1);
}

// returns true (1) if, the reordering is required, else 0
// we do not check if parent index is actually the parent of the child
// hence, this function can be user to test if the order could be made correct
static int is_reordering_required(const heap* heap_p, unsigned long long int parent_index, unsigned long long int child_index)
{
	if(parent_index >= heap_p->heap_size || child_index >= heap_p->heap_size)
	{
		// we dont allow reordering if, parent index or child index are out of bounds of heap_size
		return 0;
	}

	bucket* parent = ((bucket*)get_element(&(heap_p->heap_holder), parent_index));
	bucket* child  = ((bucket*)get_element(&(heap_p->heap_holder), child_index));

	// if the child or parent is NULL, you can not reorder 
	if(child == NULL || parent == NULL)
	{
		return 0;
	}

	int reordering_required = 0;

	switch(heap_p->type)
	{
		case MIN_HEAP :
		{
			// in min heap, parent has to be smaller than or equal to the child, we have an issue if parent is greater than child 
			if(heap_p->key_compare(parent->key, child->key) > 0)
			{
				reordering_required = 1;
			}
			break;
		}
		case MAX_HEAP :
		{
			// in min heap, parent has to be greater than or equal to the child, we have an issue if parent is smaller than child 
			if(heap_p->key_compare(parent->key, child->key) < 0)
			{
				reordering_required = 1;
			}
			break;
		}
	}

	return reordering_required;
}

static void bubble_up(heap* heap_p, unsigned long long int index)
{
	// an element at index 0, or thew index is out of range, can not be bubbled up
	if(index == 0 || index >= heap_p->heap_size)
	{
		return;
	}

	// get parent index for the index
	unsigned long long int parent_index = get_parent_index(index);

	// if a reordering is required, we interchange the parent and child bucket
	if(is_reordering_required(heap_p, parent_index, index))
	{
		// we would have to interchange for reordering
		inter_change_buckets_for_indexes(heap_p, parent_index, index);

		// and we bubble up with the parent_index
		bubble_up(heap_p, parent_index);
	}
}

void push(heap* heap_p, const void* key, const void* value)
{
	// expand heap_holder if necessary
	if(heap_p->heap_size >= heap_p->heap_holder.total_size)
	{
		expand_array(&(heap_p->heap_holder));
	}

	// set the element to the last index and increment its size
	set_element(&(heap_p->heap_holder), get_bucket(key, value), heap_p->heap_size++);

	// bubble up the newly added element at index heap_p->heap_size-1, to its desired place
	bubble_up(heap_p, heap_p->heap_size-1);
}

const void* get_top(const heap* heap_p, const void** returned_key)
{
	// ther is no top bucket value, if there are no buckets in the heap
	if(heap_p->heap_size == 0)
	{
		return NULL;
	}

	// first bucket is the top bucket
	bucket* top_bucket_p = (bucket*)get_element(&(heap_p->heap_holder), 0);

	// return the key to the client
	if(returned_key != NULL)
	{
		(*returned_key) = top_bucket_p->key;
	}

	// return the value of the first bucket, from the heap holder
	return top_bucket_p->value;
}

static void bubble_down(heap* heap_p, unsigned long long int index)
{
	// we can not bubble down the last node
	if(index >= heap_p->heap_size)
	{
		return;
	}

	unsigned long long int left_child_index = get_left_child_index(index);
	unsigned long long int right_child_index = get_right_child_index(index);

	unsigned long long int new_parent_index = -1;

	// simple logic, if reordering is required for left child and index, left child would become the new parent
	if(is_reordering_required(heap_p, index, left_child_index))
	{
		new_parent_index = left_child_index;

		// if index and right child also require reordering, we have to contest whether right child or left child would become the new parent
		// here we try to make the parent, which does not require further reordering, given the new parent is left_child_index
		if(is_reordering_required(heap_p, index, right_child_index) && 
			is_reordering_required(heap_p, new_parent_index, right_child_index))
		{
			new_parent_index = right_child_index;
		}	
	}
	// simple logic, if reordering is required for right child and index, right child would become the new parent
	else if(is_reordering_required(heap_p, index, right_child_index))
	{
		new_parent_index = right_child_index;
	}

	if(new_parent_index != -1)
	{
		// if new_parent_index has to come, we make the new_parent_index
		inter_change_buckets_for_indexes(heap_p, new_parent_index, index);

		// and bubble down from. there
		bubble_down(heap_p, new_parent_index);
	}
}

void pop(heap* heap_p)
{
	// we can pop only if the heap size if greater than 0, and there is atleast 1 element
	if(heap_p->heap_size > 0)
	{
		// put the first bucket at last and last bucket to first
		inter_change_buckets_for_indexes(heap_p, 0, heap_p->heap_size-1);

		// get the last bucket and delete it
		delete_bucket((bucket*)get_element(&(heap_p->heap_holder), heap_p->heap_size-1));

		// and set the last element to null
		set_element(&(heap_p->heap_holder), NULL, --heap_p->heap_size);

		// bubble down the element at index 0
		bubble_down(heap_p, 0);

		// let the array be shrunk if it is required
		shrink_array(&(heap_p->heap_holder), 0, heap_p->heap_size - 1);
	}
}

void heapify_at(heap* heap_p, unsigned long long int index)
{
	// do not provide out of heap-bound index
	if(index >= heap_p->heap_size)
	{
		return;
	}

	// pre-evaluate parent, left child and right child indexes for the corresponding index
	unsigned long long int parent_index = get_parent_index(index);
	unsigned long long int left_child_index = get_left_child_index(index);
	unsigned long long int right_child_index = get_right_child_index(index);

	// if re-ordering is required at the parent side, we bubble up
	if(is_reordering_required(heap_p, parent_index, index))
	{
		bubble_up(heap_p, index);
	}
	// else if the re ordering is required at any of the children's side we bubble down
	else if(is_reordering_required(heap_p, index, left_child_index) || 
			is_reordering_required(heap_p, index, right_child_index))
	{
		bubble_down(heap_p, index);
	}
}

void delete_heap(heap* heap_p)
{
	for_each_in_array(&(heap_p->heap_holder), delete_bucket_wrapper_for_array, NULL);
	deinitialize_array(&(heap_p->heap_holder));
	heap_p->heap_size = 0;
	free(heap_p);
}

void print_heap(heap* heap_p, void (*print_key)(const void* key), void (*print_value)(const void* value))
{
	switch(heap_p->type)
	{
		case MIN_HEAP :
		{
			printf("heap : MIN_HEAP\n");
			break;
		}
		case MAX_HEAP :
		{
			printf("heap : MAX_HEAP\n");
			break;
		}
	}
	printf("\theap_size : %llu\n", heap_p->heap_size);
	bucket print_functions = {.key = print_key, .value = print_value};
	printf("\theap array : \n");
	for_each_in_array(&(heap_p->heap_holder), print_bucket_wrapper_for_array, &print_functions);
	printf("\n");
	printf("\tthe top element : ");
	if(get_top(heap_p, NULL) != NULL)
	{
		print_value(get_top(heap_p, NULL));
	}
	else
	{
		printf("NULL");
	}
	printf("\n");
}

#undef push
#undef pop
#undef get_top
