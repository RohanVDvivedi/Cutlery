#include<tree.h>

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
	tree* tree_p = get_tree(3, sizeof(teststruct));

	delete_tree();
	return 0;
}