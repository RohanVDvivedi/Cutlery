 #include<queue.h>

#define push 		push_queue
#define pop 		pop_queue
#define get_top 	get_top_queue
#define for_each 	for_each_in_queue

void initialize_queue(queue* queue_p, unsigned int expected_size)
{
	initialize_array(&(queue_p->queue_holder), expected_size + 1);
	queue_p->queue_size = 0;
	queue_p->earliest_element_index = 0;
}

static unsigned int revolveToNextIndex(queue* queue_p, unsigned int index)
{
	return ((index + 1) % (queue_p->queue_holder.total_size));
}

// this is index where the latest element is located in the array
static unsigned int latest_element_index(queue* queue_p)
{
	return (((queue_p->earliest_element_index + queue_p->queue_holder.total_size) + queue_p->queue_size - 1) % (queue_p->queue_holder.total_size));
}

static void push_no_expand(queue* queue_p, const void* data_p)
{
	if(isQueueHolderFull(queue_p))
		return;

	unsigned latest_element_index_calculated = latest_element_index(queue_p);
	unsigned int new_element_index = revolveToNextIndex(queue_p, latest_element_index_calculated);
	set_element(&(queue_p->queue_holder), data_p, new_element_index);
	queue_p->queue_size++;
}

static void pop_no_shrink(queue* queue_p)
{
	if(isQueueEmpty(queue_p))
		return;
	
	set_element(&(queue_p->queue_holder), NULL, queue_p->earliest_element_index);
	queue_p->earliest_element_index = revolveToNextIndex(queue_p, queue_p->earliest_element_index);
	queue_p->queue_size--;
}

void push(queue* queue_p, const void* data_p)
{
	if(isQueueHolderFull(queue_p))
	{
		// shallow copy old queue
		queue queue_old_temp = (*queue_p);

		// expand the queue
		expand_array(&(queue_p->queue_holder));

		// if the earliest element and the latest element are the edges, we technically do not need the rotation
		if(queue_p->earliest_element_index != 0)
		{
			queue_p->queue_size = 0;

			// since we are aware about calling the expand array,
			// we know that the array memory was reallocated, so we re point it so as to rotate the array
			queue_old_temp.queue_holder.data_p_p = queue_p->queue_holder.data_p_p;

			while(!isQueueEmpty(&queue_old_temp))
			{
				const void* data_p = get_top(&queue_old_temp);
				pop_no_shrink(&queue_old_temp);
				push_no_expand(queue_p, data_p);
			}
		}
	}
	
	push_no_expand(queue_p, data_p);
}

void pop(queue* queue_p)
{
	pop_no_shrink(queue_p);

	unsigned latest_element_index_calculated = latest_element_index(queue_p);

	// we go forward to shrink the queue holder only if it is not empty -> if the queue_holder is already empty, it is small enough to not have allocated much memory to shrink it significantly
	// we also check that the earliest element_index is lesser or equal to latest element index, this allow us to avoid undesired rotations required in the ring buffer
	if(!isQueueEmpty(queue_p) && (queue_p->earliest_element_index <= latest_element_index_calculated))
	{
		int was_queue_array_shrunk = shrink_array(&(queue_p->queue_holder), queue_p->earliest_element_index, latest_element_index_calculated);
		if(was_queue_array_shrunk)
			queue_p->earliest_element_index = 0;
	}
}

const void* get_top(queue* queue_p)
{
	if(isQueueEmpty(queue_p))
		return NULL;
	return get_element(&(queue_p->queue_holder), queue_p->earliest_element_index);
}

void deinitialize_queue(queue* queue_p)
{
	deinitialize_array(&(queue_p->queue_holder));
	queue_p->queue_size = 0;
	queue_p->earliest_element_index = 0;
}

int isQueueEmpty(queue* queue_p)
{
	return queue_p->queue_size == 0;
}

int isQueueHolderFull(queue* queue_p)
{
	return queue_p->queue_size == queue_p->queue_holder.total_size;
}

void for_each(const queue* queue_p, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params)
{
	for_each_non_null_in_array(&(queue_p->queue_holder), operation, additional_params);
}

void print_queue(queue* queue_p, void (*print_element)(const void* data_p))
{
	printf("queue : \n");
	printf("\tearliest_element_index : %u\n", queue_p->earliest_element_index);
	printf("\tlatest_element_index : %u\n", latest_element_index(queue_p));
	printf("\tqueue_size : %u\n", queue_p->queue_size);
	print_array(&(queue_p->queue_holder), print_element);printf("\n\n");
}

#undef push
#undef pop
#undef get_top
#undef for_each
