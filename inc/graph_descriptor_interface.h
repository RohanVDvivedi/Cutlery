#ifndef GRAPH_DESCRIPTOR_INTERFACE_H
#define GRAPH_DESCRIPTOR_INTERFACE_H

// the below struct is a collection of method 
// that defines/describes implementation of a graph data structure
// and access methods to access it by the graph algorithms of cutlery

/*
	context is a parameter to every function defined on either of nodes, edges and even graphs

	here the context defines and the context should help you to know the following
	  * the query that we are solving (a description of the problem that you are solving)
	  * the graph that we are solving it on (ideally a pointer to the graph itself)
	  * any current query specific variables
	      * this is where you should ideally maintain 
	        * what nodes have been visited
	        * what edges have been traversed, etc
	  * the context should not be used to store parts of the graph, nodes or edges
	  * the context should instead represent the query and the state of the query processing algorithm to help you take decissions accordingly and efficiently

	the above points should be used only as a guide line to define what a context datastructure should contain
	you may aswell feel free to do what ever you like

	BUT here are my 2 cents for your design

	variables in the context should help you evaluate the all the following descriptor functions quickly
	storing the query description will help you filter what edges are not needed to be traversed for the problem at hand
	effectively sppeding up the graph traversal

	the context has to be discardable after the query or search algorithm or path finding algorithm
	so that you dont need to ""unvisit"" the visisted nodes and ""untraverse"" the traversed edges

	keeping the context specific variables will also help you segregate actual data vs temporary data of the graph (like visited node and edge specific data)
	having the traversal context separate from the graph will help you to parallelize algorithms
	by supplying separate new context to run read-only graph traversals all in different threads concurrently
*/

typedef struct node_descriptor node_descriptor;
struct node_descriptor
{
	// get the number of edges entering and leaving this node
	// get number of all the edges for which this node is either a start or end point
	unsigned int (*get_edge_count)(void* node, void* context);

	// get an edge that is touching this node i.e. this node is either source or destination for a given edge
	// here the edge_index is always lesser than the result of get_edge_count
	// it varies from 0 to get_edge_count() - 1
	void* (*get_edge)(void* node, unsigned int edge_index, void* context);

	// this is the cost incurred every time, we visit this node
	int (*get_visiting_cost)(void* edge, void* context);

	// to mark a given node as visited and
	// to check if a given node has been visited ( 1 if visited, 0 if not visited )
	void (*mark_visited)(void* node, void* context);
	int (*is_visited)(void* node, void* context);
};

// possible return values of get_edge_type function for any edge
#define DIRECTED   1
#define UNDIRECTED 2

// possible values of node_index as a parameter of get_node function for a DIRECTED edge
#define FROM_NODE 0
#define TO_NODE   1

typedef struct edge_descriptor edge_descriptor;
struct edge_descriptor
{
	// an edge type can be DIRECTED or UNDIRECTED
	unsigned int (*get_edge_type)(void* edge, void* context);

	// for any of DIRECTED or UNDIRECTED edge, the node_index can be 0 or 1 only. (node_index < 2 always)
	// in a DIRECTED edge, node_index may be equal to FROM_NODE or TO_NODE
	//    node_index == FROM_NODE  => return source node (edge comming out)
	//    node_index == TO_NODE    => return destination node (edge going in)
	void* (*get_node)(void* edge, unsigned int node_index, void* context);

	// this is the cost incurred every time, we traverse this edge
	int (*get_traversal_cost)(void* edge, void* context);

	// to mark a given edge as traversed and
	// to check if a given edge has been traversed ( 1 if traversed, 0 if not traversed )
	void (*mark_traversed)(void* edge, void* context);
	int (*is_traversed)(void* edge, void* context);
};

typedef struct graph_descriptor graph_descriptor;
struct graph_descriptor
{
	// returns root node of this graph, all algorithms start from here, unless a separate root has been provided
	void* (*get_root)(void* graph, void* context);

	// get total number of nodes and edges in the given graph
	unsigned int (*get_node_count)(void* graph, void* context);
	unsigned int (*get_edge_count)(void* graph, void* context);

	node_descriptor node_descriptor_funcs;

	edge_descriptor edge_descriptor_funcs;
};

#endif