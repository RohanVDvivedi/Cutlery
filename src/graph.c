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
	node new_node = {.node_data = node_data_p, .edge_count_of_node = 0, .edges = get_array(graph_p->expected_median_edge_count_per_node)};
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
	const node* node0_p = get_mmarray_element(graph_p->node_list, node0);
	const node* node1_p = get_mmarray_element(graph_p->node_list, node1);
	edge new_edge = {.edge_data = edge_data_p, .nodes = {node0_p, node1_p}};
	if(graph_p->edge_list->total_size <= graph_p->edge_count)
	{
		expand_mmarray(graph_p->edge_list);
	}
	unsigned long long int index_of_new_edge = graph_p->edge_count;
	set_mmarray_element(graph_p->edge_list, &new_edge, graph_p->edge_count++);
	const edge* new_edge_p = get_mmarray_element(graph_p->edge_list, index_of_new_edge);
	set_element(node0_p->edges, new_edge_p, ((node*)node0_p)->edge_count_of_node++);
	set_element(node1_p->edges, new_edge_p, ((node*)node1_p)->edge_count_of_node++);
}

void delete_graph(graph* graph_p)
{
	for(unsigned long long int i = 0; i < graph_p->node_count; i++)
	{
		const node* deletion_node = get_mmarray_element(graph_p->node_list, i);
		delete_array(deletion_node->edges);
	}
	delete_mmarray(graph_p->node_list);
	delete_mmarray(graph_p->edge_list);
	free(graph_p);
}

void print_node_connections(const node* node_p)
{
	printf("\tthe edges for the node %d =>", ((int)node_p));
	for(unsigned long long int i = 0; i < node_p->edge_count_of_node; i++)
	{
		printf(" %llu->[%d]", i, ((int)get_element(node_p->edges, i)));
	}
	printf("\n");
}

void print_edge_connections(const edge* edge_p)
{
	printf("\tthe nodes connected by the edge %d => 0->[%d] 1->[%d]\n", ((int)edge_p), ((int)edge_p->nodes[0]), ((int)edge_p->nodes[1]));
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
