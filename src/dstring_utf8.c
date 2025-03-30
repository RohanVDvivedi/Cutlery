#include<cutlery/dstring_utf8.h>

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

long utf8_decode_code_point(const char* data, cy_uint data_size, cy_uint* bytes_consumed)
{
	if(data_size == 0)
	{
		(*bytes_consumed) = 0;
		return UTF8_TOO_FEW_BYTES;
	}

	long code_point = 0;

	if((data[0] & 0x80) == 0x00)
	{
		(*bytes_consumed) = 1;
		code_point = data[0];
	}
	else if((data[0] & 0xe0) == 0xc0)
	{
		(*bytes_consumed) = 2;
		code_point = (data[0] & 0x1f);
	}
	else if((data[0] & 0xf0) == 0xe0)
	{
		(*bytes_consumed) = 3;
		code_point = (data[0] & 0xf);
	}
	else if((data[0] & 0xf8) == 0xf0)
	{
		(*bytes_consumed) = 4;
		code_point = (data[0] & 0x07);
	}
	else
	{
		(*bytes_consumed) = 0;
		return UTF8_ENCODING_ERROR;
	}

	if((*bytes_consumed) > data_size)
	{
		(*bytes_consumed) = 0;
		return UTF8_TOO_FEW_BYTES;
	}

	for(cy_uint i = 1; i < (*bytes_consumed); i++)
	{
		if((data[i] & 0xc0) != 0x80)
		{
			(*bytes_consumed) = 0;
			return UTF8_ENCODING_ERROR;
		}
		code_point = (code_point << 6) | (data[i] & (0x3f));
	}

	// if the code point was represented with more bytes then necessary then fail it
	if((*bytes_consumed) != whole_bytes_required_for_encoding_utf8_code_point(code_point))
	{
		(*bytes_consumed) = 0;
		return UTF8_ENCODING_ERROR;
	}

	return code_point;
}

int utf8_encode_code_point(char* data, cy_uint data_size, long code_point, cy_uint* bytes_consumed)
{
	if(!is_valid_utf8_code_point(code_point))
	{
		(*bytes_consumed) = 0;
		return UTF8_ENCODING_ERROR;
	}

	(*bytes_consumed) = whole_bytes_required_for_encoding_utf8_code_point(code_point);
	if((*bytes_consumed) > data_size)
	{
		(*bytes_consumed) = 0;
		return UTF8_TOO_FEW_BYTES;
	}

	if((*bytes_consumed) == 1)
	{
		data[0] = code_point;
		return 1;
	}
	else
		data[0] = (code_point >> ((*bytes_consumed)-1) * 6) | ((-1L) << (8 - (*bytes_consumed)));

	for(cy_uint i = 1; i < (*bytes_consumed); i++)
		data[i] = ((code_point >> ((*bytes_consumed)-1-i) * 6) & 0x3f) | (0x80);

	return 1;
}