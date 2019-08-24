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
	balancedbst* balancedbst_p = get_balancedbst(NON_SELF_BALANCING, key_cmp);
	print_balancedbst(balancedbst_p, print_key, print_ts);

	put_entry(balancedbst_p, &((ke){1}), &((ts){100, "one"}));
	print_balancedbst(balancedbst_p, print_key, print_ts);

	delete_balancedbst(balancedbst_p);
	return 0;
}