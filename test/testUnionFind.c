#include<union_find.h>

#include<cutlery_stds.h>

#include<stdio.h>

typedef struct data_element data_element;
struct data_element
{
	int num;

	unsigned int element_count;

	// embedded union find node
	ufnode uf_embed_node;
};

int get_tens_digit(int num)
{
	return (num / 10) % 10;
}

void print_node_details(data_element* e, int print_root, union_find* uf_p)
{
	printf("%d at (@ %p, ufnode @ %p): parent : %p", e->num, e, &(e->uf_embed_node), e->uf_embed_node.parent);
	if(print_root)
		printf("and root : %p : element_count = %u\n", find_root_in_union_find(uf_p, e), ((const data_element*)find_root_in_union_find(uf_p, e))->element_count);
	else
		printf("\n");
}

void merge_with_managing_element_count(union_find* uf_p, const data_element* a, const data_element* b)
{
	//extract element count of the group a and group b
	unsigned int element_count_group_a = ((const data_element*)find_root_in_union_find(uf_p, a))->element_count;
	unsigned int element_count_group_b = ((const data_element*)find_root_in_union_find(uf_p, b))->element_count;

	// attempt a merge, if the merge is successfull a and b belong to the same group
	// update the element_count of the root of this new group to the sum of the element_counts of both the groups
	if(merge_groups_in_union_find(uf_p, a, b))
		((data_element*)find_root_in_union_find(uf_p, a))->element_count = element_count_group_a + element_count_group_b;
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
				merge_with_managing_element_count(uf_p, elements + i, elements + j);
		}
	}

	printf("printing all group pairs (group has all numbers with same digit in 10's place)\n");
	for(int i = 0; i < sizeof(elements)/sizeof(data_element); i++)
	{
		for(int j = i + 1; j < sizeof(elements)/sizeof(data_element); j++)
		{
			if(are_in_same_group(uf_p, elements + i, elements + j))
				printf("%d %d\n", elements[i].num, elements[j].num);
		}
	}

	printf("\n");

	print_node_details(elements + 0, 0, uf_p);
	print_node_details(elements + 8, 0, uf_p);

	printf("\n");

	printf("merge %d and %d\n", elements[0].num, elements[8].num);
	merge_with_managing_element_count(uf_p, elements + 0, elements + 8);

	printf("\n");

	print_node_details(elements + 0, 0, uf_p);
	print_node_details(elements + 8, 0, uf_p);

	printf("\n");

	print_node_details(elements + 7, 0, uf_p);

	printf("\n");

	printf("find root of %d\n", elements[7].num);
	find_root_in_union_find(uf_p, elements + 7);

	printf("\n");

	print_node_details(elements + 7, 0, uf_p);

	printf("path shortening tested\n");

	return 0;
}