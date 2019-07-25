#include<linkedlist.h>

typedef struct teststruct ts;
struct teststruct
{
	int a;
	char* s;
};

void print_ts(const void* tsv)
{
	printf(" %d, %s", ((ts*)tsv)->a, ((ts*)tsv)->s);
}

int main()
{
	linkedlist* ll = get_linkedlist();
	print_linkedlist(ll, print_ts);

	insert_head(ll, &((ts){1, "one"}));
	print_linkedlist(ll, print_ts);

	insert_head(ll, &((ts){2, "two"}));
	print_linkedlist(ll, print_ts);

	insert_tail(ll, &((ts){3, "three"}));
	print_linkedlist(ll, print_ts);
	
	insert_tail(ll, &((ts){4, "four"}));
	print_linkedlist(ll, print_ts);

	delete_linkedlist(ll);
	return 0;
}