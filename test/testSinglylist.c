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

void print_ts(const void* tsv)
{
	if(tsv == NULL)
	{
		printf(" NULL");
		return;
	}
	printf(" %d, %s", ((ts*)tsv)->a, ((ts*)tsv)->s);
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

void print_ts_singlylist(singlylist* sl)
{
	dstring str;
	init_dstring(&str, "", 0);
	sprint_singlylist(&str, sl, sprint_ts, 0);
	printf_dstring(&str);
	deinit_dstring(&str);
	printf("\n");
}

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

	print_ts(get_nth_from_head_of_singlylist(sl, 1));
	printf("\n\n");

	insert_after_in_singlylist(sl, get_nth_from_head_of_singlylist(sl, 1), &((ts){3, "three"}));
	print_ts_singlylist(sl);

	// check get next and prev functionality
	{
		const ts* head_data = get_head_of_singlylist(sl);
		printf("\nnode : ");print_ts(head_data);
		printf("\nnext : ");print_ts(get_next_of_in_singlylist(sl, head_data));
		printf("\n\n");

		const ts* mid_data = find_equals_in_singlylist(sl, &((ts){3}), test_compare);
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

	const ts* temp = get_nth_from_head_of_singlylist(sl, 2);

	const ts* removed_data = remove_next_of_from_singlylist(sl, temp);
	print_ts_singlylist(sl);

	printf("Error on inserting after a not existing node %d\n", insert_after_in_singlylist(sl, removed_data, &((ts){-1, "minus one"})));
	print_ts_singlylist(sl);

	insert_after_in_singlylist(sl, get_nth_from_head_of_singlylist(sl, 1), removed_data);
	print_ts_singlylist(sl);

	printf("Error on inserting an existing node %d\n", insert_after_in_singlylist(sl, removed_data, get_nth_from_head_of_singlylist(sl, 3)));
	print_ts_singlylist(sl);

	printf("Searching for structure with a = 4\n");
	print_ts(find_equals_in_singlylist(sl, &((ts){4}), test_compare));
	printf("\n");

	printf("Searching for structure with a = 1\n");
	print_ts(find_equals_in_singlylist(sl, &((ts){1}), test_compare));
	printf("\n");

	printf("Searching for structure with a = 2\n");
	print_ts(find_equals_in_singlylist(sl, &((ts){2}), test_compare));
	printf("\n");

	printf("Searching for structure with a = -1\n");
	print_ts(find_equals_in_singlylist(sl, &((ts){-1}), test_compare));
	printf("\n");

	printf("Searching for structure with a = 6\n");
	print_ts(find_equals_in_singlylist(sl, &((ts){6}), test_compare));
	printf("\n");

	printf("Searching for structure with a = 5\n");
	print_ts(find_equals_in_singlylist(sl, &((ts){5}), test_compare));
	printf("\n\n");

	printf("Testing get_nth_from_head\n");
	for(unsigned int i = 0; i < 7; i++)
	{
		printf("get_(%u)_th_from_head : ", i);
		print_ts(get_nth_from_head_of_singlylist(sl, i));
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

	insert_all_after_in_singlylist(sl, get_nth_from_head_of_singlylist(sl, 2), testInsertAllAfter);
	print_ts_singlylist(sl);

	printf("testInsertAllAfter: \n");
	print_ts_singlylist(testInsertAllAfter);

	printf("\n\nremoving all from singlylist\n\n");
	remove_all_from_singlylist(sl);
	print_ts_singlylist(sl);

	printf("Testing get_nth_from_head\n");
	for(unsigned int i = 0; i < 2; i++)
	{
		printf("get_(%u)_th_from_head : ", i);
		print_ts(get_nth_from_head_of_singlylist(sl, i));
		printf("\n");
	}
	printf("\n");

	return 0;
}