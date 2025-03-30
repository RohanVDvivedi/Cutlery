#ifndef DSTRING_INDEX_ACCESSED_INTERFACE_H
#define DSTRING_INDEX_ACCESSED_INTERFACE_H

#include<cutlery/index_accessed_interface.h>
#include<cutlery/dstring.h>

// index access interface for dstring

// This allows you to use a lot of functionality of index_accesses_search_sort.h

index_accessed_interface get_index_accessed_interface_for_dstring(dstring* str_p);

// comparator helper functions to sort/search characters in dstring

int compare_chars_ascending_at(const void* c1, const void* c2);
int compare_chars_descending_at(const void* c1, const void* c2);

#endif