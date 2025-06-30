#include<stdio.h>
#include<cutlery/cutlery_stds.h>
#include<cutlery/pheap.h>

typedef struct element element;
struct element
{
	int key;

	phpnode embed_node;
};

int cmp(const void* data1, const void* data2)
{
	return (((element*)data1)->key) - (((element*)data2)->key);
}

void print_ts(const void* tsv)
{
	if(tsv == NULL)
		printf("(null)");
	else
		printf("%d", ((element*)tsv)->key);
}

void sprint_ts(dstring* append_str, const void* tsv, unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs + 1);
	if(tsv == NULL)
		snprintf_dstring(append_str, "(null)");
	else
		snprintf_dstring(append_str, "%d", ((element*)tsv)->key);
}

void print_ts_pheap(const pheap* pheap_p)
{
	dstring str;
	init_dstring(&str, "", 0);
	sprint_pheap(&str, pheap_p, sprint_ts, 0);
	printf_dstring(&str);
	deinit_dstring(&str);
	printf("\n");
}

#define TEST_SIZE 400

element elements[TEST_SIZE];

int main()
{
	for(int i = 0; i < TEST_SIZE; i++)
	{
		elements[i].key = i / 3;
		initialize_phpnode(&(elements[i].embed_node));
	}

	pheap pheap_temp;
	pheap* pheap_p = &pheap_temp;

	initialize_pheap(pheap_p, MIN_HEAP, SKEW, &simple_comparator(&cmp), offsetof(element, embed_node));

	print_ts_pheap(pheap_p);

	for(int i = 0; i < TEST_SIZE / 100; i++)
		push_to_pheap(pheap_p, elements + i);

	return 0;
}