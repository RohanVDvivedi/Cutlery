 #include<queue.h>

#define push    push_queue
#define pop     pop_queue
#define get_top get_top_queue

queue* get_queue(unsigned long long int expected_size)
{
	queue* queue_p = (queue*) calloc(1, sizeof(queue));
	queue_p->queue_holder = get_array(expected_size + 1);
	queue_p->queue_size = 0;
	queue_p->earliest_element_index = 1;
	queue_p->latest_element_index = 0;
	return queue_p;
}

unsigned long long int revolveToNextIndex(queue* queue_p, unsigned long long int index)
{
	return ((index + 1) % (queue_p->queue_holder->total_size));
}

void push(queue* queue_p, const void* data_p)
{
	if(isQueueHolderFull(queue_p))
	{
		queue* new_queue_p = get_queue( (2 * queue_p->queue_holder->total_size) + 1);

		while(!isQueueEmpty(queue_p))
		{
			const void* data_p = get_top(queue_p);
			pop(queue_p);
			push(new_queue_p, data_p);
		}

		delete_array(queue_p->queue_holder);
		(*queue_p) = (*new_queue_p);
		free(new_queue_p);
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
	return queue_p->queue_size == queue_p->queue_holder->total_size;
}

void print_queue(queue* queue_p, void (*print_element)(const void* data_p))
{
	printf("queue : \n");
	printf("\tearliest_element_index : %llu\n", queue_p->earliest_element_index);
	printf("\tlatest_element_index : %llu\n", queue_p->latest_element_index);
	print_array(queue_p->queue_holder, print_element);printf("\n\n");
}

#undef push
#undef pop
#undef get_top
