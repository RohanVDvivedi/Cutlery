#include<graph.h>

typedef struct testnodestruct tsn;
struct testnodestruct
{
	int index;
	char* s;
};

typedef struct testedgestruct tse;
struct testedgestruct
{
	int index;
	int a;
	int b;
	char* s;
};

void print_tsn(const void* tsv)
{
	printf(" %d, %s", ((tsn*)tsv)->index, ((tsn*)tsv)->s);
}

void print_tse(const void* tsv)
{
	printf(" %d, %d, %d, %s", ((tse*)tsv)->index, ((tse*)tsv)->a, ((tse*)tsv)->b, ((tse*)tsv)->s);
}

int main()
{
	graph* graph_p = get_graph(6, 10);
	add_node(graph_p, &((tsn){0, "zero"}));
	add_node(graph_p, &((tsn){1, "one"}));
	add_node(graph_p, &((tsn){2, "two"}));
	add_node(graph_p, &((tsn){3, "three"}));
	add_node(graph_p, &((tsn){4, "four"}));
	add_node(graph_p, &((tsn){5, "five"}));
	join_nodes(graph_p, 0, 1, &((tse){0, 0, 1, "zero"}));
	join_nodes(graph_p, 0, 2, &((tse){1, 0, 2, "one"}));
	join_nodes(graph_p, 1, 3, &((tse){2, 1, 3, "two"}));
	join_nodes(graph_p, 2, 3, &((tse){3, 2, 3, "three"}));
	//print_graph(graph_p, print_tsn, print_tse);
	join_nodes(graph_p, 1, 4, &((tse){4, 1, 4, "four"}));
	join_nodes(graph_p, 2, 4, &((tse){5, 2, 4, "five"}));
	join_nodes(graph_p, 3, 4, &((tse){6, 3, 4, "six"}));
	join_nodes(graph_p, 0, 5, &((tse){7, 0, 5, "seven"}));
	//print_graph(graph_p, print_tsn, print_tse);
	delete_graph(graph_p);
	return 0;
}