#include<uc_allocator.h>

// only default initializes the any_block struct
static void initialize_any_block(any_block* b, int is_free)
{
	b->is_free = is_free;
	b->is_marked = 0;
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