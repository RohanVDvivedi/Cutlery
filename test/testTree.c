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
	tree* tree_p = get_tree(3, &((ts){0, "zero"}));
	print_tree(tree_p, print_ts);

	set_child(tree_p, tree_p->root_node, &((ts){1, "one"}), 0);
	print_tree(tree_p, print_ts);

	set_child(tree_p, tree_p->root_node, &((ts){2, "two"}), 1);
	print_tree(tree_p, print_ts);

	set_child(tree_p, tree_p->root_node, &((ts){3, "three"}), 2);
	print_tree(tree_p, print_ts);

	set_child(tree_p, tree_p->root_node->children[1], &((ts){4, "four"}), 0);
	print_tree(tree_p, print_ts);

	set_child(tree_p, tree_p->root_node->children[2], &((ts){5, "five"}), 2);
	print_tree(tree_p, print_ts);

	remove_child(tree_p, tree_p->root_node, 1);
	print_tree(tree_p, print_ts);

	set_child(tree_p, tree_p->root_node, &((ts){6, "six"}), 1);
	print_tree(tree_p, print_ts);

	set_child(tree_p, tree_p->root_node->children[1], &((ts){7, "seven"}), 2);
	print_tree(tree_p, print_ts);

	set_child(tree_p, tree_p->root_node, &((ts){8, "eight"}), 1);
	print_tree(tree_p, print_ts);

	delete_tree(tree_p);
	return 0;
}