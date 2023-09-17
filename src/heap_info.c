#include<heap_info.h>

int is_reordering_required(const void* parent, const void* child, const heap_info* heap_info_p)
{
	return (heap_info_p->type * compare_with_comparator(&(heap_info_p->comparator), parent, child)) > 0;
}