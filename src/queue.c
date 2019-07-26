#include<queue.h>

queue* get_queue(unsigned long long int expected_size)
{
	queue* queue_p = (queue*) calloc(1, sizeof(queue));
	queue_p->queueHolder = get_array(expected_size + 1);
	queue_p->earliestElementIndex = 1;
	queue_p->latestElementIndex = 0;
	return queue_p;
}

unsigned long long int revolveToNextIndex(queue* queue_p, unsigned long long int index)
{
	return ((index + 1) % (queue_p->queueHolder->total_size));
}

void push(queue* queue_p, const void* data_p)
{
	if(isQueueHolderFull(queue_p))
	{
		queue queue_test = *queue_p;
		unsigned long long int prev_total_size = queue_p->queueHolder->total_size;
		expand_array(queue_p->queueHolder);
		for(unsigned long long int i = 0; i < prev_total_size; i++)
		{
			const void* data_p = get_top(&queue_test);
			pop(&queue_test);
			push(queue_p, data_p);
		}
	}
	queue_p->latestElementIndex = revolveToNextIndex(queue_p, queue_p->latestElementIndex);
	set_element(queue_p->queueHolder, data_p, queue_p->latestElementIndex);
}

void pop(queue* queue_p)
{
	if(isQueueEmpty(queue_p))
	{
		return;
	}
	set_element(queue_p->queueHolder, NULL, queue_p->earliestElementIndex);
	queue_p->earliestElementIndex = revolveToNextIndex(queue_p, queue_p->earliestElementIndex);
}

const void* get_top(queue* queue_p)
{
	if(isQueueEmpty(queue_p))
	{
		return NULL;
	}
	return get_element(queue_p->queueHolder, queue_p->earliestElementIndex);
}

void delete_queue(queue* queue_p)
{
	delete_array(queue_p->queueHolder);
	queue_p->queueHolder = NULL;
	queue_p->earliestElementIndex = 1;
	queue_p->latestElementIndex = 0;
	free(queue_p);
}

int isQueueEmpty(queue* queue_p)
{
	return revolveToNextIndex(queue_p, queue_p->latestElementIndex) == queue_p->earliestElementIndex && get_element(queue_p->queueHolder, queue_p->earliestElementIndex) == NULL;
}

int isQueueHolderFull(queue* queue_p)
{
	return revolveToNextIndex(queue_p, queue_p->latestElementIndex) == queue_p->earliestElementIndex && get_element(queue_p->queueHolder, queue_p->earliestElementIndex) != NULL;
}

void print_queue(queue* queue_p, void (*print_element)(const void* data_p))
{
	printf("queue : \n");
	printf("\tearliestElementIndex : %llu\n", queue_p->earliestElementIndex);
	printf("\tlatestElementIndex : %llu\n", queue_p->latestElementIndex);
	print_array(queue_p->queueHolder, print_element);
}