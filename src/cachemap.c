#include<cutlery/cachemap.h>

#include<cutlery/cutlery_node.h>

int initialize_cachemap(cachemap* cachemap_p, const void* pinning_context, int (*is_pinned)(const void* pinning_context, const void* data), cy_uint bucket_count, const hasher_interface* hasher, const comparator_interface* comparator, cy_uint node_offset)
{
	cachemap_p->pinning_context = pinning_context;
	cachemap_p->is_pinned = is_pinned;
	cachemap_p->node_offset = node_offset;
	if(!initialize_hashmap(&(cachemap_p->map), ELEMENTS_AS_RED_BLACK_BST, bucket_count, hasher, comparator, node_offset + offsetof(cchnode, map_embed_node)))
		return 0;
	initialize_linkedlist(&(cachemap_p->lru), node_offset + offsetof(cchnode, lru_embed_node));
	return 1;
}

int initialize_cachemap_with_allocator(cachemap* cachemap_p, const void* pinning_context, int (*is_pinned)(const void* pinning_context, const void* data), cy_uint bucket_count, const hasher_interface* hasher, const comparator_interface* comparator, cy_uint node_offset, memory_allocator mem_allocator)
{
	cachemap_p->pinning_context = pinning_context;
	cachemap_p->is_pinned = is_pinned;
	cachemap_p->node_offset = node_offset;
	if(!initialize_hashmap_with_allocator(&(cachemap_p->map), ELEMENTS_AS_RED_BLACK_BST, bucket_count, hasher, comparator, node_offset + offsetof(cchnode, map_embed_node), mem_allocator))
		return 0;
	initialize_linkedlist(&(cachemap_p->lru), node_offset + offsetof(cchnode, lru_embed_node));
	return 1;
}

int initialize_cachemap_with_memory(cachemap* cachemap_p, const void* pinning_context, int (*is_pinned)(const void* pinning_context, const void* data), cy_uint bucket_count, const hasher_interface* hasher, const comparator_interface* comparator, cy_uint node_offset, const void* bucket_memory[])
{
	cachemap_p->pinning_context = pinning_context;
	cachemap_p->is_pinned = is_pinned;
	cachemap_p->node_offset = node_offset;
	if(!initialize_hashmap_with_memory(&(cachemap_p->map), ELEMENTS_AS_RED_BLACK_BST, bucket_count, hasher, comparator, node_offset + offsetof(cchnode, map_embed_node), bucket_memory))
		return 0;
	initialize_linkedlist(&(cachemap_p->lru), node_offset + offsetof(cchnode, lru_embed_node));
	return 1;
}

void initialize_cchnode(cchnode* node_p)
{
	initialize_bstnode(&(node_p->map_embed_node));
	initialize_llnode(&(node_p->lru_embed_node));
}

int is_free_floating_cchnode(const cchnode* node_p)
{
	return is_free_floating_bstnode(&(node_p->map_embed_node)) && is_free_floating_llnode(&(node_p->lru_embed_node));
}

int insert_in_cachemap(cachemap* cachemap_p, const void* data);

const void* find_equals_in_cachemap(const cachemap* cachemap_p, const void* data)
{
	return find_equals_in_hashmap(&(cachemap_p->map), data);
}

int remove_from_cachemap(cachemap* cachemap_p, const void* data);

const void* get_evictable_element_from_cachemap(const cachemap* cachemap_p)
{
	// always force evicting from head of lru
	return get_head_of_linkedlist(&(cachemap_p->lru));
}

int bump_element_in_cachemap(cachemap* cachemap_p, const void* data)
{
	{
		// if it is a free floating cchnode, i.e. it is not in cachemap, then we can not bump it
		cchnode* node_p = get_node(data, cachemap_p);
		if(is_free_floating_cchnode(node_p))
			return 0;
	}

	// remove it from the lru
	remove_from_linkedlist(&(cachemap_p->lru), data);

	// insert it back at the tail, forcing unused elements towards the head, only if it is not pinned
	if(!cachemap_p->is_pinned(cachemap_p->pinning_context, data))
		insert_tail_in_linkedlist(&(cachemap_p->lru), data);

	return 1;
}

cy_uint get_bucket_count_cachemap(const cachemap* cachemap_p)
{
	return get_bucket_count_hashmap(&(cachemap_p->map));
}

cy_uint get_element_count_cachemap(const cachemap* cachemap_p)
{
	return get_element_count_hashmap(&(cachemap_p->map));
}

int is_empty_cachemap(const cachemap* cachemap_p)
{
	return is_empty_hashmap(&(cachemap_p->map));
}

int resize_cachemap(cachemap* cachemap_p, cy_uint new_bucket_count)
{
	return resize_hashmap(&(cachemap_p->map), new_bucket_count);
}

void remove_all_from_cachemap(cachemap* cachemap_p, const notifier_interface* ni_p)
{
	// remove all first from lru
	remove_all_from_linkedlist(&(cachemap_p->lru), NULL);

	// then remove all from the map, pass here ni_p so that any one waiting could be notified of the removal
	remove_all_from_hashmap(&(cachemap_p->map), ni_p);
}

void deinitialize_cachemap(cachemap* cachemap_p)
{
	deinitialize_hashmap(&(cachemap_p->map));
}

void sprint_cachemap(dstring* append_str, const cachemap* cachemap_p, void (*sprint_element)(dstring* append_str, const void* data, unsigned int tabs), unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs++);
	snprintf_dstring(append_str, "cachemap ");
	sprint_hashmap(append_str, &(cachemap_p->map), sprint_element, tabs+1);
	sprint_linkedlist(append_str, &(cachemap_p->lru), sprint_element, tabs+1);
}

void for_each_in_cachemap(const cachemap* cachemap_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params)
{
	for_each_in_hashmap(&(cachemap_p->map), operation, additional_params);
}