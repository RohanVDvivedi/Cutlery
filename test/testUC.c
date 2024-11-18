#include<stdio.h>
#include<stdlib.h>

#include<uc_allocator.h>

uc_allocator_context* ucac_p = NULL;

void sprint_block(dstring* append_str, const void* data_p, unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs + 1);
	snprintf_dstring(append_str, "%p(is_free = %d) size = %"PRIu_cy_uint, data_p, ((const any_block*)data_p)->is_free & 1, get_block_size_for_uc_allocator_block(ucac_p, data_p));
}

void print_ucac()
{
	printf("memory = %p, memory_size = %"PRIu_cy_uint"\n", ucac_p->memory, ucac_p->memory_size);

	printf("any_block_info size = %"PRIu_cy_uint" alignment = %"PRIu_cy_uint"\n", sizeof(any_block), _Alignof(any_block));
	printf("free_block_info size = %"PRIu_cy_uint" alignment = %"PRIu_cy_uint"\n", sizeof(free_block), _Alignof(free_block));

	dstring str;
	init_empty_dstring(&str, 0);

	printf("all_blocks => \n");
	make_dstring_empty(&str);
	sprint_linkedlist(&str, &(ucac_p->all_blocks), sprint_block, 1);
	printf_dstring(&str);
	printf("\n");

	printf("free_blocks => \n");
	make_dstring_empty(&str);
	sprint_bst(&str, &(ucac_p->free_blocks), sprint_block, 1);
	printf_dstring(&str);
	printf("\n");

	deinit_dstring(&str);
}

int main()
{
	// allocate some memory for the allocator
	cy_uint memory_size = 1024;
	void* memory = malloc(memory_size);

	cy_uint offset = 3;

	// unalign it
	memory += offset;
	memory_size -= offset;

	uc_allocator_context ucac;
	ucac_p = &ucac;
	if(!initialize_uc_allocator_context(ucac_p, memory, memory_size))
	{
		printf("failed to initialize uc_allocator\n");
		exit(-1);
	}

	print_ucac();


	free(memory - offset);

	return 0;
}