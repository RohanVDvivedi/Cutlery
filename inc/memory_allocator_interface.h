#ifndef MEMORY_ALLOCATOR_INTERFACE_H
#define MEMORY_ALLOCATOR_INTERFACE_H

// A memory allocator is a struct thet contains methods for allocation/deallocation of heap memory
// it defines a memory allocator methods to be used by a container data structure of this library (Cutlery)
// you may provide a custom memory allocator for any specific datastructure container for cutlery

typedef enum memory_allocator_initialization memory_allocator_initialization;
enum memory_allocator_initialization
{
	UN_AFFECTED,		// the cutlery algorithm/datastructure does not care about initialization of the newly allocated memory, that you will return 
	ZERO_INITIALIZE,	// the new allocated memory, must conatain all zero bytes
	PRESERVE_OLD_MEMORY	// if possible preserve maximum number of bytes that you could preserve from old_memory
};

typedef struct memory_allocator memory_allocator;
struct memory_allocator
{
	// usage of this function is sohisticated and complex
	// it has been designed to encapsualte the functionality of all of
	// 		* malloc, calloc, realloc and aligned_alloc (or posix_memalign)
	/*
	** parameters
	**
	** old_memory     : memory allocated in the past, that you no longer require, this memory must/will be freed
	** old_size       : size of memory pointed to by old_allocation (your custom allocator may or may not use this value, it is up to you)
	** new_size       : size of new memory that you want
	** new_alignment  : the alignment required for the new memory (0 or 1 represents no memory alignment requirements)
	** initialization : check enum memory_allocator_initialization
	**
	**
	** return value
	**		if there is enough amount of memory, then for a non zero value of parameter "new_size",
	**		it must return a pointer that is aligned to "new_alignment", and can safely hold atleast "new_size" number of bytes
	**		and these "new_size" number of bytes must be initialized according to "initialization" enum memory_allocator_initialization parameter
	**
	** 		The returned memory pointer must point to atleast new_size number of bytes
	**		The returned memory pointer may or may not point to the same old_memory
	**			and Cutlery algorithms and datastructures will not make any such assumptions about implementation details of the memory allocator provided to them
	**
	**		this function must return NULL on any error that you encounter,
	**		inclusive but not limited to 
	**			* memory allocation failure due to excessive memory utilization
	*/
	void* (*allocate)(void* old_memory, unsigned int old_size, unsigned int new_size, unsigned int new_alignment, memory_allocator_initialization initialization);

	// usage similar to free
	/*
	** 	this is equivalent to calling
	** 	**	allocate(old_memory, old_size, 0, 0, 0) **
	**	and returning NULL
	*/
	void (*deallocate)(void* old_memory, unsigned int old_size);
};

#endif