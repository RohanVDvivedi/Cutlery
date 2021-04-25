#include<stdio.h>
#include<heap.h>

typedef struct teststruct ts;
struct teststruct
{
	int key;
	int a;
	char* s;
	int index;
};

int cmp(const void* data1, const void* data2)
{
	return ( (((ts*)data1)->key) - (((ts*)data2)->key) );
}

void print_ts(const void* tsv)
{
	printf(" %d, %d, %s =<HEAP_INDEX>=> %d", ((ts*)tsv)->key, ((ts*)tsv)->a, ((ts*)tsv)->s, ((ts*)tsv)->index);
}

void sprint_ts(dstring* append_str, const void* tsv)
{
	snprintf_dstring(append_str, "%d, %d, %s =<HEAP_INDEX>=> %d", ((ts*)tsv)->key, ((ts*)tsv)->a, ((ts*)tsv)->s, ((ts*)tsv)->index);
}

void print_ts_heap(heap* heap_p)
{
	dstring str;
	init_dstring(&str, "", 0);
	sprint_heap(&str, heap_p, sprint_ts, 0);
	printf_dstring(&str);
	deinit_dstring(&str);
	printf("\n");
}

void push_heap_SAFE(heap* heap_p, const void* data_p)
{
	if(is_full_heap(heap_p))
		expand_heap(heap_p);
	push_heap(heap_p, data_p);
}

void pop_heap_OPTIMUM_MEMORY(heap* heap_p)
{
	pop_heap(heap_p);
	if(get_total_size_heap(heap_p) > 1.5 * get_element_count_heap(heap_p))
		shrink_heap(heap_p);
}

void change_key(heap* heap_p, unsigned int index, int new_key)
{
	if(index <= heap_p->element_count - 1)
	{
		((ts*)get_element(&(heap_p->heap_holder), index))->key = new_key;
		heapify_at(heap_p, index);
	}
}

void update_index_callback(const void* data, unsigned int heap_index, const void* additional_params)
{
	((ts*)data)->index = heap_index;
}

int main()
{
	heap heap_temp;
	heap* heap_p = &heap_temp;
	initialize_heap(heap_p, 5, MIN_HEAP, cmp, update_index_callback, NULL);

	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){1, 1, "one"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){2, 2, "two"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){5, 3, "three"}));
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){4, 4, "four"}));
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){4, 4, "four-x"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){3, 3, "three-x"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){2, 2, "two-x"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){1, 1, "one-x"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){5, 5, "five"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){6, 6, "six"}));
	print_ts_heap(heap_p);

	change_key(heap_p, heap_p->element_count/2, -2000);
	print_ts_heap(heap_p);

	change_key(heap_p, heap_p->element_count/2, +2000);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){7, 7, "seven"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){8, 8, "eight"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){14, 14, "fourteen"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){13, 13, "thirteen"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){12, 12, "twelve"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){11, 11, "eleven"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){9, 9, "nine"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){10, 10, "ten"}));
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){2, 2, "two-xx"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){1, 1, "one-xx"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){5, 5, "five-x"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){6, 6, "six-x"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){11, 11, "eleven-x"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){5, 5, "five-xx"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){6, 6, "six-xx"}));
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){12, 12, "twelve-x"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){13, 13, "thirteen-x"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){14, 14, "fourteen-x"}));
	print_ts_heap(heap_p);

	change_key(heap_p, 12, 3);
	print_ts_heap(heap_p);

	change_key(heap_p, 1, 8);
	print_ts_heap(heap_p);

	change_key(heap_p, 5, 2);
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){9, 9, "nine-x"}));
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){14, 14, "fourteen-x"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){13, 13, "thirteen-x"}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){12, 12, "twelve-x"}));
	print_ts_heap(heap_p);

	deinitialize_heap(heap_p);

	return 0;
}