#ifndef HEAP_TYPE_H
#define HEAP_TYPE_H

typedef enum heap_type heap_type;
enum heap_type
{
	MIN_HEAP = 1,
	MAX_HEAP = -1
	// due to given MIN_HEAP and MAX_HEAP values (1 and -1)
	// we can just multiply heap_type value with the comparison result
	// and then check for MIN_HEAP property
};

int is_reordering_required(const void* parent, const void* child, int (*compare)(const void* data1, const void* data2), heap_type type);

#endif