#ifndef MEMORY_ALLOCATOR_INTERFACE_H
#define MEMORY_ALLOCATOR_INTERFACE_H

// A memory allocator is a struct thet contains methods for allocation/deallocation of heap memory
// it defines a memory allocator methods to be used by a container data structure of this library (Cutlery)
// you may provide a custom memory allocator for any specific datastructure container for cutlery

typedef struct memory_allocator memory_allocator;
struct memory_allocator
{
	// usage of this function is sohisticated and complex
	// it has been designed to encapsualte the functionality of all of
	// 		* malloc, calloc, realloc and aligned_alloc (or posix_memalign)
	/*
	** parameters
	**
	** old_memory                 : memory allocated in the past, that you no longer require, this memory must/will be freed
	** old_size                   : size of memory pointed to by old_allocation (your custom may or may not use this value, your call)
	** new_size                   : size of new memory that you want
	** new_alignment              : the alignment reuired for the new memory, this is of size new_size
	** zero_initialize_new_memory : if this parameter is non zero, then the new memory must be nonzero
	**
	**
	** returns
	** 		if the new_size != 0 then,
	**			this a pointer to a new memory address,
	** 			of size equal to new_size,
	**			with its first address aligned to new_alignment,
	**			all the returned memory must be 0, if (zero_initialize_new_memory != 0)
	**		else
	**			this function returns NULL ( ((void*)0) as defined in cutlery_stds.h )
	**
	**	this function must return NULL on any error that you encounter,
	**	inclusive but not limited to
	**		* memory allocation failure due to excessive memory utilization
	*/
	void* (*allocate)(void* old_memory, unsigned int old_size, unsigned int new_size, unsigned int new_alignment, int zero_initialize_new_memory);

	// usage similar to free
	/*
	** 	this is equivalent to calling
	** 	**	allocate(old_memory, old_size, 0, 0, 0) **
	**	and returning NULL
	*/
	void (*deallocate)(void* old_memory, unsigned int old_size);
};

#endif