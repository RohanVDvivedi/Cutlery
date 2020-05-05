 #include<queue.h>

#define push    push_queue
#define pop     pop_queue
#define get_top get_top_queue

queue* get_queue(unsigned long long int expected_size)
{
	queue* queue_p = (queue*) calloc(1, sizeof(queue));
	initialize_array(&(queue_p->queue_holder), expected_size + 1);
	queue_p->queue_size = 0;
	queue_p->earliest_element_index = 1;
	queue_p->latest_element_index = 0;
	return queue_p;
}

static unsigned long long int revolveToNextIndex(queue* queue_p, unsigned long long int index)
{
	return ((index + 1) % (queue_p->queue_holder.total_size));
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
				pop(&queue_old_temp);
				push(queue_p, data_p);
			}
		}
	}
	queue_p->latest_element_index = revolveToNextIndex(queue_p, queue_p->latest_element_index);
	set_element(&(queue_p->queue_holder), data_p, queue_p->latest_element_index);
	queue_p->queue_size++;
}

void pop(queue* queue_p)
{
	if(isQueueEmpty(queue_p))
	{
		return;
	}
	set_element(&(queue_p->queue_holder), NULL, queue_p->earliest_element_index);
	queue_p->earliest_element_index = revolveToNextIndex(queue_p, queue_p->earliest_element_index);
	queue_p->queue_size--;
}

const void* get_top(queue* queue_p)
{
	if(isQueueEmpty(queue_p))
	{
		return NULL;
	}
	return get_element(&(queue_p->queue_holder), queue_p->earliest_element_index);
}

void delete_queue(queue* queue_p)
{
	deinitialize_array(&(queue_p->queue_holder));
	queue_p->earliest_element_index = 1;
	queue_p->latest_element_index = 0;
	free(queue_p);
}

int isQueueEmpty(queue* queue_p)
{
	return queue_p->queue_size == 0;
}

int isQueueHolderFull(queue* queue_p)
{
	return queue_p->queue_size == queue_p->queue_holder.total_size;
}

void print_queue(queue* queue_p, void (*print_element)(const void* data_p))
{
	printf("queue : \n");
	printf("\tearliest_element_index : %llu\n", queue_p->earliest_element_index);
	printf("\tlatest_element_index : %llu\n", queue_p->latest_element_index);
	printf("\tqueue_size : %llu\n", queue_p->queue_size);
	print_array(&(queue_p->queue_holder), print_element);printf("\n\n");
}

#undef push
#undef pop
#undef get_top
