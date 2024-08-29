#include<stdio.h>

#include<stream_for_file_descriptor.h>
#include<stream_for_dstring.h>
#include<stream_util.h>

int main()
{
	stream stdin;
	stream stdout;
	initialize_stream_for_fd(&stdin, 1);
	initialize_stream_for_fd(&stdout, 0);

	// test 1
	{
		dstring s1 = get_dstring_pointing_to_literal_cstring("Hi, this is rohan, and this be printed from a stream at the rate of 10 bytes at a time, so hold on !!! just like that !!?!!");
		
		stream s2;
		initialize_dstring_stream(&s2, &s1);

		#define data_capacity 10
		char data[data_capacity];
		cy_uint bytes_read = 0;
		int error = 0;

		while(1)
		{
			bytes_read = read_from_stream(&s2, data, data_capacity, &error);
			if(error || bytes_read == 0)
				break;
			write_to_stream_formatted(&stdout, &error, "data : <" printf_dstring_format ">\n", bytes_read, data);
			if(error)
				break;
		}
		if(!error)
			flush_all_from_stream(&stdout, &error);

		if(error)
			printf("error : %d\n", error);

		close_stream(&s2, &error);
		deinitialize_stream(&s2);
	}

	return 0;
}