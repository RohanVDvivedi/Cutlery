#ifndef GRAPH_H
#define GRAPH_H

#include<stdio.h>
#include<stdlib.h>
#include<mmarray.h>
#include<array.h>

// the graph will manage the data of the nodes and edges
// but not your data, the data hanging by the nodes and edges

typedef struct node node;
struct node
{
	// every node will hold some data
	const void* node_data;

	// list of edges that make us in or out of the node
	array* edges;
};

typedef struct edge edge;
struct edge
{
	// every edge will hold some data
	const void* edge_data;

	// every edge either bi-directional or uni-directional
	// will have 2 nodes at each of its ends
	const node* nodes[2];
};

typedef struct graph graph;
struct graph
{
	// number of nodes in the graph
	unsigned long long int node_count;

	// list of nodes in this graph
	mmarray* node_list;

	// number of edges in the graph
	unsigned long long int edge_count;

	// list of edges in this graph
	mmarray* edge_list;

	// value of expected edges per node
	unsigned long long int expected_median_edge_count_per_node;
};

// builds a new graph
graph* get_graph(unsigned long long int expected_nodes, unsigned long long int expected_edges);

// adds a node to the graph which will hold the node_data_p 
unsigned long long int add_node(graph* graph_p, const void* node_data_p);

// joins 2 nodes creating an edge between them, which will hold data p
void join_nodes(graph* graph_p, unsigned long long int node0, unsigned long long int node1, const void* edge_data_p);

// deletes all the nodes and edges and the graph itself
void delete_graph(graph* graph_p);

// prints the graph
void print_graph(const graph* graph_p, void (*print_node_data_element)(const void* data_p), void (*print_edge_data_element)(const void* data_p));

#endif