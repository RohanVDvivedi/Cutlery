#ifndef UC_ALLOCATOR_H
#define UC_ALLOCATOR_H

/*
	This is a binary search tree based best fit arena memory allocator for microcontrollers.
	But unlike arena memory allocators, you can free blocks of memory.
*/

#include<cutlery_stds.h>
#include<cutlery_math.h>

#include<linkedlist.h>
#include<bst.h>

// prefix of every block
typedef struct any_block any_block;
struct any_block
{
	int is_free : 1;
	int is_marked : 1; // bit left here for GC implementations to utilize, lft untouched by the allocator

	llnode ab_node; // intrusive llnode for all_blocks linkedlist inside the uc_allocator context
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

#endif