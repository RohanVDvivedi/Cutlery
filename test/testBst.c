#include<balancedbst.h>

typedef struct teststruct ts;
struct teststruct
{
	int a;
	char* s;
};

typedef struct key ke;
struct key
{
	int k;
};

unsigned long long int hash_function(const void* key)
{
	return (*((unsigned long long int *)key))-1;
}

int key_cmp(const void* key1, const void* key2)
{
	return (((ke*)key1)->k) - (((ke*)key2)->k);
}

void print_ts(const void* tsv)
{
	printf(" %d, %s", ((ts*)tsv)->a, ((ts*)tsv)->s);
}

void print_key(const void* key)
{
	printf("%d", (*((int*)key)));
}

int main()
{
	printf("INITIALIZING BALANCED BINARY SEARCH TREE\n");

	balancedbst* balancedbst_p = get_balancedbst(NON_SELF_BALANCING, key_cmp);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	printf("COMPLETED INITIALIZING BALANCED BINARY SEARCH TREE\n");

	printf("STARTING TO INSERT NODES\n");

	put_entry(balancedbst_p, &((ke){10}), &((ts){10, "ten"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){6}), &((ts){6, "six"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){14}), &((ts){14, "fourteen"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){4}), &((ts){4, "four"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){8}), &((ts){8, "eight"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){12}), &((ts){12, "twelve"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){16}), &((ts){16, "sixteen"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){0}), &((ts){0, "zero"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){18}), &((ts){18, "eighteen"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){5}), &((ts){5, "five"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){7}), &((ts){7, "seven"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){11}), &((ts){11, "eleven"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){13}), &((ts){13, "thirteen"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	printf("COMPLETED INSERTING NODES\n");

	printf("STARTING TO REMOVE NODES\n");

	remove_value(balancedbst_p, &((ke){11}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	remove_value(balancedbst_p, &((ke){12}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	remove_value(balancedbst_p, &((ke){6}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	remove_value(balancedbst_p, &((ke){13}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	remove_value(balancedbst_p, &((ke){10}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	printf("COMPLETED REMOVING NODES\n");

	printf("STARTING TO DELETE BINARY SEARCH TREE\n");

	delete_balancedbst(balancedbst_p);

	printf("COMPLETED DELETING BINARY SEARCH TREE\n");
	return 0;
}