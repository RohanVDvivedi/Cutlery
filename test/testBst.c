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

	balancedbst* balancedbst_p = get_balancedbst(/*NON_SELF_BALANCING*/ /*AVL_TREE*/ RED_BLACK_TREE, key_cmp);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	printf("COMPLETED INITIALIZING BALANCED BINARY SEARCH TREE\n");

	printf("STARTING TO INSERT NODES\n");

	put_entry(balancedbst_p, &((ke){0}), &((ts){0, "zero"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){1}), &((ts){1, "one"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){2}), &((ts){2, "two"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){3}), &((ts){3, "three"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){4}), &((ts){4, "four"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){5}), &((ts){5, "five"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){6}), &((ts){6, "six"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){7}), &((ts){7, "seven"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){8}), &((ts){8, "eight"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){9}), &((ts){9, "nine"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){10}), &((ts){10, "ten"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){11}), &((ts){11, "eleven"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){12}), &((ts){12, "twelve"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){13}), &((ts){13, "thirteen"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){14}), &((ts){14, "fourteen"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){15}), &((ts){15, "fifteen"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){16}), &((ts){16, "sixteen"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){17}), &((ts){17, "seventeen"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){18}), &((ts){18, "eighteen"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	printf("COMPLETED INSERTING NODES\n");

	printf("STARTING TO REMOVE NODES\n");

	int error = 0;

	error = remove_value(balancedbst_p, &((ke){-1}));
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = remove_value(balancedbst_p, &((ke){0}));
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = remove_value(balancedbst_p, &((ke){1}));
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = remove_value(balancedbst_p, &((ke){2}));
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = remove_value(balancedbst_p, &((ke){3}));
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = remove_value(balancedbst_p, &((ke){4}));
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = remove_value(balancedbst_p, &((ke){5}));
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = remove_value(balancedbst_p, &((ke){6}));
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = remove_value(balancedbst_p, &((ke){7}));
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = remove_value(balancedbst_p, &((ke){8}));
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = remove_value(balancedbst_p, &((ke){9}));
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = remove_value(balancedbst_p, &((ke){10}));
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = remove_value(balancedbst_p, &((ke){11}));
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = remove_value(balancedbst_p, &((ke){12}));
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = remove_value(balancedbst_p, &((ke){13}));
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = remove_value(balancedbst_p, &((ke){14}));
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = remove_value(balancedbst_p, &((ke){15}));
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = remove_value(balancedbst_p, &((ke){16}));
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = remove_value(balancedbst_p, &((ke){17}));
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = remove_value(balancedbst_p, &((ke){18}));
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = remove_value(balancedbst_p, &((ke){19}));
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	printf("COMPLETED REMOVING NODES\n");

	printf("STARTING TO DELETE BINARY SEARCH TREE\n");

	delete_balancedbst(balancedbst_p);

	printf("COMPLETED DELETING BINARY SEARCH TREE\n");
	return 0;
}