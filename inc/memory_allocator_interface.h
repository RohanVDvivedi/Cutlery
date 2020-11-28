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

// usage of this typedef-ed function is sohisticated and complex
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
**		it must return a pointer that is multiple of "new_alignment", and can safely hold atleast "new_size" number of bytes
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
typedef void* (*memory_allocator)(void* old_memory, unsigned int old_size, unsigned int new_size, unsigned int new_alignment, memory_allocator_initialization initialization);

/*
**	Notes for usage :
**	
**	this notes define how a memory_allocator interface function must behave or how it should be used
**
**	malloc like usage
**	void* new_memory = memory_allocator(NULL, 0, new_size, 0, UN_AFFECTED);
**
**	calloc like usage
**	void* new_memory = memory_allocator(NULL, 0, new_size, 0, ZERO_INITIALIZE);
**
**	realloc like usage
**	void* new_memory = memory_allocator(old_memory, old_size, new_size, 0, PRESERVE_OLD_MEMORY);
**
**	aligned_alloc/posix_memalign like usage
**	void* new_memory = memory_allocator(NULL, 0, new_size, new_alignment, UN_AFFECTED);
**
**	free like usage
**	memory_allocator(old_memory, old_size, 0, 0, UN_AFFECTED);
**
**	as you can see the memory_allocator interface is a function that specifies specific usecases according to the parameters passed
**	please note that this are just stdlib c specific usecases, a custom memory allocator interface must provide implementation of all possible usecases
**	and return NULL on an allocation failure
**	When the memory allocation fails the Cutlery datastructures assume that old_memory is freed and can not be used
*/

// BELOW ARE SIMPLIFIED MEMORY ALLOCATOR CALLS
// TO SIMPLIFY ACCESS OF THE MEMORY ALLOCATOR

// macros to simplify access to memory allocator
// although the memory_allocator has a lot of usecases
// these are the most basic usecases that it must satisfy

// first and foreost calls to the memory allocator
#define allocate(mem_allocator, new_size) 							memory_allocator(NULL, 0, new_size, 0, UN_AFFECTED)
#define zallocate(mem_allocator, new_size) 							memory_allocator(NULL, 0, new_size, 0, ZERO_INITIALIZE)

// subsequent reallocation calls to the memory allocator
#define reallocate(mem_allocator, old_memory, old_size, new_size) 	memory_allocator(old_memory, old_size, new_size, 0, PRESERVE_OLD_MEMORY)

// final deallocate / free call to the memory allocator
#define deallocate(mem_allocator, old_memory, old_size)				memory_allocator(old_memory, old_size, 0, 0, UN_AFFECTED)

extern const memory_allocator STD_C_mem_alloc;

#endif