#include<stdio.h>
#include<stdlib.h>

#include<dstring_utf8.h>

int main()
{
	for(long code_point = 0; code_point < (1L << 21); code_point++)
	{
		char buffer[10];
		cy_uint buffer_size = 10;
		for(cy_uint i = 0; i < buffer_size; i++)
			buffer[i] = rand();

		cy_uint bytes_consumed1;
		int error1 = utf8_encode_code_point(buffer, buffer_size, code_point, &bytes_consumed1);

		if(error1 != 1)
		{
			printf("there is an error(%d) right at the encoding for %ld\n", error1, code_point);
			exit(-1);
		}

		/*printf("%ld -> ", code_point);
		for(cy_uint i = 0; i < bytes_consumed1; i++)
			printf("%hhx ", buffer[i]);
		printf("\n");*/

		cy_uint bytes_consumed2;
		long r_code_point = utf8_decode_code_point(buffer, buffer_size, &bytes_consumed2);

		if(bytes_consumed1 != bytes_consumed2)
		{
			printf("unequal bytes consumed on encoding(%"PRIu_cy_uint") and decoding(%"PRIu_cy_uint") for %ld\n", bytes_consumed1, bytes_consumed2, code_point);
			printf("code points do not match after encoding and decoding %ld != %ld\n", code_point, r_code_point);
			exit(-1);
		}

		if(r_code_point != code_point)
		{
			printf("code points do not match after encoding and decoding %ld != %ld\n", code_point, r_code_point);
			exit(-1);
		}
	}
	printf("all possible encoding and decoding of the utf8 passed\n");

	char utf8_string[25] = {0x41, 0xC3, 0xA9, 0xCE, 0xBB, 0xF0, 0x90, 0x8D, 0x88, 0x5A, 0xC3, 0xB1, 0xE4, 0xB8, 0x96, 0xF0, 0x9F, 0x92, 0xA1, 0xF0, 0x90, 0x8D, 0x88, 0xCE, 0xBB};
	long code_points[10] = {0x41, 0xE9, 0x3BB, 0x10348, 0x5A, 0xF1, 0x4E16, 0x1F4A1, 0x10348, 0x3BB};

	// test encoding
	{
		char buffer[100];
		cy_uint buffer_capacity = 100;
		cy_uint buffer_size = 0;
		for(cy_uint i = 0; i < sizeof(code_points) / sizeof(code_points[0]); i++)
		{
			cy_uint bytes_consumed;
			int error = utf8_encode_code_point(buffer + buffer_size, buffer_capacity - buffer_size, code_points[i], &bytes_consumed);
			if(error != 1)
			{
				printf("there was an error(%d) encoding the test code points at %"PRIu_cy_uint"-th code point\n", error, i);
				exit(-1);
			}
			buffer_size += bytes_consumed;
		}

		if(buffer_size != sizeof(utf8_string))
		{
			printf("error in encoding, after encoding size does not match with result\n");
			exit(-1);
		}
		printf("encoding result comparison = %d -> must be 0\n", memory_compare(buffer, utf8_string, buffer_size));
	}
	printf("encoding works\n");

	// test decoding
	{
		long buf_code_points[100];
		cy_uint buf_code_points_size = 0;
		for(cy_uint i = 0; i < sizeof(utf8_string);)
		{
			cy_uint bytes_consumed;
			buf_code_points[buf_code_points_size] = utf8_decode_code_point(utf8_string + i, sizeof(utf8_string) - i, &bytes_consumed);
			if(buf_code_points[buf_code_points_size] < 0)
			{
				printf("there was an error(%ld) decoding the test code points at %"PRIu_cy_uint" in utf8_string\n", buf_code_points[buf_code_points_size], i);
				exit(-1);
			}
			buf_code_points_size++;
			i += bytes_consumed;
		}

		if(buf_code_points_size != sizeof(code_points) / sizeof(code_points[0]))
		{
			printf("error in decoding, after decoding number of code points produced does not match with result\n");
			exit(-1);
		}
		printf("decoding result comparison = %d -> must be 0\n", memory_compare(buf_code_points, code_points, buf_code_points_size * sizeof(long)));
	}
	printf("decoding works\n");

	return 0;
}