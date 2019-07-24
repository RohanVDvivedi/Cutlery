#ifndef GRAPH_H
#define GRAPH_H

#include<stdio.h>
#include<stdlib.h>
#include<mmarray.h>
#include<array.h>

// the graph will manage the data of the nodes and edges
// but not your data, the data that hangs by the nodes and edges (namely node_data_p and edge_data_p)

typedef struct node node;
struct node
{
	// every node will hold some data
	const void* node_data;

	// the edges that we are holding in the edges array for this node
	unsigned long long int edge_count_of_node;

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
	// we use mmarray here, since we dont want to do memory management for our nodes
	// even here mmarray only stores a shallow clone of the node,
	// hence we effectively still can reference node_data_p but not own it
	mmarray* node_list;

	// number of edges in the graph
	unsigned long long int edge_count;

	// list of edges in this graph
	// we use mmarray here, since we dont want to do memory management for our nodes
	// even here mmarray only stores a shallow clone of the edge,
	// hence we effectively still can reference edge_data_p but not own it
	mmarray* edge_list;

	// value of expected edges per node
	unsigned long long int expected_median_edge_count_per_node;
};

// builds a new graph, which has initial capacity of expected_nodes and expected_edges
graph* get_graph(unsigned long long int expected_nodes, unsigned long long int expected_edges);

// adds a node to the graph which will hold the node_data_p 
unsigned long long int add_node(graph* graph_p, const void* node_data_p);

// joins 2 nodes creating an edge between them, which will hold edge_data_p
void join_nodes(graph* graph_p, unsigned long long int node0, unsigned long long int node1, const void* edge_data_p);

// deletes all the nodes and edges and the graph itself
void delete_graph(graph* graph_p);

// prints the graph
void print_graph(const graph* graph_p, void (*print_node_data_element)(const void* data_p), void (*print_edge_data_element)(const void* data_p));

// prints pointers to all the edges of the node
void print_node_connections(const node* node_p);

// prints pointers to both the nodes that the edge connects
void print_edge_connections(const edge* edge_p);

#endif