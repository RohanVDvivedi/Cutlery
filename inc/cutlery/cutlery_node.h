#ifndef CUTLERY_NODE_H
#define CUTLERY_NODE_H

/*
*	MACROS TO CONVERT FROM DATA TO NODE AND NODE TO DATA
*  This is only used by embedded node based data structures (intrusive data structures),
*  that includes, all embedded node based data structures (like linkedlist, singlylist, bst, hashmap, heap and union_find)
*  which store node_offset to their nodes.
*  Since, their nodes (that these datastructures operate on) are always at this fixed offset inside the user's data (structs),
*  simple pointer arithmetic as described in below macros to go from (user's) data pointer to node pointers, and viceversa, suffices.
*/
#define get_data(node_p, ds_p) 	(((const void*)(node_p)) - ((ds_p)->node_offset))
#define get_node(data_p, ds_p) 	(((void*)(data_p)) + ((ds_p)->node_offset))

#endif