#include<array.h>

typedef struct teststruct ts;
struct teststruct
{
	int a;
	char* s;
};

void print_ts(void* tsv)
{
	printf("=> %d, %s", ((ts*)tsv)->a, ((ts*)tsv)->s);
}



int main()
{
	array* array_p = get_array(1, sizeof(ts));
	append_element(array_p, &((ts){1, "1"}));
	append_element(array_p, &((ts){2, "2"}));
	append_element(array_p, &((ts){3, "3"}));
	append_element(array_p, &((ts){4, "4"}));
	print_array(array_p, print_ts);
	delete_array(array_p);
	return 0;
}