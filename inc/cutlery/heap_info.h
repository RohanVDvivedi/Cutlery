#ifndef HEAP_INFO_H
#define HEAP_INFO_H

#include<cutlery/comparator_interface.h>

typedef enum heap_type heap_type;
enum heap_type
{
	MIN_HEAP = 1,
	MAX_HEAP = -1
	// due to given MIN_HEAP and MAX_HEAP values (1 and -1)
	// we can just multiply heap_type value with the comparison result
	// and then check for MIN_HEAP property
};

typedef struct heap_info heap_info;
struct heap_info
{
	// type of heap, MIN_HEAP or MAX_HEAP
	heap_type type;

	// comparator for the elements
	// it returns 0 if they are same, >0 if data1 is greater than data2 else it must return <0 value
	comparator_interface comparator;
};

static inline int is_reordering_required(const void* parent, const void* child, const heap_info* heap_info_p)
{
	return (heap_info_p->type * compare_with_comparator(&(heap_info_p->comparator), parent, child)) > 0;
}

#endif