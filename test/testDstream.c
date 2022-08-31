#include<dstream.h>

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void write_to_dstream_wrapper(dstream* strm, const char* str, dstream_operation_type op_type)
{
	printf("writing \"%s\" => %u\n\n", str, write_to_dstream(strm, str, strlen(str), op_type));
}

unsigned int read_from_dstream_wrapper(dstream* strm, char* str, unsigned int bytes_read, dstream_operation_type op_type)
{
	memset(str, 0, bytes_read + 1);
	bytes_read = read_from_dstream(strm, str, bytes_read, op_type);
	printf("read \"%s\" => %u\n\n", str, bytes_read);
}

void unread_to_dstream_wrapper(dstream* strm, const char* str, dstream_operation_type op_type)
{
	printf("writing %s => %u\n\n", str, unread_to_dstream(strm, str, strlen(str), op_type));
}

void resize_dstream_wrapper(dstream* strm, unsigned int new_capacity)
{
	printf("before resize => first = %u count = %u\n", strm->first_byte, strm->byte_count);
	int resized = 0;
	printf("resize to %u bytes : %d\n", new_capacity, resized = resize_dstream(strm, new_capacity));
	if(resized)
		printf("after resize => first = %u count = %u\n", strm->first_byte, strm->byte_count);
	printf("\n");
}

char read_buff[128] = {};

int main()
{
	dstream temp;
	dstream* strm = &temp;

	initialize_dstream(strm, 32);

	write_to_dstream_wrapper(strm, "Rohan is good boy.", ALL_OR_NONE);
	write_to_dstream_wrapper(strm, "Rohan is bad boy.", ALL_OR_NONE);
	write_to_dstream_wrapper(strm, "Rohan is bad boy.", PARTIAL_ALLOWED);

	read_from_dstream_wrapper(strm, read_buff, 18, ALL_OR_NONE);
	read_from_dstream_wrapper(strm, read_buff, 17, ALL_OR_NONE);
	read_from_dstream_wrapper(strm, read_buff, 17, PARTIAL_ALLOWED);

	write_to_dstream_wrapper(strm, "RohanRupaVipulDevashree", ALL_OR_NONE);
	read_from_dstream_wrapper(strm, read_buff, 5, ALL_OR_NONE);
	unread_to_dstream_wrapper(strm, "RohanDvivedi", ALL_OR_NONE);

	read_from_dstream_wrapper(strm, read_buff, 5, ALL_OR_NONE);
	read_from_dstream_wrapper(strm, read_buff, 11, ALL_OR_NONE);

	write_to_dstream_wrapper(strm, " RohanRupa", ALL_OR_NONE);

	resize_dstream_wrapper(strm, 24);

	read_from_dstream_wrapper(strm, read_buff, 15, ALL_OR_NONE);

	write_to_dstream_wrapper(strm, " Hello World", ALL_OR_NONE);

	resize_dstream_wrapper(strm, 24);

	resize_dstream_wrapper(strm, 64);

	unread_to_dstream_wrapper(strm, "VipulDevashree", ALL_OR_NONE);

	resize_dstream_wrapper(strm, 80);

	read_from_dstream_wrapper(strm, read_buff, 24, ALL_OR_NONE);

	resize_dstream_wrapper(strm, 77);

	resize_dstream_wrapper(strm, 32);

	resize_dstream_wrapper(strm, 16);

	read_from_dstream_wrapper(strm, read_buff, 6, PARTIAL_ALLOWED);

	write_to_dstream_wrapper(strm, "RohanVipul", ALL_OR_NONE);

	resize_dstream_wrapper(strm, 0);

	unsigned int bytes_popped_back = pop_back_from_dstream(strm, read_buff, 8, PARTIAL_ALLOWED);
	printf("popped_back \"%.*s\" => %u\n\n", bytes_popped_back, read_buff, bytes_popped_back);

	bytes_popped_back = pop_back_from_dstream(strm, read_buff, 8, PARTIAL_ALLOWED);
	printf("popped_back \"%.*s\" => %u\n\n", bytes_popped_back, read_buff, bytes_popped_back);

	write_to_dstream_wrapper(strm, "XXXXX", ALL_OR_NONE);

	resize_dstream_wrapper(strm, 0);

	remove_all_from_dstream(strm);
	printf("removing all :: %d\n", strm->byte_count == 0);

	deinitialize_dstream(strm);

	return 0;
}