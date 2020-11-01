#ifndef MEMORY_ALLOCATOR_INTERFACE_H
#define MEMORY_ALLOCATOR_INTERFACE_H

// A memory allocator is a struct thet contains methods for allocation/deallocation of heap memory
// it defines a memory allocator methods to be used by a container data structure
// you may provide a custom memory allocator for any specific datastructure container for cutlery
// the memory allocator functions definitions are kept so as to be equivalent to heap memory allocator of stdlib.h 

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