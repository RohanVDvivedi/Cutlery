#include<dpipe.h>

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void write_to_dpipe_wrapper(dpipe* pipe, const char* str, dpipe_operation_type op_type)
{
	printf("writing \"%s\" => %u\n\n", str, write_to_dpipe(pipe, str, strlen(str), op_type));
}

unsigned int read_from_dpipe_wrapper(dpipe* pipe, char* str, unsigned int bytes_read, dpipe_operation_type op_type)
{
	memset(str, 0, bytes_read + 1);
	bytes_read = read_from_dpipe(pipe, str, bytes_read, op_type);
	printf("read \"%s\" => %u\n\n", str, bytes_read);
}

void unread_to_dpipe_wrapper(dpipe* pipe, const char* str, dpipe_operation_type op_type)
{
	printf("writing %s => %u\n\n", str, unread_to_dpipe(pipe, str, strlen(str), op_type));
}

void resize_dpipe_wrapper(dpipe* pipe, unsigned int new_capacity)
{
	printf("before resize => first = %u count = %u\n", pipe->first_byte, pipe->byte_count);
	int resized = 0;
	printf("resize to %u bytes : %d\n", new_capacity, resized = resize_dpipe(pipe, new_capacity));
	if(resized)
		printf("after resize => first = %u count = %u\n", pipe->first_byte, pipe->byte_count);
	printf("\n");
}

char read_buff[128] = {};

int main()
{
	dpipe temp;
	dpipe* pipe = &temp;

	initialize_dpipe(pipe, 32);

	write_to_dpipe_wrapper(pipe, "Rohan is good boy.", ALL_OR_NONE);
	write_to_dpipe_wrapper(pipe, "Rohan is bad boy.", ALL_OR_NONE);
	write_to_dpipe_wrapper(pipe, "Rohan is bad boy.", PARTIAL_ALLOWED);

	read_from_dpipe_wrapper(pipe, read_buff, 18, ALL_OR_NONE);
	read_from_dpipe_wrapper(pipe, read_buff, 17, ALL_OR_NONE);
	read_from_dpipe_wrapper(pipe, read_buff, 17, PARTIAL_ALLOWED);

	write_to_dpipe_wrapper(pipe, "RohanRupaVipulDevashree", ALL_OR_NONE);
	read_from_dpipe_wrapper(pipe, read_buff, 5, ALL_OR_NONE);
	unread_to_dpipe_wrapper(pipe, "RohanDvivedi", ALL_OR_NONE);

	read_from_dpipe_wrapper(pipe, read_buff, 5, ALL_OR_NONE);
	read_from_dpipe_wrapper(pipe, read_buff, 11, ALL_OR_NONE);

	write_to_dpipe_wrapper(pipe, " RohanRupa", ALL_OR_NONE);

	resize_dpipe_wrapper(pipe, 24);

	read_from_dpipe_wrapper(pipe, read_buff, 15, ALL_OR_NONE);

	write_to_dpipe_wrapper(pipe, " Hello World", ALL_OR_NONE);

	resize_dpipe_wrapper(pipe, 24);

	resize_dpipe_wrapper(pipe, 64);

	unread_to_dpipe_wrapper(pipe, "VipulDevashree", ALL_OR_NONE);

	resize_dpipe_wrapper(pipe, 80);

	read_from_dpipe_wrapper(pipe, read_buff, 24, ALL_OR_NONE);

	resize_dpipe_wrapper(pipe, 77);

	resize_dpipe_wrapper(pipe, 32);

	resize_dpipe_wrapper(pipe, 16);

	read_from_dpipe_wrapper(pipe, read_buff, 6, PARTIAL_ALLOWED);

	write_to_dpipe_wrapper(pipe, "RohanVipul", ALL_OR_NONE);

	resize_dpipe_wrapper(pipe, 0);

	unsigned int bytes_popped_back = get_back_of_dpipe(pipe, read_buff, 8, PARTIAL_ALLOWED);
	pop_back_from_dpipe(pipe, bytes_popped_back);
	printf("popped_back \"%.*s\" => %u\n\n", bytes_popped_back, read_buff, bytes_popped_back);

	bytes_popped_back = get_back_of_dpipe(pipe, read_buff, 8, PARTIAL_ALLOWED);
	pop_back_from_dpipe(pipe, bytes_popped_back);
	printf("popped_back \"%.*s\" => %u\n\n", bytes_popped_back, read_buff, bytes_popped_back);

	write_to_dpipe_wrapper(pipe, "XXXXX", ALL_OR_NONE);

	resize_dpipe_wrapper(pipe, 0);

	remove_all_from_dpipe(pipe);
	printf("removing all :: %d\n", pipe->byte_count == 0);

	deinitialize_dpipe(pipe);

	return 0;
}