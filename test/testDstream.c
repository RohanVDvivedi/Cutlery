#include<dstream.h>

void write_to_dstream_wrapper(dstream* strm, const char* str, dstream_operation_type op_type)
{
	printf("writing \"%s\" => %u\n", str, write_to_dstream(strm, str, strlen(str), op_type));
}

unsigned int read_from_dstream_wrapper(dstream* strm, char* str, unsigned int bytes_read, dstream_operation_type op_type)
{
	memset(str, 0, bytes_read + 1);
	bytes_read = read_from_dstream(strm, str, bytes_read, op_type);
	printf("read \"%s\" => %u\n", str, bytes_read);
}

void unread_to_dstream_wrapper(dstream* strm, const char* str, dstream_operation_type op_type)
{
	printf("writing %s => %u\n", str, unread_to_dstream(strm, str, strlen(str), op_type));
}

int main()
{
	dstream temp;
	dstream* strm = &temp;

	initialize_dstream(strm, 32);

	write_to_dstream_wrapper(strm, "Rohan is good boy.", ALL_OR_NONE);
	write_to_dstream_wrapper(strm, "Rohan is bad boy.", ALL_OR_NONE);

	return 0;
}