#ifndef MEMORY_ALLOCATOR_INTERFACE_H
#define MEMORY_ALLOCATOR_INTERFACE_H

#include<cutlery_stds.h>

/*
	A memory_allocator struct consists of two pointers
	1. allocator_context
	2. allocator_function

	This struct memory_allocator enables any container data structure of this library (Cutlery)
	to use a custom memory allocator, that can be provided by you.
*/

// for building a custom memory allocator to use with Cutlery-containers, please read this header file thoroughly and follow the conventions defined

// the enum defined below defines the few ways your allocated memory is initialized before being returned
typedef enum memory_allocator_initialization memory_allocator_initialization;
enum memory_allocator_initialization
{
	DONT_CARE,	// caller does not care about initialization of the newly allocated memory, that you will return 
	ZERO,		// the new allocated memory, must contain all zeros
	PRESERVE	// preserve a minimum_of(old_size, new_size) number of bytes from old_memory, upon new allocation
};

// usage of this typedef-ed function is sohisticated and complex
// it has been designed to encapsualte the functionality of all of
// 		* malloc, calloc, realloc and aligned_alloc (or posix_memalign)

/*
** parameters
**
** allocator_context : context to this memory allocator
** old_memory        : memory allocated in the past, that you no longer require, this memory must/will be freed
** old_size          : size of memory pointed to by old_allocation (your custom allocator may or may not use this value)
** new_size          : size of new memory that you want (this is a pointer hence the actual allocated size is returned) (a NULL or (pointer to) 0 in this parameter implies - only deallocate old memory)
** new_alignment     : the alignment required for the new memory (0 or 1 represents no memory alignment requirements)
** initialization    : check enum memory_allocator_initialization
**
**
** return value
**		if there is enough amount of memory available to the system, then for a non zero value of parameter "new_size",
**		it must return a pointer that is multiple of "new_alignment", and can safely hold atleast "new_size" number of bytes
**		and these "new_size" number of bytes must be initialized according to "initialization" enum memory_allocator_initialization parameter
**
** 		The returned memory pointer must point to atleast new_size number of bytes (unless new_size is NULL or (a pointer to) 0)
**			the actual size is returned in the new_size parameter
**		The returned memory pointer may or may not point to the same old_memory
**			and Cutlery algorithms and datastructures will not make any such assumptions about implementation details of the memory allocator provided to them
**
**		this function must return NULL on any error that you encounter,
**		inclusive but not limited to 
**			* memory allocation failure due to excessive memory utilization
**
**		success of a call to only deallocate (new_size = NULL or (a pointer to) 0) can not be known, it is assumed that such a deallocate call always succeeds
**
**
**		BUT you can query if the old_memory memory is owned by the allocater, by setting new_size = old_size,
**		where retrun value of NON-NULL (same as old_memory) implies ownership, and a NULL return implies that the old_memory is not owned by the corresponding allocator
*/
typedef void* (*memory_allocator_function)(void* allocator_context, void* old_memory, cy_uint old_size, cy_uint* new_size, cy_uint new_alignment, memory_allocator_initialization initialization);

struct memory_allocator
{
	void* allocator_context;
	memory_allocator_function allocator_function;
};
typedef const struct memory_allocator* memory_allocator;

/*
**	Notes for usage :
**	
**	this notes define how a memory_allocator ma must behave or how it should be used
**
**	memory_allocator ma;
**
**	malloc like usage
**	void* new_memory = ma->allocator_function(ma->allocator_context, NULL, 0, new_size, 0, DONT_CARE);
**
**	calloc like usage
**	void* new_memory = ma->allocator_function(ma->allocator_context, NULL, 0, new_size, 0, ZERO);
**
**	realloc like usage
**	void* new_memory = ma->allocator_function(ma->allocator_context, old_memory, old_size, new_size, 0, PRESERVE);
**
**	aligned_alloc/posix_memalign like usage
**	void* new_memory = ma->allocator_function(ma->allocator_context, NULL, 0, new_size, new_alignment, DONT_CARE);
**
**	free like usage
**	ma->allocator_function(ma->allocator_context, old_memory, old_size, NULL, 0, DONT_CARE);
**		to just free the memory, the new_size must be 0 or NULL
**
**	as you can see the memory_allocator suffices all specific usecases according to the parameters passed
**	please note that this are just stdlib c specific usecases, a custom memory allocator interface must provide implementation of all possible usecases
**	and return NULL on an allocation failure
**
**	When the memory allocation fails (return value = NULL) the Cutlery datastructures assume that old_memory is not freed and can still be used
**		in this case the parameter new_size is returned unchanged
*/

// BELOW ARE SIMPLIFIED MEMORY ALLOCATOR CALLS
// TO SIMPLIFY ACCESS TO THE MEMORY ALLOCATOR

// macros to simplify access to memory allocator
// although the memory_allocator has a lot of usecases
// these are the most basic usecases that it must suffice

// first and foremost calls to the memory allocator
#define allocate(mem_allocator, new_size) 							mem_allocator->allocator_function(mem_allocator->allocator_context, NULL, 0, new_size, 0, DONT_CARE)
#define zallocate(mem_allocator, new_size) 							mem_allocator->allocator_function(mem_allocator->allocator_context, NULL, 0, new_size, 0, ZERO)

// subsequent reallocation calls to the memory allocator
#define reallocate(mem_allocator, old_memory, old_size, new_size) 	mem_allocator->allocator_function(mem_allocator->allocator_context, old_memory, old_size, new_size, 0, PRESERVE)

// final deallocate / free call to the memory allocator
#define deallocate(mem_allocator, old_memory, old_size)				mem_allocator->allocator_function(mem_allocator->allocator_context, old_memory, old_size, NULL, 0, DONT_CARE)

// aligned allocation functions of the above mcros
#define aligned_allocate(mem_allocator, new_size, new_alignment)	mem_allocator->allocator_function(mem_allocator->allocator_context, NULL, 0, new_size, new_alignment, DONT_CARE)
#define aligned_zallocate(mem_allocator, new_size, new_alignment) 	mem_allocator->allocator_function(mem_allocator->allocator_context, NULL, 0, new_size, new_alignment, ZERO)
#define aligned_reallocate(mem_allocator, old_memory, old_size, new_size, new_alignment) 	mem_allocator->allocator_function(mem_allocator->allocator_context, old_memory, old_size, new_size, new_alignment, PRESERVE)

// the memory allocator interface for a STD_C library functions like malloc, calloc, aligned_alloc, and free
extern struct memory_allocator STD_C_memory_allocator;
extern memory_allocator STD_C_mem_allocator;
// please check the source file, and use STD_C_mem_allocator as a reference to implement your custom memory_allocator

#endif
