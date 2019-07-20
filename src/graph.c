#include<graph.h>

graph* get_graph(unsigned long long int expected_nodes, unsigned long long int expected_edges)
{
	graph* graph_p = (graph*) calloc(1, sizeof(graph));
	graph_p->node_count = 0;
	graph_p->node_list = get_mmarray(expected_nodes + 1, sizeof(node));
	graph_p->edge_count = 0;
	graph_p->edge_list = get_mmarray(expected_edges + 1, sizeof(edge));
	graph_p->expected_median_edge_count_per_node = (2 * ((expected_edges + 1) / (expected_nodes + 1))) + 1;
	return graph_p;
}

unsigned long long int add_node(graph* graph_p, const void* node_data_p)
{
	node new_node = {.node_data = node_data_p, .edges = get_array(graph_p->expected_median_edge_count_per_node)};
	if(graph_p->node_list->total_size <= graph_p->node_count)
	{
		expand_mmarray(graph_p->node_list);
	}
	unsigned long long int index_of_new_node = graph_p->node_count;
	set_mmarray_element(graph_p->node_list, &new_node, graph_p->node_count++);
	return index_of_new_node;
}

void join_nodes(graph* graph_p, unsigned long long int node0, unsigned long long int node1, const void* edge_data_p)
{
	edge new_edge = {.edge_data = edge_data_p, .nodes = {get_mmarray_element(graph_p->edge_list, node0), get_mmarray_element(graph_p->edge_list, node1)}};
	if(graph_p->edge_list->total_size <= graph_p->edge_count)
	{
		expand_mmarray(graph_p->edge_list);
	}
	set_mmarray_element(graph_p->edge_list, &new_edge, graph_p->edge_count++);
}

void delete_graph(graph* graph_p)
{
	for(unsigned long long int i = 0; i < graph_p->node_list->total_size; i++)
	{
		const node* deletion_node = get_mmarray_element(graph_p->node_list, i);
		delete_array(deletion_node->edges);
	}
	delete_mmarray(graph_p->node_list);
	delete_mmarray(graph_p->edge_list);
	free(graph_p);
}

void print_graph(const graph* graph_p, void (*print_node_data_element)(const void* data_p), void (*print_edge_data_element)(const void* data_p))
{
	printf("graph : \n");
	printf("\tnode_count : %llu\n", graph_p->node_count);
	printf("\tedge_count : %llu\n", graph_p->edge_count);
	printf("\tnode_list : \n");
	print_mmarray(graph_p->node_list, print_node_data_element);
	printf("\tedge_list : \n");
	print_mmarray(graph_p->edge_list, print_edge_data_element);
}
