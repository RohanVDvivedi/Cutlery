#include<stdio.h>
#include<cutlery/cutlery_stds.h>
#include<cutlery/bst.h>

typedef struct teststruct ts;
struct teststruct
{
	int key;

	order_stat_bstnode os_bst_embed_node;

	bstnode bst_embed_node;
};

// note : how we compare the structures only based on their key values
int cmp(const void* data1, const void* data2)
{
	return (((ts*)data1)->key) - (((ts*)data2)->key);
}

void print_ts(const void* tsv)
{
	printf("%d ==> %"PRIu_cy_uint"", ((ts*)tsv)->key, ((ts*)tsv)->os_bst_embed_node.subtree_size);
}

void sprint_ts(dstring* append_str, const void* tsv, unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs + 1);
	snprintf_dstring(append_str, "%d ==> %"PRIu_cy_uint"", ((ts*)tsv)->key, ((ts*)tsv)->os_bst_embed_node.subtree_size);
}

void print_ts_bst(bst* bst_p)
{
	printf("ELEMENT_COUNT : %"PRIu_cy_uint"\n", get_element_count_bst(bst_p));
	dstring str;
	init_dstring(&str, "", 0);
	sprint_bst(&str, bst_p, sprint_ts, 0);
	printf_dstring(&str);
	deinit_dstring(&str);
	printf("\n");
}

#define TREE_TYPE_TO_USE 	/*NON_SELF_BALANCING*/ AVL_TREE /*RED_BLACK_TREE*/

#define TEST_COUNT 100
ts tests[TEST_COUNT] = {};

int main()
{
	printf("INITIALIZING BALANCED ORDER STATISTIC BINARY SEARCH TREE\n");

	bst my_bst;
	bst* bst_p = &my_bst;
	initialize_order_stat_bst(bst_p, TREE_TYPE_TO_USE, &simple_comparator(cmp), offsetof(ts, bst_embed_node), offsetof(ts, os_bst_embed_node));
	print_ts_bst(bst_p);

	printf("INSERTING ALL FROM 0 to %d\n", TEST_COUNT);
	for(int i = 0; i < TEST_COUNT; i++)
	{
		tests[i] = (ts){.key = i};
		insert_in_bst(bst_p, &(tests[i]));
	}
	print_ts_bst(bst_p);

	printf("PRINTING AT ALL INDICES\n");
	for(int i = 0; i < TEST_COUNT; i++)
	{
		const ts* tsv = get_element_at_index_in_bst(bst_p, i);
		int i2 = get_index_of_element_in_bst(bst_p, tsv);
		printf("%d %d %d\n", i, tsv->key, i2);
		if(i != tsv->key || tsv->key != i2)
			printf("there is a bug here\n");
	}

	printf("REMOVING EVERY N mod 3 != 0 FROM 0 to %d\n", TEST_COUNT);
	for(int i = 0; i < TEST_COUNT; i++)
	{
		if(i % 3 != 0)
			remove_from_bst(bst_p, &(tests[i]));
	}
	print_ts_bst(bst_p);

	printf("PRINTING AT ALL INDICES\n");
	for(int i = 0; i < TEST_COUNT; i++)
	{
		const ts* tsv = get_element_at_index_in_bst(bst_p, i);
		if(tsv == NULL)
			break;
		int i2 = get_index_of_element_in_bst(bst_p, tsv);
		printf("%d %d %d\n", i, tsv->key, i2);
		if(i != i2)
			printf("there is a bug here\n");
	}

	return 0;
}