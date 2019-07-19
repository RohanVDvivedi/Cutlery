#ifndef GRAPH_H
#define GRAPH_H

#include<stdio.h>
#include<stdlib.h>
#include<array.h>

typedef struct node node;
struct node
{
	// every node will hold some data
	void* nodeData;

	// number of edges this node uses
	unsigned long long int edge_count;

	// a node connects to other nodes by edges
	edge** edges;
};

typedef struct edge edge;
struct edge
{
	// every edge will hold some data
	void* edgeData;

	// every edge either bi-directional or uni-directional
	// will have 2 nodes at each of its ends
	node nodes[2];
};

typedef struct graph graph;
struct graph
{
	unsigned long long int size_of_data_on_edge;

	unsigned long long int edge_count;

	node** node_p_p;

	unsigned long long int size_of_data_on_node;

	unsigned long long int node_count;

	edge** edge_p_p;
};

#endif