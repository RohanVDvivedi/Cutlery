#include<stdio.h>
#include<stdlib.h>

#include<cutlery/stream_for_file_descriptor.h>
#include<cutlery/stream_for_dstring.h>
#include<cutlery/stream_util.h>

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
		{
			printf("error : %d\n", error);
			exit(-1);
		}

		close_stream(&s2, &error);
		deinitialize_stream(&s2);
	}

	{
		dstring s1;
		init_empty_dstring(&s1, 0);
		stream s2;
		initialize_dstring_stream(&s2, &s1);

		int error = 0;

		for(int i = 0; i < 100; i++)
		{
			write_to_stream_formatted(&s2, &error, "%d,", i);
			if(error)
				break;
			flush_all_from_stream(&s2, &error);
			if(error)
				break;
		}
		if(!error)
			write_to_stream_formatted(&s2, &error, "\n");
		if(!error)
			flush_all_from_stream(&s2, &error);

		if(error)
		{
			printf("error : %d\n", error);
			exit(-1);
		}

		printf(printf_dstring_format, printf_dstring_params(&s1));

		#define data_capacity 10
		char data[data_capacity];
		cy_uint bytes_read = 0;

		while(1)
		{
			bytes_read = read_from_stream(&s2, data, data_capacity, &error);
			if(error || bytes_read == 0)
				break;
			write_to_stream_formatted(&stdout, &error, printf_dstring_format, bytes_read, data);
			if(error)
				break;
		}
		if(!error)
			flush_all_from_stream(&stdout, &error);

		if(error)
		{
			printf("error : %d\n", error);
			exit(-1);
		}

		close_stream(&s2, &error);
		deinitialize_stream(&s2);
			

		deinit_dstring(&s1);
	}

	deinitialize_stream(&stdin);
	deinitialize_stream(&stdout);

	return 0;
}