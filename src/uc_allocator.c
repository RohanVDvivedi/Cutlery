#include<uc_allocator.h>

// only default initializes the any_block struct
static void initialize_any_block(any_block* b);

// only default initializes the free_block struct
static void initialize_free_block(free_block* b);

// if it returns NULL, then the alignment or space constraints do not allow this
static free_block* carve_out_free_block_in_memory_region(void* memory, cy_uint size);

// function to be used for contexted comparator for free_blocks bst
static int compare_by_sizes_for_free_blocks_bst(const uc_allocator_context* ucac_p, const free_block* b1, const free_block* b2);