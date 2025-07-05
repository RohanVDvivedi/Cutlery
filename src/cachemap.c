#include<cutlery/cachemap.h>

int initialize_cachemap(cachemap* cachemap_p, const void* pinning_context, int (*is_pinned)(const void* pinning_context, const void* data), cy_uint bucket_count, const hasher_interface* hasher, const comparator_interface* comparator, cy_uint node_offset);

int initialize_cachemap_with_allocator(cachemap* cachemap_p, const void* pinning_context, int (*is_pinned)(const void* pinning_context, const void* data), cy_uint bucket_count, const hasher_interface* hasher, const comparator_interface* comparator, cy_uint node_offset, memory_allocator mem_allocator);

int initialize_cachemap_with_memory(cachemap* cachemap_p, const void* pinning_context, int (*is_pinned)(const void* pinning_context, const void* data), cy_uint bucket_count, const hasher_interface* hasher, const comparator_interface* comparator, cy_uint node_offset, const void* bucket_memory[]);

void initialize_cchnode(cchnode* node_p);

int is_free_floating_cchnode(const cchnode* node_p);

int insert_in_cachemap(cachemap* cachemap_p, const void* data);

const void* find_equals_in_cachemap(const cachemap* cachemap_p, const void* data);

int remove_from_cachemap(cachemap* cachemap_p, const void* data);

const void* get_evictable_element_from_cachemap(const cachemap* cachemap_p);

int bump_element_in_cachemap(cachemap* cachemap_p, const void* data);

cy_uint get_bucket_count_cachemap(const cachemap* cachemap_p);

cy_uint get_element_count_cachemap(const cachemap* cachemap_p);

int is_empty_cachemap(const cachemap* cachemap_p);

int resize_cachemap(cachemap* cachemap_p, cy_uint new_bucket_count);

void remove_all_from_cachemap(cachemap* cachemap_p, const notifier_interface* ni_p);

void deinitialize_cachemap(cachemap* cachemap_p);

void sprint_cachemap(dstring* append_str, const cachemap* cachemap_p, void (*sprint_element)(dstring* append_str, const void* data, unsigned int tabs), unsigned int tabs);

void for_each_in_cachemap(const cachemap* cachemap_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params);