#ifndef HEAP_INFO_H
#define HEAP_INFO_H

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

	// comparator function for the elements
	int (*compare)(const void* data1, const void* data2);
};

int is_reordering_required(const void* parent, const void* child, const heap_info* heap_info_p);

#endif