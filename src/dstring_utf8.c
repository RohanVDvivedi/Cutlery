#include<dstring_utf8.h>

int is_valid_utf8_code_point(long code_point)
{
	return (code_point < (1L << 21));
}

cy_uint whole_bytes_required_for_encoding_utf8_code_point(long code_point);

long utf8_decode_code_point(const char* data, cy_uint data_size, cy_uint* bytes_consumed);

int utf8_encode_code_point(char* data, cy_uint data_size, long code_point, cy_uint* bytes_consumed);