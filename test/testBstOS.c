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
	snprintf_dstring("%d ==> %"PRIu_cy_uint"", ((ts*)tsv)->key, ((ts*)tsv)->os_bst_embed_node.subtree_size);
}

void print_ts_bst(bst* bst_p)
{
	dstring str;
	init_dstring(&str, "", 0);
	sprint_bst(&str, bst_p, sprint_ts, 0);
	printf_dstring(&str);
	deinit_dstring(&str);
	printf("\n");
}

#define TREE_TYPE_TO_USE 	/*NON_SELF_BALANCING*/ AVL_TREE /*RED_BLACK_TREE*/

int main()
{
	printf("INITIALIZING BALANCED ORDER STATISTIC BINARY SEARCH TREE\n");

	bst my_bst;
	bst* bst_p = &my_bst;
	initialize_order_stat_bst(bst_p, TREE_TYPE_TO_USE, &simple_comparator(cmp), offsetof(ts, bst_embed_node), offsetof(ts, os_bst_embed_node));
	print_ts_bst(bst_p);

	return 0;
}