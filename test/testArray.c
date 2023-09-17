#include<stdio.h>
#include<stdlib.h>

#include<cutlery_stds.h>
#include<array.h>
#include<arraylist.h>

typedef struct teststruct ts;
struct teststruct
{
	int a;
	char* s;
};

void print_ts(const void* tsv)
{
	if(tsv == NULL)
	{
		printf("NULL");
		return;
	}
	printf("%d, %s", ((ts*)tsv)->a, ((ts*)tsv)->s);
}

void sprint_ts(dstring* append_str, const void* tsv, unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs + 1);
	snprintf_dstring(append_str, "%d, %s", ((ts*)tsv)->a, ((ts*)tsv)->s);
}

int test_compare(const void* a, const void* b)
{
	if(a==NULL && b==NULL)
	{
		return 0;
	}
	else if(a!=NULL && b==NULL)
	{
		return -1;
	}
	else if(a==NULL && b!=NULL)
	{
		return 1;
	}
	else
	{
		return ((ts*)a)->a - ((ts*)b)->a;
	}
}

const comparator_interface test_comparator = simple_comparator(test_compare);

unsigned long long int get_radix_sort_attr(const void* a)
{
	return ((ts*)a)->a;
}

void print_ts_array(array* array_p)
{
	dstring str;
	init_dstring(&str, "", 0);
	sprint_array(&str, array_p, sprint_ts, 0);
	printf_dstring(&str);
	deinit_dstring(&str);
	printf("\n");
}

int main()
{
	printf("MAX_ARRAY_CAPACITY() = %llu, %llx\n\n", MAX_ARRAY_CAPACITY, MAX_ARRAY_CAPACITY);

	array array_temp;
	array* array_p = &array_temp;
	initialize_array(array_p, 3);

	print_ts_array(array_p);

	set_in_array(array_p, &((ts){111, "def"}), 1);

	print_ts_array(array_p);

	expand_array(array_p);
	expand_array(array_p);

	print_ts_array(array_p);

	set_in_array(array_p, &((ts){333, "jkl"}), 3);
	set_in_array(array_p, &((ts){222, "ghi"}), 2);

	print_ts_array(array_p);

	set_in_array(array_p, &((ts){555, "one 0 3"}), 5);
	set_in_array(array_p, &((ts){100, "abc"}), 0);

	print_ts_array(array_p);

	set_in_array(array_p, &((ts){444, "mno"}), 4);
	set_in_array(array_p, &((ts){555, "pqr"}), 5);

	print_ts_array(array_p);

	set_in_array(array_p, &((ts){666, "stu"}), 6);

	print_ts_array(array_p);

	expand_array(array_p);
	expand_array(array_p);

	print_ts_array(array_p);

	set_in_array(array_p, &((ts){777, "7 th"}), 7);
	set_in_array(array_p, &((ts){888, "8 th"}), 8);

	print_ts_array(array_p);

	reserve_capacity_for_array(array_p, 105);

	print_ts_array(array_p);

	shrink_array(array_p, 5);

	print_ts_array(array_p);


	ts to_find = {444, "lol"};
	printf("Finding data where a = %d\n", to_find.a);
	printf("Linear search : ");
	index_accessed_interface array_iai = get_index_accessed_interface_for_array(array_p);
	print_ts(get_from_array(array_p, linear_search_in_iai(&array_iai, 0, get_capacity_array(array_p) - 1, ((void*)(&to_find)), &test_comparator, FIRST_OCCURENCE)));
	printf("\n");


	// testing sort
	#define sort_array_size 50

	#define test_sort_size  30

	#define start_index     9
	#define end_index       (start_index + test_sort_size - 1)

	// expand existing array, to atleast_capacity of sort_array_size
	reserve_capacity_for_array(array_p, sort_array_size);

	// initialize array
	ts ts_ss[sort_array_size];
	for(int i = 0; i < sort_array_size; i++)
	{
		ts_ss[i] = ((ts){rand() % sort_array_size, "XXX-dont care"});
		set_in_array(array_p, ts_ss + i, i);
	}

	printf("Array initialized\n\n");print_ts_array(array_p);printf("\n\n");

//#define MERGE_SORT
//#define HEAP_SORT
//#define HEAP_SORT_native
//#define QUICK_SORT
#define RADIX_SORT
//#define BUBBLE_SORT
//#define INSERTION_SORT

	array_iai = get_index_accessed_interface_for_array(array_p);
	index_accessed_interface* array_iai_p = &array_iai;

	printf("printf if is_sorted(%u %u) = %d\n\n", start_index, end_index, is_sorted_iai(array_iai_p, start_index, end_index, &test_comparator));

#if defined MERGE_SORT
	printf("Sorting %u to %u using MERGE_SORT\n\n", start_index, end_index);
	merge_sort_iai(array_iai_p, start_index, end_index, &test_comparator, STD_C_mem_allocator);
#elif defined HEAP_SORT
	printf("Sorting %u to %u using HEAP_SORT\n\n", start_index, end_index);
	heap_sort_iai(array_iai_p, start_index, end_index, &test_comparator, STD_C_mem_allocator);
#elif defined HEAP_SORT_native
	printf("Sorting %u to %u using HEAP_SORT_native\n\n", start_index, end_index);
	arraylist temp_al;
	if(!get_slice_as_arraylist_from_array(&temp_al, array_p, start_index, test_sort_size))
		exit(-1);
	heap_sort_arraylist(&temp_al, 0, test_sort_size-1, &test_comparator);
#elif defined QUICK_SORT
	printf("Sorting %u to %u using QUICK_SORT\n\n", start_index, end_index);
	quick_sort_iai(array_iai_p, start_index, end_index, &test_comparator);
#elif defined RADIX_SORT
	printf("Sorting %u to %u using RADIX_SORT\n\n", start_index, end_index);
	radix_sort_iai(array_iai_p, start_index, end_index, get_radix_sort_attr, STD_C_mem_allocator);
#elif defined BUBBLE_SORT
	printf("Sorting %u to %u using BUBBLE_SORT\n\n", start_index, end_index);
	bubble_sort_iai(array_iai_p, start_index, end_index, &test_comparator);
#elif defined INSERTION_SORT
	printf("Sorting %u to %u using INSERTION_SORT\n\n", start_index, end_index);
	insertion_sort_iai(array_iai_p, start_index, end_index, &test_comparator);
#else
	printf("No sort algorithm defined\n\n")
	return 0;
#endif

	printf("Array sorted %d <-> %d\n\n", start_index, end_index);print_ts_array(array_p);printf("\n\n");

	printf("is_sorted(%u %u) = %d\n\n", start_index, end_index, is_sorted_iai(array_iai_p, start_index, end_index, &test_comparator));

	printf("yet is_sorted(%u %u) = %d\n\n", 0, sort_array_size - 1, is_sorted_iai(array_iai_p, 0, sort_array_size - 1, &test_comparator));

	printf("Executing Search of all\n\n");

	for(int i = 0; i < sort_array_size; i++)
	{
		ts to_find = {i, "lol"};
		cy_uint index;
		printf("Finding data where a = %d\n", to_find.a);

		printf("Linear search (First occurence) : ");
		index = linear_search_in_iai(array_iai_p, start_index, end_index, ((void*)(&to_find)), &test_comparator, FIRST_OCCURENCE);
		if(index != INVALID_INDEX)
		{
			printf("%llu : ", index);
			print_ts(get_from_array(array_p, index));
		}
		else
			printf("Not found");
		printf("\n");

		printf("Binary search (First occurence) : ");
		index = binary_search_in_sorted_iai(array_iai_p, start_index, end_index, ((void*)(&to_find)), &test_comparator, FIRST_OCCURENCE);
		if(index != INVALID_INDEX)
		{
			printf("%llu : ", index);
			print_ts(get_from_array(array_p, index));
		}
		else
			printf("Not found");
		printf("\n\n");

		printf("Linear search (Last occurence) : ");
		index = linear_search_in_iai(array_iai_p, start_index, end_index, ((void*)(&to_find)), &test_comparator, LAST_OCCURENCE);
		if(index != INVALID_INDEX)
		{
			printf("%llu : ", index);
			print_ts(get_from_array(array_p, index));
		}
		else
			printf("Not found");
		printf("\n");

		printf("Binary search (Last occurence) : ");
		index = binary_search_in_sorted_iai(array_iai_p, start_index, end_index, ((void*)(&to_find)), &test_comparator, LAST_OCCURENCE);
		if(index != INVALID_INDEX)
		{
			printf("%llu : ", index);
			print_ts(get_from_array(array_p, index));
		}
		else
			printf("Not found");
		printf("\n\n");

		printf("Find preceding : ");
		index = find_preceding_in_sorted_iai(array_iai_p, start_index, end_index, ((void*)(&to_find)), &test_comparator);
		if(index != INVALID_INDEX)
		{
			printf("%llu : ", index);
			print_ts(get_from_array(array_p, index));
		}
		else
			printf("Not found");
		printf("\n\n");

		printf("Find preceding or equals : ");
		index = find_preceding_or_equals_in_sorted_iai(array_iai_p, start_index, end_index, ((void*)(&to_find)), &test_comparator);
		if(index != INVALID_INDEX)
		{
			printf("%llu : ", index);
			print_ts(get_from_array(array_p, index));
		}
		else
			printf("Not found");
		printf("\n\n");

		printf("Find succeeding : ");
		index = find_succeeding_in_sorted_iai(array_iai_p, start_index, end_index, ((void*)(&to_find)), &test_comparator);
		if(index != INVALID_INDEX)
		{
			printf("%llu : ", index);
			print_ts(get_from_array(array_p, index));
		}
		else
			printf("Not found");
		printf("\n\n");

		printf("Find succeeding or equals : ");
		index = find_succeeding_or_equals_in_sorted_iai(array_iai_p, start_index, end_index, ((void*)(&to_find)), &test_comparator);
		if(index != INVALID_INDEX)
		{
			printf("%llu : ", index);
			print_ts(get_from_array(array_p, index));
		}
		else
			printf("Not found");
		printf("\n\n");

		printf("Find insertion index : ");
		index = find_insertion_index_in_sorted_iai(array_iai_p, start_index, end_index, ((void*)(&to_find)), &test_comparator);
		printf("%llu : ", index);
		print_ts(get_from_array(array_p, index));
		printf("\n\n");

		printf("\n\n");
		printf("-----------------------------------------------------------------\n\n");
		printf("\n\n");
	}

	printf("before removing all from array\n");
	print_ts_array(array_p);

	remove_all_from_array(array_p);

	printf("after removing all from array\n");
	print_ts_array(array_p);

	deinitialize_array(array_p);

	return 0;
}