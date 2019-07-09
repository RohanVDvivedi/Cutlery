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
	tree* tree_p = get_tree(3, sizeof(ts), &((ts){0, "zero"}));

	add_child(tree_p, tree_p->root_node, &((ts){1, "one"}), 0);
	print_tree(tree_p, print_ts);

	delete_tree(tree_p);
	return 0;
}