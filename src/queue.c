#include<queue.h>

#define push    push_queue
#define pop     pop_queue
#define get_top get_top_queue

queue* get_queue(unsigned long long int expected_size)
{
	queue* queue_p = (queue*) calloc(1, sizeof(queue));
	queue_p->queue_holder = get_array(expected_size + 1);
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
		queue queue_test = (*queue_p);
		unsigned long long int prev_total_size = queue_p->queue_holder->total_size;
		expand_array(queue_p->queue_holder);
		unsigned long long int curr_total_size = queue_p->queue_holder->total_size;
		for(unsigned long long int i = 0; i < prev_total_size; i++)
		{
			queue_p->queue_holder->total_size = prev_total_size;
			const void* data_p = get_top(&queue_test);
			pop(&queue_test);
			queue_p->queue_holder->total_size = curr_total_size;
			push(queue_p, data_p);
		}
	}
	queue_p->latest_element_index = revolveToNextIndex(queue_p, queue_p->latest_element_index);
	set_element(queue_p->queue_holder, data_p, queue_p->latest_element_index);
}

void pop(queue* queue_p)
{
	if(isQueueEmpty(queue_p))
	{
		return;
	}
	set_element(queue_p->queue_holder, NULL, queue_p->earliest_element_index);
	queue_p->earliest_element_index = revolveToNextIndex(queue_p, queue_p->earliest_element_index);
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
	return revolveToNextIndex(queue_p, queue_p->latest_element_index) == queue_p->earliest_element_index && get_element(queue_p->queue_holder, queue_p->earliest_element_index) == NULL;
}

int isQueueHolderFull(queue* queue_p)
{
	return revolveToNextIndex(queue_p, queue_p->latest_element_index) == queue_p->earliest_element_index && get_element(queue_p->queue_holder, queue_p->earliest_element_index) != NULL;
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
