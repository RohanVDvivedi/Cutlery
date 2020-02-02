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

void print_smallest(const balancedbst* balancedbst_p)
{
	printf("the value for the smallest key is : ");
	const void* result = find_value_with_smallest_key(balancedbst_p);
	if(result != NULL)
	{
		print_ts(result);
	}
	else
	{
		printf("NULL");
	}
	printf("\n");
}

void print_largest(const balancedbst* balancedbst_p)
{
	printf("the value for the largest key is : ");
	const void* result = find_value_with_largest_key(balancedbst_p);
	if(result != NULL)
	{
		print_ts(result);
	}
	else
	{
		printf("NULL");
	}
	printf("\n");
}

void print_succeeding_equals(const balancedbst* balancedbst_p, int k)
{
	printf("the value succeeding %d is : ", k);
	const void* result = find_value_succeeding_or_equals(balancedbst_p, &((ke){k}));
	if(result != NULL)
	{
		print_ts(result);
	}
	else
	{
		printf("NULL");
	}
	printf("\n");
}

void print_preceding_equals(const balancedbst* balancedbst_p, int k)
{
	printf("the value preceding %d is : ", k);
	const void* result = find_value_preceding_or_equals(balancedbst_p, &((ke){k}));
	if(result != NULL)
	{
		print_ts(result);
	}
	else
	{
		printf("NULL");
	}
	printf("\n");
}

int main()
{
	printf("INITIALIZING BALANCED BINARY SEARCH TREE\n");

	balancedbst* balancedbst_p = get_balancedbst(/*NON_SELF_BALANCING*/ /*AVL_TREE*/ RED_BLACK_TREE, key_cmp);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	printf("COMPLETED INITIALIZING BALANCED BINARY SEARCH TREE\n");

	print_smallest(balancedbst_p);
	print_largest(balancedbst_p);

	printf("STARTING TO INSERT NODES\n");

	insert_entry_in_bst(balancedbst_p, &((ke){18}), &((ts){18, "eighteen"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	insert_entry_in_bst(balancedbst_p, &((ke){17}), &((ts){17, "seventeen"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	insert_entry_in_bst(balancedbst_p, &((ke){16}), &((ts){16, "sixteen"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	insert_entry_in_bst(balancedbst_p, &((ke){15}), &((ts){15, "fifteen"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	insert_entry_in_bst(balancedbst_p, &((ke){14}), &((ts){14, "fourteen"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	insert_entry_in_bst(balancedbst_p, &((ke){13}), &((ts){13, "thirteen"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	insert_entry_in_bst(balancedbst_p, &((ke){12}), &((ts){12, "twelve"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	print_smallest(balancedbst_p);
	print_largest(balancedbst_p);

	insert_entry_in_bst(balancedbst_p, &((ke){11}), &((ts){11, "eleven"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	insert_entry_in_bst(balancedbst_p, &((ke){10}), &((ts){10, "ten"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	insert_entry_in_bst(balancedbst_p, &((ke){9}), &((ts){9, "nine"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	insert_entry_in_bst(balancedbst_p, &((ke){8}), &((ts){8, "eight"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	insert_entry_in_bst(balancedbst_p, &((ke){0}), &((ts){0, "zero"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	insert_entry_in_bst(balancedbst_p, &((ke){1}), &((ts){1, "one"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	insert_entry_in_bst(balancedbst_p, &((ke){2}), &((ts){2, "two"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	insert_entry_in_bst(balancedbst_p, &((ke){3}), &((ts){3, "three"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	print_smallest(balancedbst_p);
	print_largest(balancedbst_p);

	insert_entry_in_bst(balancedbst_p, &((ke){4}), &((ts){4, "four"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	insert_entry_in_bst(balancedbst_p, &((ke){5}), &((ts){5, "five"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	insert_entry_in_bst(balancedbst_p, &((ke){6}), &((ts){6, "six"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	print_preceding_equals(balancedbst_p, 7);
	print_succeeding_equals(balancedbst_p, 7);

	insert_entry_in_bst(balancedbst_p, &((ke){7}), &((ts){7, "seven"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	insert_entry_in_bst(balancedbst_p, &((ke){19}), &((ts){19, "nineteen"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	update_value_in_bst(balancedbst_p, &((ke){19}), &((ts){19, "LOL"}), NULL);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	insert_entry_in_bst(balancedbst_p, &((ke){9}), &((ts){9, "nine-lol"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	printf("COMPLETED INSERTING NODES\n");

	printf("STARTING TO REMOVE NODES\n");

	int error = 0;

	error = delete_entry_from_bst(balancedbst_p, &((ke){-1}), NULL, NULL);
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = delete_entry_from_bst(balancedbst_p, &((ke){0}), NULL, NULL);
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = delete_entry_from_bst(balancedbst_p, &((ke){1}), NULL, NULL);
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = delete_entry_from_bst(balancedbst_p, &((ke){2}), NULL, NULL);
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = delete_entry_from_bst(balancedbst_p, &((ke){3}), NULL, NULL);
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = delete_entry_from_bst(balancedbst_p, &((ke){4}), NULL, NULL);
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = delete_entry_from_bst(balancedbst_p, &((ke){5}), NULL, NULL);
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	print_smallest(balancedbst_p);
	print_largest(balancedbst_p);

	error = delete_entry_from_bst(balancedbst_p, &((ke){6}), NULL, NULL);
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = delete_entry_from_bst(balancedbst_p, &((ke){7}), NULL, NULL);
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = delete_entry_from_bst(balancedbst_p, &((ke){8}), NULL, NULL);
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = delete_entry_from_bst(balancedbst_p, &((ke){9}), NULL, NULL);
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = delete_entry_from_bst(balancedbst_p, &((ke){10}), NULL, NULL);
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = delete_entry_from_bst(balancedbst_p, &((ke){11}), NULL, NULL);
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = delete_entry_from_bst(balancedbst_p, &((ke){12}), NULL, NULL);
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = delete_entry_from_bst(balancedbst_p, &((ke){13}), NULL, NULL);
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	print_smallest(balancedbst_p);
	print_largest(balancedbst_p);

	error = delete_entry_from_bst(balancedbst_p, &((ke){14}), NULL, NULL);
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = delete_entry_from_bst(balancedbst_p, &((ke){15}), NULL, NULL);
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = delete_entry_from_bst(balancedbst_p, &((ke){16}), NULL, NULL);
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = delete_entry_from_bst(balancedbst_p, &((ke){17}), NULL, NULL);
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = delete_entry_from_bst(balancedbst_p, &((ke){18}), NULL, NULL);
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	error = delete_entry_from_bst(balancedbst_p, &((ke){19}), NULL, NULL);
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	print_smallest(balancedbst_p);
	print_largest(balancedbst_p);

	error = delete_entry_from_bst(balancedbst_p, &((ke){9}), NULL, NULL);
	printf("nodes deleted = %d\n", error);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	print_smallest(balancedbst_p);
	print_largest(balancedbst_p);

	printf("COMPLETED REMOVING NODES\n");

	printf("STARTING TO DELETE BINARY SEARCH TREE\n");

	delete_balancedbst(balancedbst_p);

	printf("COMPLETED DELETING BINARY SEARCH TREE\n");
	return 0;
}