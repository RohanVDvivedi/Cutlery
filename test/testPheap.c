#include<stdio.h>
#include<stdlib.h>
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

#define TEST_SIZE 600

element elements[TEST_SIZE];

#define T1 MIN_HEAP /*MAX_HEAP*/
#define T2 SKEW /*LEFTIST*/

void print_tabs(int t)
{
	while(t)
	{
		printf("  ");
		t--;
	}
}

void print_tree_level_wise(const phpnode* node_p, int t)
{
	if(node_p == NULL)
		return;
	const element* temp = ((const void*)node_p) - offsetof(element, embed_node);
	print_tabs(t);
	printf("%d\n", temp->key);
	print_tree_level_wise(temp->embed_node.left, t+1);
	print_tree_level_wise(temp->embed_node.right, t+1);
}

int main()
{
	for(int i = 0; i < TEST_SIZE; i++)
	{
		elements[i].key = i / 3;
		initialize_phpnode(&(elements[i].embed_node));
	}

	pheap pheap_temp;
	pheap* pheap_p = &pheap_temp;

	initialize_pheap(pheap_p, T1, T2, &simple_comparator(&cmp), offsetof(element, embed_node));

	print_ts_pheap(pheap_p);

	// 0 to 99 in ascending order
	for(int i = 0; i <= 99; i++)
		push_to_pheap(pheap_p, elements + i);

	// 300 to 399 in descending order
	for(int i = 399; i >= 300; i--)
		push_to_pheap(pheap_p, elements + i);

	// 100 to 199 random inserts
	{
		int seen[100]= {};
		int seen_count = 0;
		while(seen_count < 100)
		{
			int i = ((unsigned int)rand()) % 100;
			if(seen[i])
				continue;
			seen[i] = 1;
			seen_count++;
			push_to_pheap(pheap_p, elements + i + 100);
		}
	}

	// 400 to 499 in ascending order
	for(int i = 400; i <= 499; i++)
		push_to_pheap(pheap_p, elements + i);

	// 500 to 599 random inserts
	{
		int seen[100] = {};
		int seen_count = 0;
		while(seen_count < 100)
		{
			int i = ((unsigned int)rand()) % 100;
			if(seen[i])
				continue;
			seen[i] = 1;
			seen_count++;
			push_to_pheap(pheap_p, elements + i + 500);
		}
	}

	// 200 to 299 in descending order
	for(int i = 299; i >= 200; i--)
		push_to_pheap(pheap_p, elements + i);

	print_ts_pheap(pheap_p);

	element e1;
	e1.key = 3;
	initialize_phpnode(&(e1.embed_node));
	push_to_pheap(pheap_p, &e1);

	element e2;
	e2.key = 160;
	initialize_phpnode(&(e2.embed_node));
	push_to_pheap(pheap_p, &e2);

	e1.key = 2000;
	heapify_for_in_pheap(pheap_p, &e1);

	e2.key = -2000;
	heapify_for_in_pheap(pheap_p, &e2);

	print_tree_level_wise(pheap_p->root, 0);

	{
		const element* e = get_top_of_pheap(pheap_p);
		int popped = pop_from_pheap(pheap_p);
		printf("first pop = %d\n", e->key);
		if(!popped)
		{
			printf("ELEMENT NOT POPPED\n");
			exit(-1);
		}
	}

	for(int i = 0; i < TEST_SIZE; i++)
	{
		const element* e = get_top_of_pheap(pheap_p);
		int popped = pop_from_pheap(pheap_p);
		if(T1 == MIN_HEAP)
		{
			if(e->key != (i/3))
			{
				printf("ERROR IN PHEAP, value = %d, expected = %d\n", e->key, (i/3));
				exit(-1);
			}
		}
		else
		{
			if(e->key != ((599-i)/3))
			{
				printf("ERROR IN PHEAP, value = %d, expected = %d\n", e->key, ((599-i)/3));
				exit(-1);
			}
		}
		if(!popped)
		{
			printf("ELEMENT NOT POPPED\n");
			exit(-1);
		}
	}

	{
		const element* e = get_top_of_pheap(pheap_p);
		int popped = pop_from_pheap(pheap_p);
		printf("last pop = %d\n", e->key);
		if(!popped)
		{
			printf("ELEMENT NOT POPPED\n");
			exit(-1);
		}
	}

	print_ts_pheap(pheap_p);

	if(!is_empty_pheap(pheap_p))
	{
		printf("PHEAP STILL NOT EMPTY\n");
		exit(-1);
	}

	printf("TESTS PASSED SUCCESSFULLY\n");
	return 0;
}