#include<uc_allocator_interface.h>

static void* uc_allocator_function(void* allocator_context, void* old_memory, cy_uint old_size, cy_uint* new_size, cy_uint new_alignment, memory_allocator_initialization initialization);

int initialize_uc_allocator(struct memory_allocator* uca, uc_allocator_context* ucac_p, void* memory, cy_uint memory_size)
{
	if(!initialize_uc_allocator_context(ucac_p, memory, memory_size))
		return 0;

	uca->allocator_context = ucac_p;
	uca->allocator_function = uc_allocator_function;
	return 1;
}

static void* uc_allocator_function(void* allocator_context, void* old_memory, cy_uint old_size, cy_uint* new_size, cy_uint new_alignment, memory_allocator_initialization initialization)
{
	// TODO
}