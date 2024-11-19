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
	uc_allocator_context* ucac_p = allocator_context;

	// figure out the old allocation any_block and the old_size (of old_memory), we do not trust the old_size
	any_block* old_block = NULL;
	if(old_memory != NULL)
	{
		// pointer to the any block is right before the block
		memory_move(&old_block, old_memory - sizeof(void*), sizeof(void*));

		// self calculate the old_size, which is equal to the block_size - (offset of the old_memory in old_block)
		old_size = get_block_size_for_uc_allocator_block(ucac_p, old_block) - (old_memory - ((void*)old_block));
	}
	else
		old_size = 0;

	// if old_size is equal to new_size, then return old_memory
	if(old_size == (*new_size))
	{
		// PRESERVE and DONT_CARE are already handled
		// but need to handle ZERO initialization
		if(initialization == ZERO)
			memory_set(old_memory, 0, old_size);
		return old_memory;
	}

	// new_alignment = 0, implies new_alignment = 1, i.e. no alignment constraints
	if(new_alignment == 0)
		new_alignment = 1;

	// perform new allocation, adjust new pointer and set the new_size to be returned
	void* new_memory = NULL;
	if((*new_size) > 0)
	{
		// calculate the new block size
		cy_uint new_block_size = sizeof(any_block) + (new_alignment - 1) + sizeof(void*) + (*new_size);

		// allocate new block
		any_block* new_block = (any_block*) allocate_block_uc_allocator(ucac_p, new_block_size);
		if(new_block == NULL) // fail if allocation fails
			return NULL;

		// calculate the new)memory pointer
		new_memory = ((void*)new_block) + sizeof(any_block) + sizeof(void*);
		new_memory = (void*)(UINT_ALIGN_UP(((cy_uint)new_memory), new_alignment));

		// write new_block pointer value to the prior bytes of the new_memory
		memory_move(new_memory - sizeof(void*), &new_block, sizeof(void*));

		// calculate new_size
		(*new_size) = get_block_size_for_uc_allocator_block(ucac_p, new_block) - (new_memory - ((void*)new_block));
	}

	// initialize new allocation
	switch(initialization)
	{
		default :
		case DONT_CARE :
		{
			break;
		}
		case ZERO :
		{
			memory_set(new_memory, 0, (*new_size));
			break;
		}
		case PRESERVE :
		{
			memory_move(new_memory, old_memory, min((*new_size), old_size));
			break;
		}
	}

	// free old allocation
	if(old_block != NULL)
		deallocate_block_uc_allocator(ucac_p, old_block);

	return new_memory;
}