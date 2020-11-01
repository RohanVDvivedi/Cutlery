#ifndef MEMORY_ALLOCATOR_INTERFACE_H
#define MEMORY_ALLOCATOR_INTERFACE_H

typedef struct memory_allocator memory_allocator;
struct memory_allocator
{
	// usage similar to malloc
	void* allocate(unsigned int size);

	// usage similar to calloc
	void* allocate_zero(unsigned int number_of_elements, unsigned int size_of_each_element);

	// usage similar to realloc
	void* reallocate(void* old_allocation, unsigned int size);

	// usage similar to free
	void deallocate(void* old_allocation);
};

#endif