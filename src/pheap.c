#include<cutlery/pheap.h>

void initialize_pheap(pheap* pheap_p, heap_type type, pheaptype policy, const comparator_interface* comparator, cy_uint node_offset)
{
	pheap_p->info = (heap_info){.type = type, .comparator = (*comparator)};;
	pheap_p->type = policy;
	pheap_p->node_offset = node_offset;
	pheap_p->root = NULL;
}

void initialize_phpnode(phpnode* node_p);

int is_free_floating_phpnode(const phpnode* node_p);

int is_empty_pheap(const pheap* pheap_p);

int push_to_pheap(pheap* pheap_p, const void* data);

int pop_from_pheap(pheap* pheap_p);

const void* get_top_of_pheap(const pheap* pheap_p);

void heapify_for_in_pheap(pheap* pheap_p, const void* data);

int remove_from_pheap(pheap* pheap_p, const void* data);

void remove_all_from_pheap(pheap* pheap_p, const notifier_interface* ni_p);

void for_each_in_pheap(const pheap* pheap_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params);

void sprint_pheap(dstring* append_str, const pheap* pheap_p, void (*sprint_element)(dstring* append_str, const void* data, unsigned int tabs), unsigned int tabs);
