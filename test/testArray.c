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
	if(a==NULL || b==NULL)
	{
		return -1;
	}
	else
	{
		return ((ts*)a)->a - ((ts*)b)->a;
	}
}

int main()
{
	array* array_p = get_array(3);

	print_array(array_p, print_ts);

	set_element(array_p, &((ts){111, "def"}), 1);

	print_array(array_p, print_ts);

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

	ts to_find = {444, "lol"};
	printf("Finding data where a = %d\n", to_find.a);
	print_ts(get_element(array_p, find_first_in_array(array_p, ((void*)(&to_find)), test_compare)));
	printf("\n");

	delete_array(array_p);

	return 0;
}