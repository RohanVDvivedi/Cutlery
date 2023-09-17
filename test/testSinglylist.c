#include<stdio.h>
#include<cutlery_stds.h>
#include<singlylist.h>

typedef struct teststruct ts;
struct teststruct
{
	int a;
	char* s;

	slnode sl_embed_node;
};

unsigned long long int get_radix_sort_attr(const void* data)
{
	return (((const ts*)data)->a + 100);
}

int compare_by_a_increasing(const void* a, const void* b)
{
	if(((const ts*)a)->a < ((const ts*)b)->a)
		return -1;
	else if(((const ts*)a)->a > ((const ts*)b)->a)
		return 1;
	else
		return 0;
}

const comparator_interface compare_by_a_increasing_comparator = simple_comparator(compare_by_a_increasing);

int compare_by_a_decreasing(const void* a, const void* b)
{
	if(((const ts*)a)->a < ((const ts*)b)->a)
		return 1;
	else if(((const ts*)a)->a > ((const ts*)b)->a)
		return -1;
	else
		return 0;
}

const comparator_interface compare_by_a_decreasing_comparator = simple_comparator(compare_by_a_decreasing);

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
	printf("%d, %s :: is_free_floating_slnode = %d\n", ((ts*)tsv)->a, ((ts*)tsv)->s, is_free_floating_slnode(&(((ts*)tsv)->sl_embed_node)));
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

const comparator_interface test_comparator = simple_comparator(test_compare);

void print_ts_singlylist(singlylist* sl)
{
	dstring str;
	init_dstring(&str, "", 0);
	sprint_singlylist(&str, sl, sprint_ts, 0);
	printf_dstring(&str);
	deinit_dstring(&str);
	printf("\n");
}

#define USE_RADIX_SORT
#define USE_BUBBLE_SORT

int main()
{
	singlylist slist;
	singlylist* sl = &slist;
	initialize_singlylist(sl, offsetof(ts, sl_embed_node));

	printf("Error on attempt to remove head : %d, from an empty singlylist\n\n", remove_head_from_singlylist(sl));

	print_ts_singlylist(sl);

	insert_head_in_singlylist(sl, &((ts){2, "two"}));
	print_ts_singlylist(sl);

	insert_head_in_singlylist(sl, &((ts){1, "one"}));
	print_ts_singlylist(sl);

	insert_tail_in_singlylist(sl, &((ts){5, "five"}));
	print_ts_singlylist(sl);

	insert_tail_in_singlylist(sl, &((ts){6, "six"}));
	print_ts_singlylist(sl);

	print_ts(get_from_head_of_singlylist(sl, 1));
	printf("\n\n");

	insert_after_in_singlylist(sl, get_from_head_of_singlylist(sl, 1), &((ts){3, "three"}));
	print_ts_singlylist(sl);

	// check get next and prev functionality
	{
		const ts* head_data = get_head_of_singlylist(sl);
		printf("\nnode : ");print_ts(head_data);
		printf("\nnext : ");print_ts(get_next_of_in_singlylist(sl, head_data));
		printf("\n\n");

		const ts* mid_data = find_equals_in_singlylist(sl, &((ts){3}), &test_comparator);
		printf("\nnode : ");print_ts(mid_data);
		printf("\nnext : ");print_ts(get_next_of_in_singlylist(sl, mid_data));
		printf("\n\n");

		const ts* tail_data = get_tail_of_singlylist(sl);
		printf("\nnode : ");print_ts(tail_data);
		printf("\nnext : ");print_ts(get_next_of_in_singlylist(sl, tail_data));
		printf("\n\n");
	}
	// **** 

	remove_head_from_singlylist(sl);
	print_ts_singlylist(sl);

	const ts* temp = get_from_head_of_singlylist(sl, 2);

	const ts* removed_data = remove_next_of_from_singlylist(sl, temp);
	print_ts_singlylist(sl);

	printf("Error on inserting after a not existing node %d\n", insert_after_in_singlylist(sl, removed_data, &((ts){-1, "minus one"})));
	print_ts_singlylist(sl);

	insert_after_in_singlylist(sl, get_from_head_of_singlylist(sl, 1), removed_data);
	print_ts_singlylist(sl);

	printf("Error on inserting an existing node %d\n", insert_after_in_singlylist(sl, removed_data, get_from_head_of_singlylist(sl, 3)));
	print_ts_singlylist(sl);

	printf("Searching for structure with a = 4\n");
	print_ts(find_equals_in_singlylist(sl, &((ts){4}), &test_comparator));
	printf("\n");

	printf("Searching for structure with a = 1\n");
	print_ts(find_equals_in_singlylist(sl, &((ts){1}), &test_comparator));
	printf("\n");

	printf("Searching for structure with a = 2\n");
	print_ts(find_equals_in_singlylist(sl, &((ts){2}), &test_comparator));
	printf("\n");

	printf("Searching for structure with a = -1\n");
	print_ts(find_equals_in_singlylist(sl, &((ts){-1}), &test_comparator));
	printf("\n");

	printf("Searching for structure with a = 6\n");
	print_ts(find_equals_in_singlylist(sl, &((ts){6}), &test_comparator));
	printf("\n");

	printf("Searching for structure with a = 5\n");
	print_ts(find_equals_in_singlylist(sl, &((ts){5}), &test_comparator));
	printf("\n\n");

	printf("Testing get_from_head\n");
	for(unsigned int i = 0; i < 7; i++)
	{
		printf("get_(%u)_th_from_head : ", i);
		print_ts(get_from_head_of_singlylist(sl, i));
		printf("\n");
	}
	printf("\n");

	print_ts_singlylist(sl);

	singlylist* testInsertAllHead = &(singlylist){};
	initialize_singlylist(testInsertAllHead, offsetof(ts, sl_embed_node));
	insert_head_in_singlylist(testInsertAllHead, &((ts){100, "1 0 0"}));
	insert_head_in_singlylist(testInsertAllHead, &((ts){101, "1 0 1"}));
	insert_head_in_singlylist(testInsertAllHead, &((ts){102, "1 0 2"}));

	printf("testInsertAllHead: \n");
	print_ts_singlylist(testInsertAllHead);

	insert_all_at_head_in_singlylist(sl, testInsertAllHead);
	print_ts_singlylist(sl);

	printf("testInsertAllHead: \n");
	print_ts_singlylist(testInsertAllHead);

	singlylist* testInsertAllTail = &(singlylist){};
	initialize_singlylist(testInsertAllTail, offsetof(ts, sl_embed_node));
	insert_head_in_singlylist(testInsertAllTail, &((ts){103, "1 0 3"}));
	insert_head_in_singlylist(testInsertAllTail, &((ts){104, "1 0 4"}));
	insert_head_in_singlylist(testInsertAllTail, &((ts){105, "1 0 5"}));

	printf("testInsertAllTail: \n");
	print_ts_singlylist(testInsertAllTail);

	insert_all_at_tail_in_singlylist(sl, testInsertAllTail);
	print_ts_singlylist(sl);

	printf("testInsertAllTail: \n");
	print_ts_singlylist(testInsertAllTail);

	singlylist* testInsertAllAfter = &(singlylist){};
	initialize_singlylist(testInsertAllAfter, offsetof(ts, sl_embed_node));
	insert_head_in_singlylist(testInsertAllAfter, &((ts){108, "1 0 8"}));

	printf("testInsertAllAfter: \n");
	print_ts_singlylist(testInsertAllAfter);

	insert_all_after_in_singlylist(sl, get_from_head_of_singlylist(sl, 2), testInsertAllAfter);
	print_ts_singlylist(sl);

	printf("testInsertAllAfter: \n");
	print_ts_singlylist(testInsertAllAfter);

	#ifdef USE_RADIX_SORT
		// radix sort singlylist
		printf("after radix sort : \n");
		radix_sort_singlylist(sl, get_radix_sort_attr);
		print_ts_singlylist(sl);
	#endif

	#ifdef USE_BUBBLE_SORT
		// bubble sort linkedlist
		printf("after bubble sort : \n");
		bubble_sort_singlylist(sl, &compare_by_a_increasing_comparator);
		print_ts_singlylist(sl);
	#endif

	// bubble sort linkedlist
	printf("after bubble sort (reverse) : \n");
	bubble_sort_singlylist(sl, &compare_by_a_decreasing_comparator);
	print_ts_singlylist(sl);

	printf("\n\nremoving all from singlylist\n\n");
	remove_all_from_singlylist(sl, &((notifier_interface){NULL, on_remove_all_println_ts_with_node}));
	printf("\n\n");
	print_ts_singlylist(sl);
	printf("\n\n");

	printf("Testing get_from_head\n");
	for(unsigned int i = 0; i < 2; i++)
	{
		printf("get_(%u)_th_from_head : ", i);
		print_ts(get_from_head_of_singlylist(sl, i));
		printf("\n");
	}
	printf("\n");

	return 0;
}