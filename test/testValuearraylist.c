#include<value_arraylist.h>

#include<stdio.h>

declarations_value_arraylist(int_list, int)
#define EXPANSION_FACTOR 1.5
definitions_value_arraylist(int_list, int)

int compare_ints(const void* a, const void* b)
{
	int ai = *((const int*)a);
	int bi = *((const int*)b);
	return compare_numbers(ai, bi);
}

unsigned long long int get_radix_sort_attr(const int* a)
{
	return ((*((const int*)a)) + 1000000);
}

void sprint_int(dstring* append_str, const void* data_p, unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs);
	snprintf_dstring(append_str, "%d\n", *((const int*)data_p));
}

void print_int_list(const int_list* il_p)
{
	dstring append_str;
	init_empty_dstring(&append_str, 0);
	sprint_int_list(&append_str, il_p, sprint_int, 0);
	printf_dstring(&append_str);
	printf("\n");
	deinit_dstring(&append_str);
}

int main()
{
	int_list il;
	int_list* il_p = &il;
	initialize_int_list(il_p, 10);

	int set_val = 0;

	set_val = 5;
	push_back_to_int_list(il_p, &set_val);

	set_val = 4;
	push_front_to_int_list(il_p, &set_val);

	set_val = 3;
	push_front_to_int_list(il_p, &set_val);

	set_val = 6;
	push_back_to_int_list(il_p, &set_val);

	set_val = 7;
	push_back_to_int_list(il_p, &set_val);

	print_int_list(il_p);

	set_val = 0;
	push_back_to_int_list(il_p, &set_val);

	set_val = 1;
	push_back_to_int_list(il_p, &set_val);

	set_val = 2;
	push_front_to_int_list(il_p, &set_val);

	set_val = 9;
	push_back_to_int_list(il_p, &set_val);

	print_int_list(il_p);

	expand_int_list(il_p);

	print_int_list(il_p);

	heap_info hinfo = {MAX_HEAP, compare_ints};
	cy_uint degree = 2;

	heapify_int_list(il_p, &hinfo, degree);

	print_int_list(il_p);

	set_val = 8;
	push_to_heap_int_list(il_p, &hinfo, degree, &set_val);

	print_int_list(il_p);

	printf("printing the popped contents of heap\n");
	while(!is_empty_int_list(il_p))
	{
		const int* top = get_front_of_int_list(il_p);
		printf("%d\n", *top);
		pop_from_heap_int_list(il_p, &hinfo, degree);
	}

	print_int_list(il_p);

	shrink_int_list(il_p);

	print_int_list(il_p);

	reserve_capacity_for_int_list(il_p, 9);

	set_val = 5;
	push_front_to_int_list(il_p, &set_val);

	set_val = 4;
	push_front_to_int_list(il_p, &set_val);

	set_val = 3;
	push_back_to_int_list(il_p, &set_val);

	set_val = 6;
	push_front_to_int_list(il_p, &set_val);

	set_val = 7;
	push_front_to_int_list(il_p, &set_val);

	set_val = 0;
	push_back_to_int_list(il_p, &set_val);

	set_val = 1;
	push_front_to_int_list(il_p, &set_val);

	set_val = 2;
	push_front_to_int_list(il_p, &set_val);

	set_val = 9;
	push_back_to_int_list(il_p, &set_val);

	print_int_list(il_p);

	reserve_capacity_for_int_list(il_p, 12);

	print_int_list(il_p);

//#define USE_HEAP_SORT
#define USE_RADIX_SORT
//#define USE_MERGE_SORT

#if defined USE_HEAP_SORT
	printf("sorting - HEAP_SORT\n\n");
	heap_sort_int_list(il_p, 0, get_element_count_int_list(il_p)-1, compare_ints);
#elif defined USE_RADIX_SORT
	printf("sorting - RADIX_SORT\n\n");
	radix_sort_int_list(il_p, 0, get_element_count_int_list(il_p)-1, get_radix_sort_attr, il_p->mem_allocator);
#elif defined USE_MERGE_SORT
	printf("sorting - MERGE_SORT\n\n");
	merge_sort_int_list(il_p, 0, get_element_count_int_list(il_p)-1, compare_ints, il_p->mem_allocator);
#endif

	print_int_list(il_p);

	remove_all_from_int_list(il_p);

	print_int_list(il_p);

	reserve_capacity_for_int_list(il_p, 27);
	for(int i = 0; i < 24; i++)
	{
		if(i % 2)
			push_back_to_int_list(il_p, &i);
		else
			push_front_to_int_list(il_p, &i);
	}

	print_int_list(il_p);

	remove_elements_from_front_of_int_list(il_p, 3, 3);

	print_int_list(il_p);

	remove_elements_from_back_of_int_list(il_p, 3, 3);

	print_int_list(il_p);

	printf("failure removing more elements = %d\n", remove_elements_from_back_of_int_list(il_p, 1, 18));

	print_int_list(il_p);

	remove_elements_from_back_of_int_list(il_p, 0, get_element_count_int_list(il_p));

	print_int_list(il_p);

	deinitialize_int_list(il_p);

	return 0;
}