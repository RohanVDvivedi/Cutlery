#include<uc_allocator.h>

// only default initializes the any_block struct
static void initialize_any_block(any_block* b, int is_free)
{
	b->is_free = is_free;
	b->is_marked = 0;
	b->is_temporary = 0;
	initialize_llnode(&(b->ab_node));
}

// only default initializes the free_block struct
static void initialize_free_block(free_block* b)
{
	initialize_any_block(&(b->any), 1); // marked is_free = 1
	initialize_bstnode(&(b->fb_node));
}

// if it returns NULL, then the alignment or space constraints do not allow this
static free_block* carve_out_free_block_in_memory_region(void* memory, cy_uint memory_size)
{
	// find first pointer in memory region that has right alignment
	void* b = (void*) memory_get_first_aigned_in_region(memory, memory_size, _Alignof(free_block));
	if(b == NULL)
		return NULL;

	// ensure that there are enough byte to atleast hold a free_block
	cy_uint b_size = (memory + memory_size - b);
	if(b_size < sizeof(free_block))
		return NULL;

	// initialize it
	initialize_free_block(b);

	// return it
	return b;
}

// function to be used for contexted comparator for free_blocks bst
static int compare_by_sizes_for_free_blocks_bst(const uc_allocator_context* ucac_p, const free_block* b1, const free_block* b2)
{
	cy_uint b1_size = get_block_size_for_uc_allocator_block(ucac_p, (const any_block*)b1);
	cy_uint b2_size = get_block_size_for_uc_allocator_block(ucac_p, (const any_block*)b2);
	return compare_numbers(b1_size, b2_size);
}

cy_uint get_block_size_for_uc_allocator_block(const uc_allocator_context* ucac_p, const any_block* b)
{
	// a temporary block's size is present in the block itself
	if(b->is_temporary)
		return b->temporary_block_size;

	// if it is the tail block it extends until the end of the memory
	const void* all_blocks_tail = get_tail_of_linkedlist(&(ucac_p->all_blocks));
	if(all_blocks_tail == b)
		return ucac_p->memory + ucac_p->memory_size - ((const void*)b);

	// else it extends until the next block in the all_blocks linkedlist
	const void* b_next = get_next_of_in_linkedlist(&(ucac_p->all_blocks), b);
	return b_next - ((const void*)b);
}

int initialize_uc_allocator_context(uc_allocator_context* ucac_p, void* memory, cy_uint memory_size)
{
	ucac_p->memory = memory;
	ucac_p->memory_size = memory_size;
	initialize_linkedlist(&(ucac_p->all_blocks), offsetof(any_block, ab_node));
	initialize_bst(&(ucac_p->free_blocks), RED_BLACK_TREE, &contexted_comparator(ucac_p, ((int (*)(const void*, const void*, const void*))compare_by_sizes_for_free_blocks_bst)), offsetof(free_block, fb_node));

	// carve out the first and the only free block from the memory region provided
	free_block* b = carve_out_free_block_in_memory_region(memory, memory_size);
	if(b == NULL)
		return 0;

	insert_head_in_linkedlist(&(ucac_p->all_blocks), b);
	insert_in_bst(&(ucac_p->free_blocks), b);
	return 1;
}

const any_block* get_head_block_for_uc_allocator(const uc_allocator_context* ucac_p)
{
	return get_head_of_linkedlist(&(ucac_p->all_blocks));
}

const any_block* get_next_block_for_uc_allocator(const uc_allocator_context* ucac_p, const any_block* b)
{
	// if it is tail return NULL
	const void* all_blocks_tail = get_tail_of_linkedlist(&(ucac_p->all_blocks));
	if(all_blocks_tail == b)
		return NULL;

	// else return the next
	return get_next_of_in_linkedlist(&(ucac_p->all_blocks), b);
}

const any_block* allocate_block_uc_allocator(uc_allocator_context* ucac_p, cy_uint size)
{
	// can not allocate lesser than free_block worth of memory
	size = max(size, sizeof(free_block));

	void* b = (void*) find_succeeding_or_equals_in_bst(&(ucac_p->free_blocks), &((any_block){.is_temporary = 1, .temporary_block_size = size}));
	if(b == NULL)
		return NULL;

	// remove it from free_blocks
	remove_from_bst(&(ucac_p->free_blocks), b);

	// mark it not free
	((any_block*)b)->is_free = 0;

	// perform a split if possible
	{
		cy_uint b_size = get_block_size_for_uc_allocator_block(ucac_p, b);
		if(b_size - size >= sizeof(free_block))
		{
			free_block* cb = carve_out_free_block_in_memory_region(b + size, b_size - size);
			if(cb != NULL)
			{
				insert_after_in_linkedlist(&(ucac_p->all_blocks), b, cb);
				insert_in_bst(&(ucac_p->free_blocks), cb);
			}
		}
	}

	return b;
}

void deallocate_block_uc_allocator(uc_allocator_context* ucac_p, any_block* _b)
{
	void* b = _b;

	// mark it free
	((any_block*)b)->is_free = 1;

	// initialize its fb_node
	initialize_bstnode(&(((free_block*)b)->fb_node));

	// insert it in free_blocks
	insert_in_bst(&(ucac_p->free_blocks), b);

	// find the next one and try to merge with it
	{
		// if it is tail skip
		if(b == get_tail_of_linkedlist(&(ucac_p->all_blocks)))
			goto SKIP_MERGE_WITH_NEXT;

		// grab b_next the next of b
		void* b_next = (void*) get_next_of_in_linkedlist(&(ucac_p->all_blocks), b);

		// if it is allocated then skip
		if(((any_block*)b_next)->is_free == 0)
			goto SKIP_MERGE_WITH_NEXT;

		// remove them both from the free_blocks temporarily
		remove_from_bst(&(ucac_p->free_blocks), b);
		remove_from_bst(&(ucac_p->free_blocks), b_next);

		// remove b_next from the all_blocks
		remove_from_linkedlist(&(ucac_p->all_blocks), b_next);

		// insert b back into free_blocks
		insert_in_bst(&(ucac_p->free_blocks), b);
	}
	SKIP_MERGE_WITH_NEXT:;

	// Probably, we merged the next of b with b, if so, the next is destroyed, but the reference b still lives as a bigger block

	// find the prev one and try to merge with it
	{
		// if it is head skip
		if(b == get_head_of_linkedlist(&(ucac_p->all_blocks)))
			goto SKIP_MERGE_WITH_PREV;

		// grab b_prev the prev of b
		void* b_prev = (void*) get_prev_of_in_linkedlist(&(ucac_p->all_blocks), b);

		// if it is allocated then skip
		if(((any_block*)b_prev)->is_free == 0)
			goto SKIP_MERGE_WITH_PREV;

		// remove them both from the free_blocks temporarily
		remove_from_bst(&(ucac_p->free_blocks), b);
		remove_from_bst(&(ucac_p->free_blocks), b_prev);

		// remove b from the all_blocks
		remove_from_linkedlist(&(ucac_p->all_blocks), b);

		// insert b back into free_blocks
		insert_in_bst(&(ucac_p->free_blocks), b_prev);
	}
	SKIP_MERGE_WITH_PREV:;
}