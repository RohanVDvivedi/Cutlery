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

int main()
{
	linkedlist llist;
	linkedlist* ll = &llist;
	initialize_linkedlist(ll, (unsigned long long int)(&(((ts*)0)->ll_embed_node)), test_compare);

	print_linkedlist(ll, print_ts);

	insert_head(ll, &((ts){2, "two", {NULL, NULL}}));
	print_linkedlist(ll, print_ts);

	insert_head(ll, &((ts){1, "one", {NULL, NULL}}));
	print_linkedlist(ll, print_ts);

	insert_tail(ll, &((ts){5, "five", {NULL, NULL}}));
	print_linkedlist(ll, print_ts);

	insert_tail(ll, &((ts){6, "six", {NULL, NULL}}));
	print_linkedlist(ll, print_ts);

	insert_after(ll, get_nth_from_head(ll, 1), &((ts){3, "three", {NULL, NULL}}));
	print_linkedlist(ll, print_ts);

	insert_before(ll, get_nth_from_tail(ll, 1), &((ts){4, "four", {NULL, NULL}}));
	print_linkedlist(ll, print_ts);

	insert_before(ll, get_nth_from_tail(ll, 2), &((ts){-1, "minus one", {NULL, NULL}}));
	print_linkedlist(ll, print_ts);

	remove_head(ll);
	print_linkedlist(ll, print_ts);

	remove_tail(ll);
	print_linkedlist(ll, print_ts);

	remove_from_list(ll, get_nth_from_head(ll, 2));
	print_linkedlist(ll, print_ts);

	return 0;
}