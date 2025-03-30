#include<stdio.h>
#include<stdlib.h>

#include<cutlery/uc_allocator.h>

uc_allocator_context* ucac_p = NULL;

void sprint_block(dstring* append_str, const void* data_p, unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs + 1);
	snprintf_dstring(append_str, "%p(is_free = %d) size = %"PRIu_cy_uint, data_p, ((const any_block*)data_p)->is_free & 1, get_block_size_for_uc_allocator_block(ucac_p, data_p));
}

void print_ucac()
{
	printf("memory = %p, memory_size = %"PRIu_cy_uint"\n", ucac_p->memory, ucac_p->memory_size);

	printf("any_block_info size = %"PRIu_cy_uint" alignment = %"PRIu_cy_uint"\n", (cy_uint)(sizeof(any_block)), (cy_uint)(_Alignof(any_block)));
	printf("free_block_info size = %"PRIu_cy_uint" alignment = %"PRIu_cy_uint"\n", (cy_uint)(sizeof(free_block)), (cy_uint)(_Alignof(free_block)));

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

const void* allocate_with_logging(cy_uint size)
{
	const void* b = allocate_block_uc_allocator(ucac_p, size);
	if(b == NULL)
		printf("failed allocation for size %"PRIu_cy_uint"\n", size);
	else
		printf("success allocation of %p for size %"PRIu_cy_uint", (actual size = %"PRIu_cy_uint")\n", b, size, get_block_size_for_uc_allocator_block(ucac_p, b));
	return b;
}

void deallocate_with_logging(const void* b)
{
	printf("deallocation of %p of size = %"PRIu_cy_uint"\n", b, get_block_size_for_uc_allocator_block(ucac_p, b));
	deallocate_block_uc_allocator(ucac_p, b);
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

	const void* b0 = allocate_with_logging(100);
	const void* b1 = allocate_with_logging(250);
	const void* b2 = allocate_with_logging(120);
	const void* b3 = allocate_with_logging(540);
	const void* b4 = allocate_with_logging(70);
	const void* b5 = allocate_with_logging(130);

	print_ucac();

	deallocate_with_logging(b1);
	deallocate_with_logging(b4);

	print_ucac();

	const void* b6 = allocate_with_logging(60);
	const void* b7 = allocate_with_logging(300);

	print_ucac();

	deallocate_with_logging(b5);

	print_ucac();

	const void* b8 = allocate_with_logging(150);

	print_ucac();

	deallocate_with_logging(b2);
	deallocate_with_logging(b6);

	print_ucac();

	deallocate_with_logging(b0);

	print_ucac();

	deallocate_with_logging(b8);

	print_ucac();

	deallocate_with_logging(b7);

	print_ucac();

	b0 = allocate_with_logging(300);
	b1 = allocate_with_logging(400);
	b2 = allocate_with_logging(150);
	b3 = allocate_with_logging(60);
	deallocate_with_logging(b0);
	deallocate_with_logging(b2);

	print_ucac();

	b4 = allocate_with_logging(310);

	print_ucac();

	b5 = allocate_with_logging(96);
	b6 = allocate_with_logging(152);
	b7 = allocate_with_logging(304);

	print_ucac();

	deallocate_with_logging(b7);
	deallocate_with_logging(b5);

	print_ucac();

	free(memory - offset);

	return 0;
}