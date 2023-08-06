#include<stdio.h>
#include<cutlery_stds.h>
#include<linkedlist.h>

typedef struct teststruct ts;
struct teststruct
{
	int a;
	char* s;

	llnode ll_embed_node;
};

int compare_by_a_increasing(const void* a, const void* b)
{
	if(((const ts*)a)->a < ((const ts*)b)->a)
		return -1;
	else if(((const ts*)a)->a > ((const ts*)b)->a)
		return 1;
	else
		return 0;
}

int compare_by_a_decreasing(const void* a, const void* b)
{
	if(((const ts*)a)->a < ((const ts*)b)->a)
		return 1;
	else if(((const ts*)a)->a > ((const ts*)b)->a)
		return -1;
	else
		return 0;
}

unsigned long long int get_radix_sort_attr(const void* data)
{
	return (((const ts*)data)->a + 100);
}

void print_ts(const void* tsv)
{
	if(tsv == NULL)
	{
		printf(" NULL");
		return;
	}
	printf(" %d, %s", ((ts*)tsv)->a, ((ts*)tsv)->s);
}

void on_remove_all_println_ts_with_node(void* resource_p, const void* tsv)
{
	printf("%d, %s :: is_free_floating_llnode = %d\n", ((ts*)tsv)->a, ((ts*)tsv)->s, is_free_floating_llnode(&(((ts*)tsv)->ll_embed_node)));
}

void sprint_ts(dstring* append_str, const void* tsv, unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs + 1);
	if(tsv == NULL)
	{
		snprintf_dstring(append_str, "NULL");
		return;
	}
	snprintf_dstring(append_str, "%d, %s", ((ts*)tsv)->a, ((ts*)tsv)->s);
}

int test_compare(const void* a, const void* b)
{
	if(a==NULL || b==NULL)
	{
		return -1;
	}
	else
	{
		return (((ts*)a)->a) - (((ts*)b)->a);
	}
}

void print_ts_linkedlist(linkedlist* ll)
{
	dstring str;
	init_dstring(&str, "", 0);
	sprint_linkedlist(&str, ll, sprint_ts, 0);
	printf_dstring(&str);
	deinit_dstring(&str);
	printf("\n");
}

#define USE_RADIX_SORT
#define USE_BUBBLE_SORT

int main()
{
	linkedlist llist;
	linkedlist* ll = &llist;
	initialize_linkedlist(ll, offsetof(ts, ll_embed_node));

	printf("Error on attempt to remove head : %d, or tail : %d, from an empty linkedlist\n\n", remove_head_from_linkedlist(ll), remove_tail_from_linkedlist(ll));

	print_ts_linkedlist(ll);

	insert_head_in_linkedlist(ll, &((ts){2, "two"}));
	print_ts_linkedlist(ll);

	insert_head_in_linkedlist(ll, &((ts){1, "one"}));
	print_ts_linkedlist(ll);

	insert_tail_in_linkedlist(ll, &((ts){5, "five"}));
	print_ts_linkedlist(ll);

	insert_tail_in_linkedlist(ll, &((ts){6, "six"}));
	print_ts_linkedlist(ll);

	print_ts(get_from_head_of_linkedlist(ll, 1));
	printf("\n\n");

	insert_after_in_linkedlist(ll, get_from_head_of_linkedlist(ll, 1), &((ts){3, "three"}));
	print_ts_linkedlist(ll);

	insert_before_in_linkedlist(ll, get_from_tail_of_linkedlist(ll, 1), &((ts){4, "four"}));
	print_ts_linkedlist(ll);

	insert_before_in_linkedlist(ll, get_from_tail_of_linkedlist(ll, 2), &((ts){-1, "minus one"}));
	print_ts_linkedlist(ll);

	// check get next and prev functionality
	{
		const ts* head_data = get_head_of_linkedlist(ll);
		printf("prev : ");print_ts(get_prev_of_in_linkedlist(ll, head_data));
		printf("\nnode : ");print_ts(head_data);
		printf("\nnext : ");print_ts(get_next_of_in_linkedlist(ll, head_data));
		printf("\n\n");

		const ts* mid_data = find_equals_in_linkedlist(ll, &((ts){-1}), test_compare);
		printf("prev : ");print_ts(get_prev_of_in_linkedlist(ll, mid_data));
		printf("\nnode : ");print_ts(mid_data);
		printf("\nnext : ");print_ts(get_next_of_in_linkedlist(ll, mid_data));
		printf("\n\n");

		const ts* tail_data = get_tail_of_linkedlist(ll);
		printf("prev : ");print_ts(get_prev_of_in_linkedlist(ll, tail_data));
		printf("\nnode : ");print_ts(tail_data);
		printf("\nnext : ");print_ts(get_next_of_in_linkedlist(ll, tail_data));
		printf("\n\n");
	}
	// **** 

	remove_head_from_linkedlist(ll);
	print_ts_linkedlist(ll);

	remove_tail_from_linkedlist(ll);
	print_ts_linkedlist(ll);

	const ts* temp = get_from_head_of_linkedlist(ll, 2);

	remove_from_linkedlist(ll, temp);
	print_ts_linkedlist(ll);

	printf("Error on removing a not existing node twice %d\n", remove_from_linkedlist(ll, temp));
	print_ts_linkedlist(ll);

	printf("Error on inserting before a not existing node %d\n", insert_before_in_linkedlist(ll, temp, &((ts){-1, "minus one"})));
	print_ts_linkedlist(ll);

	insert_after_in_linkedlist(ll, get_from_head_of_linkedlist(ll, 1), temp);
	print_ts_linkedlist(ll);

	printf("Error on inserting an existing node %d\n", insert_before_in_linkedlist(ll, temp, get_from_tail_of_linkedlist(ll, 3)));
	print_ts_linkedlist(ll);

	printf("Searching for structure with a = 4\n");
	print_ts(find_equals_in_linkedlist(ll, &((ts){4}), test_compare));
	printf("\n");

	printf("Searching for structure with a = 1\n");
	print_ts(find_equals_in_linkedlist(ll, &((ts){1}), test_compare));
	printf("\n");

	printf("Searching for structure with a = 2\n");
	print_ts(find_equals_in_linkedlist(ll, &((ts){2}), test_compare));
	printf("\n");

	printf("Searching for structure with a = -1\n");
	print_ts(find_equals_in_linkedlist(ll, &((ts){-1}), test_compare));
	printf("\n");

	printf("Searching for structure with a = 6\n");
	print_ts(find_equals_in_linkedlist(ll, &((ts){6}), test_compare));
	printf("\n");

	printf("Searching for structure with a = 5\n");
	print_ts(find_equals_in_linkedlist(ll, &((ts){5}), test_compare));
	printf("\n\n");

	printf("Testing get_from_head\n");
	for(unsigned int i = 0; i < 7; i++)
	{
		printf("get_th_from_head(%u) : ", i);
		print_ts(get_from_head_of_linkedlist(ll, i));
		printf("\n");
	}
	printf("\n");

	printf("Testing get_from_tail\n");
	for(unsigned int i = 0; i < 7; i++)
	{
		printf("get_from_tail(%u) : ", i);
		print_ts(get_from_tail_of_linkedlist(ll, i));
		printf("\n");
	}
	printf("\n");

	print_ts_linkedlist(ll);

	linkedlist* testInsertAllHead = &(linkedlist){};
	initialize_linkedlist(testInsertAllHead, offsetof(ts, ll_embed_node));
	insert_head_in_linkedlist(testInsertAllHead, &((ts){100, "1 0 0"}));
	insert_head_in_linkedlist(testInsertAllHead, &((ts){101, "1 0 1"}));
	insert_head_in_linkedlist(testInsertAllHead, &((ts){102, "1 0 2"}));
	insert_head_in_linkedlist(testInsertAllHead, &((ts){102, "1 0 2.5"}));

	printf("testInsertAllHead: \n");
	print_ts_linkedlist(testInsertAllHead);

	swap_in_linkedlist(testInsertAllHead, get_head_of_linkedlist(testInsertAllHead), get_from_head_of_linkedlist(testInsertAllHead, 2));
	printf("testInsertAllHead: (swapped 0th and 2nd element) \n");
	print_ts_linkedlist(testInsertAllHead);

	insert_all_at_head_in_linkedlist(ll, testInsertAllHead);
	print_ts_linkedlist(ll);

	printf("testInsertAllHead: \n");
	print_ts_linkedlist(testInsertAllHead);

	linkedlist* testInsertAllTail = &(linkedlist){};
	initialize_linkedlist(testInsertAllTail, offsetof(ts, ll_embed_node));
	insert_head_in_linkedlist(testInsertAllTail, &((ts){103, "1 0 3"}));
	insert_head_in_linkedlist(testInsertAllTail, &((ts){104, "1 0 4"}));
	insert_head_in_linkedlist(testInsertAllTail, &((ts){105, "1 0 5"}));

	printf("testInsertAllTail: \n");
	print_ts_linkedlist(testInsertAllTail);

	insert_all_at_tail_in_linkedlist(ll, testInsertAllTail);
	print_ts_linkedlist(ll);

	printf("testInsertAllTail: \n");
	print_ts_linkedlist(testInsertAllTail);

	linkedlist* testInsertAllBefore = &(linkedlist){};
	initialize_linkedlist(testInsertAllBefore, offsetof(ts, ll_embed_node));
	insert_head_in_linkedlist(testInsertAllBefore, &((ts){106, "1 0 6"}));
	insert_head_in_linkedlist(testInsertAllBefore, &((ts){107, "1 0 7"}));

	printf("testInsertAllBefore: \n");
	print_ts_linkedlist(testInsertAllBefore);

	swap_in_linkedlist(testInsertAllBefore, get_head_of_linkedlist(testInsertAllBefore), get_from_head_of_linkedlist(testInsertAllBefore, 1));
	printf("testInsertAllBefore: (swapped 0th and 1st element) \n");
	print_ts_linkedlist(testInsertAllBefore);

	insert_all_before_in_linkedlist(ll, get_from_head_of_linkedlist(ll, 0), testInsertAllBefore);
	print_ts_linkedlist(ll);

	printf("testInsertAllBefore: \n");
	print_ts_linkedlist(testInsertAllBefore);

	linkedlist* testInsertAllAfter = &(linkedlist){};
	initialize_linkedlist(testInsertAllAfter, offsetof(ts, ll_embed_node));
	insert_head_in_linkedlist(testInsertAllAfter, &((ts){108, "1 0 8"}));

	printf("testInsertAllAfter: \n");
	print_ts_linkedlist(testInsertAllAfter);

	insert_all_after_in_linkedlist(ll, get_from_head_of_linkedlist(ll, 2), testInsertAllAfter);
	print_ts_linkedlist(ll);

	printf("testInsertAllAfter: \n");
	print_ts_linkedlist(testInsertAllAfter);

	#ifdef USE_RADIX_SORT
		// radix sort linkedlist
		printf("after radix sort : \n");
		radix_sort_linkedlist(ll, get_radix_sort_attr);
		print_ts_linkedlist(ll);
	#endif

	#ifdef USE_BUBBLE_SORT
		// bubble sort linkedlist
		printf("after bubble sort : \n");
		bubble_sort_linkedlist(ll, compare_by_a_increasing);
		print_ts_linkedlist(ll);
	#endif

	// radix sort linkedlist
	printf("after bubble sort (reverse) : \n");
	bubble_sort_linkedlist(ll, compare_by_a_decreasing);
	print_ts_linkedlist(ll);

	printf("\n\nremoving all from linkedlist\n\n");
	remove_all_from_linkedlist(ll, &((notifier_interface){NULL, on_remove_all_println_ts_with_node}));
	print_ts_linkedlist(ll);

	printf("Testing get_from_head\n");
	for(unsigned int i = 0; i < 2; i++)
	{
		printf("get_th_from_head(%u) : ", i);
		print_ts(get_from_head_of_linkedlist(ll, i));
		printf("\n");
	}
	printf("\n");

	printf("Testing get_from_tail\n");
	for(unsigned int i = 0; i < 2; i++)
	{
		printf("get_th_from_tail(%u) : ", i);
		print_ts(get_from_tail_of_linkedlist(ll, i));
		printf("\n");
	}
	printf("\n");

	return 0;
}