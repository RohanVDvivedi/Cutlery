#include<stdio.h>
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
		printf(" NULL");
		return;
	}
	printf(" %d, %s", ((ts*)tsv)->a, ((ts*)tsv)->s);
}

int test_compare(const void* a, const void* b)
{
	if(a==NULL && b==NULL)
	{
		return 0;
	}
	else if(a!=NULL && b==NULL)
	{
		return 1;
	}
	else if(a==NULL && b!=NULL)
	{
		return -1;
	}
	else
	{
		return ((ts*)a)->a - ((ts*)b)->a;
	}
}

#define USE_STACK_MEMORY
//#define USE_HEAP_MEMORY

int main()
{
	#if defined USE_STACK_MEMORY
		printf("ARRAY WILL BE CREATED ON STACK MEMORY\n\n");
		array array_temp;
		array* array_p = &array_temp;
		initialize_array(array_p, 3);
	#elif defined USE_HEAP_MEMORY
		printf("ARRAY WILL BE CREATED ON HEAP MEMORY\n\n");
		array* array_p = get_array(3);
	#endif

	print_array(array_p, print_ts);

	set_element(array_p, &((ts){111, "def"}), 1);

	print_array(array_p, print_ts);

	expand_array(array_p);
	expand_array(array_p);

	print_array(array_p, print_ts);

	set_element(array_p, &((ts){333, "jkl"}), 3);
	set_element(array_p, &((ts){222, "ghi"}), 2);

	print_array(array_p, print_ts);

	set_element(array_p, &((ts){555, "one 0 3"}), 5);
	set_element(array_p, &((ts){100, "abc"}), 0);

	print_array(array_p, print_ts);

	set_element(array_p, &((ts){444, "mno"}), 4);
	set_element(array_p, &((ts){555, "pqr"}), 5);

	print_array(array_p, print_ts);

	set_element(array_p, &((ts){666, "stu"}), 6);

	print_array(array_p, print_ts);

	expand_array(array_p);
	expand_array(array_p);

	print_array(array_p, print_ts);

	set_element(array_p, &((ts){777, "7 th"}), 7);
	set_element(array_p, &((ts){888, "8 th"}), 8);

	print_array(array_p, print_ts);

	expand_array(array_p);
	expand_array(array_p);

	print_array(array_p, print_ts);

	expand_array(array_p);
	expand_array(array_p);

	print_array(array_p, print_ts);

	expand_array(array_p);

	print_array(array_p, print_ts);

	expand_array(array_p);

	print_array(array_p, print_ts);

	expand_array(array_p);

	print_array(array_p, print_ts);

	shrink_array(array_p, 3, 7);

	print_array(array_p, print_ts);


	ts to_find = {444, "lol"};
	printf("Finding data where a = %d\n", to_find.a);
	printf("Linear search : ");
	print_ts(get_element(array_p, linear_search_in_array(array_p, 0, array_p->total_size - 1, ((void*)(&to_find)), test_compare)));
	printf("\nBinary search : ");
	print_ts(get_element(array_p, binary_search_in_array(array_p, 0, array_p->total_size - 1, ((void*)(&to_find)), test_compare)));
	printf("\n");


	// testing sort


	#define test_sort_size 123

	while(array_p->total_size < test_sort_size)
	{
		expand_array(array_p);
	}

	ts ts_ss[test_sort_size];

	for(int i = 0; i < test_sort_size; i++)
	{
		ts_ss[i] = ((ts){rand() % test_sort_size, "XXX-dont care"});
		set_element(array_p, ts_ss + i, i);
	}

	print_array(array_p, print_ts);



	#if defined USE_STACK_MEMORY
		deinitialize_array(array_p);
	#elif defined USE_HEAP_MEMORY
		delete_array(array_p);
	#endif

	return 0;
}