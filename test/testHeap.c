#include<stdio.h>
#include<cutlery_stds.h>
#include<heap.h>

#include<cutlery_stds.h>

typedef struct teststruct ts;
struct teststruct
{
	int key;
	int a;
	char* s;

	hpnode hp_embed_node;
};

int cmp(const void* data1, const void* data2)
{
	return ( (((ts*)data1)->key) - (((ts*)data2)->key) );
}

void print_ts(const void* tsv)
{
	printf(" %d, %d, %s =<HEAP_INDEX>=> %u", ((ts*)tsv)->key, ((ts*)tsv)->a, ((ts*)tsv)->s, ((ts*)tsv)->hp_embed_node.heap_index);
}

void sprint_ts(dstring* append_str, const void* tsv, unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs + 1);
	snprintf_dstring(append_str, "%d, %d, %s =<HEAP_INDEX>=> %u", ((ts*)tsv)->key, ((ts*)tsv)->a, ((ts*)tsv)->s, ((ts*)tsv)->hp_embed_node.heap_index);
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
	push_to_heap(heap_p, data_p);
}

void pop_heap_OPTIMUM_MEMORY(heap* heap_p)
{
	pop_from_heap(heap_p);
	if(get_capacity_heap(heap_p) > 1.5 * get_element_count_heap(heap_p))
		shrink_heap(heap_p);
}

void change_key(heap* heap_p, unsigned int index, int new_key)
{
	if(index <= heap_p->element_count - 1)
	{
		ts* element_to_update = ((ts*)get_from_array(&(heap_p->heap_holder), index));
		element_to_update->key = new_key;
		heapify_for(heap_p, element_to_update);
	}
}

#define NEW_HP_NODE {INVALID_INDEX}

int main()
{
	heap heap_temp;
	heap* heap_p = &heap_temp;
	initialize_heap(heap_p, 5, MIN_HEAP, cmp, offsetof(ts, hp_embed_node));

	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){1, 1, "one", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){2, 2, "two", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){5, 3, "three", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){4, 4, "four", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){4, 4, "four-x", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){3, 3, "three-x", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){2, 2, "two-x", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){1, 1, "one-x", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){5, 5, "five", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){6, 6, "six", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	change_key(heap_p, get_element_count_heap(heap_p)/2, -2000);
	print_ts_heap(heap_p);

	change_key(heap_p, get_element_count_heap(heap_p)/2, +2000);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){7, 7, "seven", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){8, 8, "eight", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){14, 14, "fourteen", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){13, 13, "thirteen", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){12, 12, "twelve", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){11, 11, "eleven", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){9, 9, "nine", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){10, 10, "ten", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){2, 2, "two-xx", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){1, 1, "one-xx", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){5, 5, "five-x", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){6, 6, "six-x", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){11, 11, "eleven-x", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){5, 5, "five-xx", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){6, 6, "six-xx", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){12, 12, "twelve-x", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){13, 13, "thirteen-x", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){14, 14, "fourteen-x", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	change_key(heap_p, 12, 3);
	print_ts_heap(heap_p);

	change_key(heap_p, 1, 8);
	print_ts_heap(heap_p);

	change_key(heap_p, 5, 2);
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){9, 9, "nine-x", NEW_HP_NODE}));
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

	printf("TOP before pop : \n");
	const ts* top = get_top_of_heap(heap_p);
	print_ts(top);
	printf("\n\n\n");

	pop_heap_OPTIMUM_MEMORY(heap_p);
	print_ts_heap(heap_p);

	printf("old-TOP after pop : \n");
	print_ts(top);
	printf("\n\n\n");

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

	push_heap_SAFE(heap_p, &((ts){14, 14, "fourteen-x", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){13, 13, "thirteen-x", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	push_heap_SAFE(heap_p, &((ts){12, 12, "twelve-x", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	printf("TOP before remove all : \n");
	top = get_top_of_heap(heap_p);
	print_ts(top);
	printf("\n\n\n");

	remove_all_from_heap(heap_p);
	print_ts_heap(heap_p);

	printf("TOP after remove all : \n");
	print_ts(top);
	printf("\n\n\n");

	push_heap_SAFE(heap_p, &((ts){0, 0, "000", NEW_HP_NODE}));
	print_ts_heap(heap_p);

	deinitialize_heap(heap_p);

	return 0;
}