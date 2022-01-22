#include<singlylist.h>

#include<cutlery_stds.h>

// get data from slnode
#define get_data(node_p) 	(((const void*)(node_p)) - sl->node_offset)
// get slnode from data
#define get_node(data_p) 	(((void*)(data_p)) + sl->node_offset)

void initialize_singlylist(singlylist* sl, unsigned int node_offset)
{
	sl->node_offset = node_offset;
	sl->head = NULL;
	sl->tail = NULL;
}

void initialize_slnode(slnode* node_p)
{
	node_p->next = NULL;
}

static int is_new_slnode(const singlylist* sl, const slnode* node_p)
{
	return node_p->next == NULL;
}

int is_empty_singlylist(const singlylist* sl)
{
	return sl->head == NULL;
}

const void* get_head_of_singlylist(const singlylist* sl);
const void* get_tail_of_singlylist(const singlylist* sl);
const void* get_nth_from_head_of_singlylist(const singlylist* sl, unsigned int n);

const void* get_next_of_in_singlylist(const singlylist* sl, const void* data_xist);

int insert_head_in_singlylist(singlylist* sl, const void* data);
int insert_tail_in_singlylist(singlylist* sl, const void* data);
int insert_after_in_singlylist(singlylist* sl, const void* data_xist, const void* data);

int insert_all_at_head_in_singlylist(singlylist* sl, singlylist* insert_from_ll);
int insert_all_at_tail_in_singlylist(singlylist* sl, singlylist* insert_from_ll);
int insert_all_after_in_singlylist(singlylist* sl, const void* data_xist, singlylist* insert_from_ll);

int remove_head_from_singlylist(singlylist* sl);
int remove_next_of_from_singlylist(singlylist* sl, const void* data_xist);

void remove_all_from_singlylist(singlylist* sl);

const void* find_equals_in_singlylist(const singlylist* sl, const void* data, int (*compare)(const void* data1, const void* data2));

void for_each_in_singlylist(const singlylist* sl, void (*operation)(const void* data_p, const void* additional_params), const void* additional_params);

void sprint_singlylist(dstring* append_str, const singlylist* sl, void (*sprint_element)(dstring* append_str, const void* data_p, unsigned int tabs), unsigned int tabs);
