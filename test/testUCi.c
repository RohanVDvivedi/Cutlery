#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<cutlery/uc_allocator_interface.h>

memory_allocator uca;
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

#define MEMORY_SIZE 1024
#define ALIGNMENT 5

int main()
{
	uc_allocator_context ucac;
	ucac_p = &ucac;

	struct memory_allocator uca_v;
	struct memory_allocator* uca = &uca_v;

	void* memory = malloc(MEMORY_SIZE);
	if(!initialize_uc_allocator(uca, ucac_p, memory, MEMORY_SIZE))
	{
		printf("initialization of uc allocator interface unsuccessfull\n");
		exit(-1);
	}

	cy_uint capacity = 60;
	char* str = aligned_allocate(uca, &capacity, ALIGNMENT);
	if(str == NULL)
	{
		printf("failed to allocate memeory with capacity = %"PRIu_cy_uint"\n", capacity);
		exit(-1);
	}
	printf("allocated %p %"PRIu_cy_uint"\n", str, capacity);
	strncpy(str, "Rohan Dvivedi, Rupa Dvivedi, Vipulkumar Dvivedi, Devashree Joshi...", min(capacity, 1 + strlen("Rohan Dvivedi, Rupa Dvivedi, Vipulkumar Dvivedi, Devashree Joshi...")));
	printf("->%.*s\n", (int)capacity, str);

	print_ucac();

	cy_uint new_capacity = 40;
	str = aligned_reallocate(uca, str, capacity, &new_capacity, ALIGNMENT);
	if(str == NULL)
	{
		printf("failed to allocate memeory with capacity = %"PRIu_cy_uint"\n", capacity);
		exit(-1);
	}
	capacity = new_capacity;
	printf("allocated %p %"PRIu_cy_uint"\n", str, capacity);
	printf("->%.*s\n", (int)capacity, str);

	print_ucac();

	new_capacity = 90;
	str = aligned_reallocate(uca, str, capacity, &new_capacity, ALIGNMENT);
	if(str == NULL)
	{
		printf("failed to allocate memeory with capacity = %"PRIu_cy_uint"\n", capacity);
		exit(-1);
	}
	if(new_capacity > capacity)
		str[capacity] = '\0';
	capacity = new_capacity;
	printf("allocated %p %"PRIu_cy_uint"\n", str, capacity);
	printf("->%.*s\n", (int)capacity, str);

	print_ucac();

	new_capacity = 20;
	str = uca->allocator_function(uca->allocator_context, str, capacity, &new_capacity, ALIGNMENT, ZERO);
	if(str == NULL)
	{
		printf("failed to allocate memeory with capacity = %"PRIu_cy_uint"\n", capacity);
		exit(-1);
	}
	capacity = new_capacity;
	printf("allocated %p %"PRIu_cy_uint"\n", str, capacity);
	printf("->%.*s\n", (int)capacity, str);

	print_ucac();

	printf("deallocating %p %"PRIu_cy_uint"\n", str, capacity);
	deallocate(uca, str, capacity);

	print_ucac();

	free(memory);

	return 0;
}