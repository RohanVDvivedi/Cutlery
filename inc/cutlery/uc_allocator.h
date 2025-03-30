#ifndef UC_ALLOCATOR_H
#define UC_ALLOCATOR_H

/*
	This is a binary search tree based best fit arena memory allocator for microcontrollers.
	But unlike arena memory allocators, you can free blocks, before you destroy the arena completely.
*/

#include<cutlery/cutlery_stds.h>
#include<cutlery/cutlery_math.h>

#include<cutlery/linkedlist.h>
#include<cutlery/bst.h>

// prefix of every block
typedef struct any_block any_block;
struct any_block
{
	int is_free : 1;
	int is_marked : 1; // bit left here for GC implementations to utilize, left untouched by the allocator

	int is_temporary : 1;

	union
	{
		llnode ab_node; // intrusive llnode for all_blocks linkedlist inside the uc_allocator context

		cy_uint temporary_block_size; // valid only if is_temporary is set
		// temporary blocks are used as local variable to find the next larger or equal sized block from the free_blocks bst
	};
};

// prefix of every free block
typedef struct free_block free_block;
struct free_block
{
	// any must always be the first attribute of this struct
	any_block any; // every free_block is an any_block

	bstnode fb_node; // intrusive bstnode for free_blocks bst inside the uc_allocator context
};

typedef struct uc_allocator_context uc_allocator_context;
struct uc_allocator_context
{
	void* memory;
	cy_uint memory_size;

	// remember the first block in the all_blocks may not be at pointer memory, due to alignment constraints

	linkedlist all_blocks; // contains all the blocks, the position of the block here controls its size

	bst free_blocks;	// compared by get_block_size

	// only a block present in all_blocks should be present in free_blocks
	// all blocks must be present in all_blocks but only free blocks are present in free_blocks
};

// fails if alignment and size constraints do not allow initializing a uc_allocator at the desired memory block
int initialize_uc_allocator_context(uc_allocator_context* ucac_p, void* memory, cy_uint memory_size);

// functions to be used externally to iterate over all the blocks
// they return const, because you are not generally allowed to modify them, unless it is the is_marked bit - which is of no use in the near future
const any_block* get_head_block_for_uc_allocator(const uc_allocator_context* ucac_p);
const any_block* get_next_block_for_uc_allocator(const uc_allocator_context* ucac_p, const any_block* b); // next of tail is NULL

// get block size for any block, allocated or not
// this will always be the complete size of the block, including the any_block/free_block prefix structs
cy_uint get_block_size_for_uc_allocator_block(const uc_allocator_context* ucac_p, const any_block* b);

// returns some any_block or NULL, if a non-NULL is returned, then it is atleast 'size' bytes big
// again this 'size' includes the any_block/free_block prefix structs
// alignment of this block as expected to be same as that of free_block `REMEBER, it was free prior to allocation.`
// the return value is a const pointer because you are only allowed to touch memory that is after the prefix of any_block struct
// i.e. you are allowed to use the memory only after the first sizeof(any_block) bytes of memory
const any_block* allocate_block_uc_allocator(uc_allocator_context* ucac_p, cy_uint size);
void deallocate_block_uc_allocator(uc_allocator_context* ucac_p, const any_block* b);

#endif