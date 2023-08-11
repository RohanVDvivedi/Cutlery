#include<heap_info.h>

int is_reordering_required(const void* parent, const void* child, int (*compare)(const void* data1, const void* data2), heap_type type)
{
	return (type * compare(parent, child)) > 0;
}