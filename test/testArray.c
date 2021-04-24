#include<stdio.h>
#include<stdlib.h>

#include<array.h>

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

void sprint_ts(dstring* append_str, const void* tsv)
{
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

unsigned int get_radix_sort_attr(const void* a)
{
	return ((ts*)a)->a;
}

void print_ts_array(array* array_p)
{
	dstring str;
	init_dstring(&str, "", 0);
	sprint_array(&str, array_p, sprint_ts, 5, 1);
	printf_dstring(&str);
	deinit_dstring(&str);
	printf("\n");
}

int main()
{
	array array_temp;
	array* array_p = &array_temp;
	initialize_array(array_p, 3);

	print_ts_array(array_p);

	set_element(array_p, &((ts){111, "def"}), 1);

	print_ts_array(array_p);

	expand_array(array_p);
	expand_array(array_p);

	print_ts_array(array_p);

	set_element(array_p, &((ts){333, "jkl"}), 3);
	set_element(array_p, &((ts){222, "ghi"}), 2);

	print_ts_array(array_p);

	set_element(array_p, &((ts){555, "one 0 3"}), 5);
	set_element(array_p, &((ts){100, "abc"}), 0);

	print_ts_array(array_p);

	set_element(array_p, &((ts){444, "mno"}), 4);
	set_element(array_p, &((ts){555, "pqr"}), 5);

	print_ts_array(array_p);

	set_element(array_p, &((ts){666, "stu"}), 6);

	print_ts_array(array_p);

	expand_array(array_p);
	expand_array(array_p);

	print_ts_array(array_p);

	set_element(array_p, &((ts){777, "7 th"}), 7);
	set_element(array_p, &((ts){888, "8 th"}), 8);

	print_ts_array(array_p);

	expand_array(array_p);

	print_ts_array(array_p);

	expand_array(array_p);

	print_ts_array(array_p);

	expand_array(array_p);

	print_ts_array(array_p);

	shrink_array(array_p, 5);

	print_ts_array(array_p);


	ts to_find = {444, "lol"};
	printf("Finding data where a = %d\n", to_find.a);
	printf("Linear search : ");
	print_ts(get_element(array_p, linear_search_in_array(array_p, 0, array_p->total_size - 1, ((void*)(&to_find)), test_compare)));
	printf("\nBinary search : ");
	print_ts(get_element(array_p, binary_search_in_array(array_p, 0, array_p->total_size - 1, ((void*)(&to_find)), test_compare)));
	printf("\n");


	// testing sort
	#define sort_array_size 50

	#define test_sort_size  30

	#define start_index     9
	#define end_index       (start_index + test_sort_size - 1)

	// expand existing array
	while(array_p->total_size < sort_array_size)
		expand_array(array_p);
	// initialize array
	ts ts_ss[sort_array_size];
	for(int i = 0; i < sort_array_size; i++)
	{
		ts_ss[i] = ((ts){rand() % sort_array_size, "XXX-dont care"});
		set_element(array_p, ts_ss + i, i);
	}

	printf("Array initialized\n\n");print_ts_array(array_p);printf("\n\n");

//#define MERGE_SORT
#define HEAP_SORT
//#define RADIX_SORT

#if defined MERGE_SORT
	printf("Sorting %u to %u using MERGE_SORT\n\n", start_index, end_index);
	merge_sort_array(array_p, start_index, end_index, test_compare);
#elif defined HEAP_SORT
	printf("Sorting %u to %u using HEAP_SORT\n\n", start_index, end_index);
	heap_sort_array(array_p, start_index, end_index, test_compare);
#elif defined RADIX_SORT
	printf("Sorting %u to %u using RADIX_SORT\n\n", start_index, end_index);
	radix_sort_array(array_p, start_index, end_index, get_radix_sort_attr);
#else
	printf("No sort algorithm defined\n\n")
	return 0;
#endif

	printf("Array sorted %d <-> %d\n\n", start_index, end_index);print_ts_array(array_p);printf("\n\n");

	printf("Executing Search of all\n\n");

	for(int i = 0; i < test_sort_size; i++)
	{
		ts to_find = {i, "lol"};
		unsigned int index;
		printf("Finding data where a = %d\n", to_find.a);

		printf("Linear search : ");
		index = linear_search_in_array(array_p, start_index, end_index, ((void*)(&to_find)), test_compare);
		printf("%u : ", index);
		if(index != array_p->total_size)
			print_ts(get_element(array_p, index));
		else
			printf("Not found");

		printf("\nBinary search : ");
		index = binary_search_in_array(array_p, start_index, end_index, ((void*)(&to_find)), test_compare);
		printf("%u : ", index);
		if(index != array_p->total_size)
			print_ts(get_element(array_p, index));
		else
			printf("Not found");

		printf("\n\n");
	}

	deinitialize_array(array_p);

	return 0;
}