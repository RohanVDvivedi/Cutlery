#include<dstring_utf8.h>

int is_valid_utf8_code_point(long code_point)
{
	return (code_point < (1L << 21));
}

cy_uint whole_bytes_required_for_encoding_utf8_code_point(long code_point)
{
	if(!is_valid_utf8_code_point(code_point))
		return 0;

	if(code_point < (1L << (8 - 1)))
		return 1;
	else if(code_point < (1L << ((8-3) + 6*1)))
		return 2;
	else if(code_point < (1L << ((8-4) + 6*2)))
		return 3;
	else if(code_point < (1L << ((8-5) + 6*3)))
		return 4;
	else
		return 0;
}

long utf8_decode_code_point(const char* data, cy_uint data_size, cy_uint* bytes_consumed);

int utf8_encode_code_point(char* data, cy_uint data_size, long code_point, cy_uint* bytes_consumed);