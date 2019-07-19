#include<mmarray.h>

typedef struct teststruct ts;
struct teststruct
{
	int a;
	char* s;
};

void print_ts(const void* tsv)
{
	printf(" %d, %s", ((ts*)tsv)->a, ((ts*)tsv)->s);
}

int main()
{
	mmarray* mmarray_p = get_mmarray(3, sizeof(ts));

	print_mmarray(mmarray_p, print_ts);

	set_mmarray_element(mmarray_p, &((ts){111, "def"}), 1);

	print_mmarray(mmarray_p, print_ts);

	expand_mmarray(mmarray_p);

	print_mmarray(mmarray_p, print_ts);

	set_mmarray_element(mmarray_p, &((ts){333, "jkl"}), 3);
	set_mmarray_element(mmarray_p, &((ts){222, "ghi"}), 2);

	print_mmarray(mmarray_p, print_ts);

	set_mmarray_element(mmarray_p, &((ts){555, "one 0 3"}), 5);
	set_mmarray_element(mmarray_p, &((ts){100, "abc"}), 0);

	print_mmarray(mmarray_p, print_ts);

	set_mmarray_element(mmarray_p, &((ts){444, "mno"}), 4);
	set_mmarray_element(mmarray_p, &((ts){555, "pqr"}), 5);

	print_mmarray(mmarray_p, print_ts);

	set_mmarray_element(mmarray_p, &((ts){666, "stu"}), 6);

	print_mmarray(mmarray_p, print_ts);

	delete_mmarray(mmarray_p);

	return 0;
}