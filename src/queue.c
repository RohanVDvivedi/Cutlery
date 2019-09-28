 #include<queue.h>

#define push    push_queue
#define pop     pop_queue
#define get_top get_top_queue

queue* get_queue(unsigned long long int expected_size)
{
	queue* queue_p = (queue*) calloc(1, sizeof(queue));
	queue_p->queue_holder = get_array(expected_size + 1);
	queue_p->array_size = queue_p->queue_holder->total_size;
	queue_p->queue_size = 0;
	queue_p->earliest_element_index = 1;
	queue_p->latest_element_index = 0;
	return queue_p;
}

unsigned long long int revolveToNextIndex(queue* queue_p, unsigned long long int index)
{
	return ((index + 1) % (queue_p->array_size));
}

void push(queue* queue_p, const void* data_p)
{
	if(isQueueHolderFull(queue_p))
	{
		// shallow copy old queue
		queue queue_old_temp = (*queue_p);

		// expand the queue, below 2 lines always go hand in hand
		expand_array(queue_p->queue_holder);
		queue_p->array_size = queue_p->queue_holder->total_size;

		// if the earliest element and the latest element are the edges, we technically do not need the rotation
		if(queue_p->earliest_element_index != 0)
		{
			queue_p->queue_size = 0;
			while(!isQueueEmpty(&queue_old_temp))
			{
				const void* data_p = get_top(&queue_old_temp);
				pop(&queue_old_temp);
				push(queue_p, data_p);
			}
		}
		else
		{
			queue_p->queue_size = queue_old_temp.queue_size;
		}
	}
	queue_p->latest_element_index = revolveToNextIndex(queue_p, queue_p->latest_element_index);
	set_element(queue_p->queue_holder, data_p, queue_p->latest_element_index);
	queue_p->queue_size++;
}

void pop(queue* queue_p)
{
	if(isQueueEmpty(queue_p))
	{
		return;
	}
	set_element(queue_p->queue_holder, NULL, queue_p->earliest_element_index);
	queue_p->earliest_element_index = revolveToNextIndex(queue_p, queue_p->earliest_element_index);
	queue_p->queue_size--;
}

const void* get_top(queue* queue_p)
{
	if(isQueueEmpty(queue_p))
	{
		return NULL;
	}
	return get_element(queue_p->queue_holder, queue_p->earliest_element_index);
}

void delete_queue(queue* queue_p)
{
	delete_array(queue_p->queue_holder);
	queue_p->queue_holder = NULL;
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
	return queue_p->queue_size == queue_p->array_size;
}

void print_queue(queue* queue_p, void (*print_element)(const void* data_p))
{
	printf("queue : \n");
	printf("\tearliest_element_index : %llu\n", queue_p->earliest_element_index);
	printf("\tlatest_element_index : %llu\n", queue_p->latest_element_index);
	printf("\tqueue_size : %llu\n", queue_p->queue_size);
	printf("\tarray_size : %llu\n", queue_p->array_size);
	print_array(queue_p->queue_holder, print_element);printf("\n\n");
}

#undef push
#undef pop
#undef get_top
