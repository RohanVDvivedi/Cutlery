#include<stdio.h>
#include<stdlib.h>

#include<dstring_utf8.h>

int main()
{
	for(long code_point = 0; code_point < (1L << 21); code_point++)
	{
		char buffer[10];
		cy_uint buffer_size;

		cy_uint bytes_consumed1;
		int error1 = utf8_encode_code_point(buffer, buffer_size, code_point, &bytes_consumed1);

		cy_uint bytes_consumed2;
		long r_code_point = utf8_decode_code_point(buffer, buffer_size, &bytes_consumed2);

		if(bytes_consumed1 != bytes_consumed2)
		{
			printf("unequal bytes consumed on encoding and decoding for %ld\n", code_point);
			exit(-1);
		}

		if(r_code_point != code_point)
		{
			printf("code points do not match after encoding and decoding %ld != %ld\n", code_point, r_code_point);
			exit(-1);
		}
	}
	return 0;
}