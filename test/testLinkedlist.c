#include<stdio.h>
#include<linkedlist.h>

typedef struct teststruct ts;
struct teststruct
{
	int a;
	char* s;

	llnode ll_embed_node;
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
	sprint_chars(append_str, '\t', tabs);
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

int main()
{
	linkedlist llist;
	linkedlist* ll = &llist;
	initialize_linkedlist(ll, (unsigned long long int)(&(((ts*)0)->ll_embed_node)));

	printf("Error on attempt to remove head : %d, or tail : %d, from an empty linkedlist\n\n", remove_head(ll), remove_tail(ll));

	print_ts_linkedlist(ll);

	insert_head(ll, &((ts){2, "two", {NULL, NULL}}));
	print_ts_linkedlist(ll);

	insert_head(ll, &((ts){1, "one", {NULL, NULL}}));
	print_ts_linkedlist(ll);

	insert_tail(ll, &((ts){5, "five", {NULL, NULL}}));
	print_ts_linkedlist(ll);

	insert_tail(ll, &((ts){6, "six", {NULL, NULL}}));
	print_ts_linkedlist(ll);

	print_ts(get_nth_from_head(ll, 1));

	insert_after(ll, get_nth_from_head(ll, 1), &((ts){3, "three", {NULL, NULL}}));
	print_ts_linkedlist(ll);

	insert_before(ll, get_nth_from_tail(ll, 1), &((ts){4, "four", {NULL, NULL}}));
	print_ts_linkedlist(ll);

	insert_before(ll, get_nth_from_tail(ll, 2), &((ts){-1, "minus one", {NULL, NULL}}));
	print_ts_linkedlist(ll);

	// check get next and prev functionality
	{
		const ts* head_data = get_head(ll);
		printf("prev : ");print_ts(get_prev_of(ll, head_data));
		printf("\nnode : ");print_ts(head_data);
		printf("\nnext : ");print_ts(get_next_of(ll, head_data));
		printf("\n\n");

		const ts* mid_data = find_equals_in_linkedlist(ll, &((ts){-1}), test_compare);
		printf("prev : ");print_ts(get_prev_of(ll, mid_data));
		printf("\nnode : ");print_ts(mid_data);
		printf("\nnext : ");print_ts(get_next_of(ll, mid_data));
		printf("\n\n");

		const ts* tail_data = get_tail(ll);
		printf("prev : ");print_ts(get_prev_of(ll, tail_data));
		printf("\nnode : ");print_ts(tail_data);
		printf("\nnext : ");print_ts(get_next_of(ll, tail_data));
		printf("\n\n");
	}
	// **** 

	remove_head(ll);
	print_ts_linkedlist(ll);

	remove_tail(ll);
	print_ts_linkedlist(ll);

	const ts* temp = get_nth_from_head(ll, 2);

	remove_from_linkedlist(ll, temp);
	print_ts_linkedlist(ll);

	printf("Error on removing a not existing node twice %d\n", remove_from_linkedlist(ll, temp));
	print_ts_linkedlist(ll);

	printf("Error on inserting before a not existing node %d\n", insert_before(ll, temp, &((ts){-1, "minus one", {NULL, NULL}})));
	print_ts_linkedlist(ll);

	insert_after(ll, get_nth_from_head(ll, 1), temp);
	print_ts_linkedlist(ll);

	printf("Error on inserting an existing node %d\n", insert_before(ll, temp, get_nth_from_tail(ll, 3)));
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
	printf("\n");

	return 0;
}