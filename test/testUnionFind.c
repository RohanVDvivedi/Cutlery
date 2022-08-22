#include<union_find.h>

#include<cutlery_stds.h>

#include<stdio.h>

typedef struct data_element data_element;
struct data_element
{
	int num;

	// embedded union find node
	ufnode uf_embed_node;
};

int get_tens_digit(int num)
{
	return (num / 10) % 10;
}

int main()
{
	// initialize all data elements
	data_element elements[] = {
		{0}, {1}, {2}, {7}, {10}, {11}, {12}, {13}, {14}, {20}, {21}, {22}, {29},
	};

	// initialize all their embedded nodes
	for(int i = 0; i < sizeof(elements)/sizeof(data_element); i++)
		initialize_ufnode(&(elements[i].uf_embed_node));

	union_find uf_temp;
	union_find* uf_p = &uf_temp;

	initialize_union_find(uf_p, offsetof(data_element, uf_embed_node));

	// group all numbers that have the same digit in ten's place
	for(int i = 0; i < sizeof(elements)/sizeof(data_element); i++)
	{
		for(int j = i + 1; j < sizeof(elements)/sizeof(data_element); j++)
		{
			if(get_tens_digit(elements[i].num) == get_tens_digit(elements[j].num))
				merge_groups_in_union_find(uf_p, elements + i, elements + j);
		}
	}

	printf("printing all group pairs (group has all numbers with same digit in 10's place)\n");
	for(int i = 0; i < sizeof(elements)/sizeof(data_element); i++)
	{
		for(int j = i + 1; j < sizeof(elements)/sizeof(data_element); j++)
		{
			if(find_root_in_union_find(uf_p, elements + i) == find_root_in_union_find(uf_p, elements + j))
				printf("%d %d\n", elements[i].num, elements[j].num);
		}
	}

	printf("parent of %d => %p\n", elements[0].num, elements[0].uf_embed_node.parent);
	printf("parent of %d => %p\n", elements[8].num, elements[8].uf_embed_node.parent);

	printf("merge %d and %d\n", elements[0].num, elements[8].num);
	merge_groups_in_union_find(uf_p, elements + 0, elements + 8);

	printf("parent of %d => %p\n", elements[0].num, elements[0].uf_embed_node.parent);
	printf("parent of %d => %p\n", elements[8].num, elements[8].uf_embed_node.parent);

	printf("find root of %d\n", elements[8].num);
	find_root_in_union_find(uf_p, elements + 8);

	printf("parent of %d => %p\n", elements[0].num, elements[0].uf_embed_node.parent);
	printf("parent of %d => %p\n", elements[8].num, elements[8].uf_embed_node.parent);

	printf("path shortening tested\n");

	return 0;
}