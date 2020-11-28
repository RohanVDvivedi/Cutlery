#include<memory_allocator_interface.h>

#include<stdlib.h>

void* STD_C_mem_alloc_impl(void* old_memory, unsigned int old_size, unsigned int new_size, unsigned int new_alignment, memory_allocator_initialization initialization)
{
	return NULL;
}
const memory_allocator STD_C_mem_alloc = STD_C_mem_alloc_impl;