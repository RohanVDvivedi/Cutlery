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
	// construct the required node on the stack memory
	node new_node = {.node_data = node_data_p, .edge_count_of_node = 0, .edges = get_array(graph_p->expected_median_edge_count_per_node)};
	
	// we are required to extend the mmarray to save this given node,
	// if we are exceeding the initial base size 
	if(graph_p->node_list->total_size <= graph_p->node_count)
	{
		expand_mmarray(graph_p->node_list);
	}

	// the mmarray is filled one after another as in a stack
	// the new node is placed at the last
	unsigned long long int index_of_new_node = graph_p->node_count;

	// now create the node and save it in node_list mmarray of the graph
	// note : since mmarray manages the data on its own, we are effectively
	// offloading memory management of nodes to node_list mmarray
	set_mmarray_element(graph_p->node_list, &new_node, graph_p->node_count++);

	// we return the index where we add the current node,
	// for external logic for reference
	return index_of_new_node;
}

// this is a separate method, which adds reference of the edge to edges array of the node
// this is to be used if your given node connects to any other edge using the corresponding edge
void add_reference_of_edge_to_node(node* node_p, const edge* edge_p)
{
	// expand edges array if required
	if(node_p->edges->total_size <= node_p->edge_count_of_node)
	{
		expand_array(node_p->edges);
	}
	// put the new references in
	set_element(node_p->edges, edge_p, ((node*)node_p)->edge_count_of_node++);
}

void join_nodes(graph* graph_p, unsigned long long int node0, unsigned long long int node1, const void* edge_data_p)
{
	// get the pointer to node0
	const node* node0_p = get_mmarray_element(graph_p->node_list, node0);

	// get the pointer to node1
	const node* node1_p = get_mmarray_element(graph_p->node_list, node1);

	// construct the new edge on the stack
	// which has pointer to node0 and node1 nodes (the nodes that it connects)
	edge new_edge = {.edge_data = edge_data_p, .nodes = {node0_p, node1_p}};

	// expand the edge_list if required, i.e. this is required to make sure,
	// if we have enough pointers to hold the new edge
	if(graph_p->edge_list->total_size <= graph_p->edge_count)
	{
		expand_mmarray(graph_p->edge_list);
	}

	// this is the index of the new edge in the mmarray edge_list,
	// the new edge is place at the last, after the previously last edge
	unsigned long long int index_of_new_edge = graph_p->edge_count;

	// now create the edge and save it in edge_list mmarray of the graph
	// note : since mmarray manages the data on its own, we are effectively
	// offloading memory management of edges to edge_list mmarray
	set_mmarray_element(graph_p->edge_list, &new_edge, graph_p->edge_count++);

	// get reference pointer to the newly created edge
	const edge* new_edge_p = get_mmarray_element(graph_p->edge_list, index_of_new_edge);

	// add pointer of this edge in the edges array of the node0,
	// so that the node0 can use this to determine which edge it may use to reach node1
	add_reference_of_edge_to_node(((node*)node0_p), new_edge_p);

	// add pointer of this edge in the edges array of the node1,
	// so that the node1 can use this to determine which edge it may use to reach node0
	add_reference_of_edge_to_node(((node*)node1_p), new_edge_p);
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
