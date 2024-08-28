#include<stream_util.h>

#include<dstring.h>
#include<cutlery_stds.h>
#include<cutlery_math.h>

cy_uint write_to_stream_formatted(stream* ws, int* error, const char* cstr_format, ...)
{
	dstring str;
	if(!init_empty_dstring(&str, 0))
	{
		(*error) = ALLOCATION_FAILURE_IN_STREAM;
		return 0;
	}

	va_list var_args;
	va_start(var_args, cstr_format);

	int dstringify_success = vsnprintf_dstring(&str, cstr_format, var_args);
	if(!dstringify_success)
	{
		(*error) = ALLOCATION_FAILURE_IN_STREAM;
		deinit_dstring(&str);
		return 0;
	}

	va_end(var_args);

	cy_uint bytes_written = write_dstring_to_stream(ws, &str, error);

	deinit_dstring(&str);

	return bytes_written;
}

cy_uint read_unsigned_long_long_int_from_stream(stream* rs, unsigned int radix, unsigned long long int* data, int* error)
{
	if((radix != BINARY) && (radix != OCTAL) && (radix != DECIMAL) && (radix != HEXADECIMAL))
	{
		(*error) = -1;
		return 0;
	}

	static cy_uint max_bytes_to_read_BINARY = 0;
	static cy_uint max_bytes_to_read_OCTAL = 0;
	static cy_uint max_bytes_to_read_DECIMAL = 0;
	static cy_uint max_bytes_to_read_HEXADECIMAL = 0;
	if(max_bytes_to_read_BINARY == 0) // to initialize these static variables only once
	{
		max_bytes_to_read_BINARY = get_digits_required_to_represent_unsigned_long_long_int(2);
		max_bytes_to_read_OCTAL = get_digits_required_to_represent_unsigned_long_long_int(8);
		max_bytes_to_read_DECIMAL = get_digits_required_to_represent_unsigned_long_long_int(10);
		max_bytes_to_read_HEXADECIMAL = get_digits_required_to_represent_unsigned_long_long_int(16);
	}

	cy_uint max_bytes_to_read = 0;
	switch(radix)
	{
		case BINARY :
		{
			max_bytes_to_read = max_bytes_to_read_BINARY;
			break;
		}
		case OCTAL :
		{
			max_bytes_to_read = max_bytes_to_read_OCTAL;
			break;
		}
		case DECIMAL :
		{
			max_bytes_to_read = max_bytes_to_read_DECIMAL;
			break;
		}
		case HEXADECIMAL :
		{
			max_bytes_to_read = max_bytes_to_read_HEXADECIMAL;
			break;
		}
	}

	(*data) = 0;

	cy_uint bytes_read = 0;
	while(bytes_read < max_bytes_to_read)
	{
		char byte;
		cy_uint byte_read = read_from_stream(rs, &byte, 1, error);

		if(byte_read == 0 || (*error))
			break;

		unsigned int digit = get_digit_from_char(byte, radix);

		if(digit != -1)
		{
			bytes_read++;
			(*data) *= radix;
			(*data) += digit;
		}
		else
		{
			unread_from_stream(rs, &byte, 1, error);
			if((*error))
				bytes_read++;
			break;
		}
	}

	return bytes_read;
}

cy_uint skip_whitespaces_from_stream(stream* rs, cy_uint max_whitespaces_to_skip, int* error)
{
	cy_uint bytes_read = 0;
	while(bytes_read < max_whitespaces_to_skip)
	{
		char byte;
		cy_uint byte_read = read_from_stream(rs, &byte, 1, error);

		if(byte_read == 0 || (*error))
			break;

		if(is_whitespace_char(byte))
			bytes_read++;
		else
		{
			unread_from_stream(rs, &byte, 1, error);
			if((*error))
				bytes_read++;
			break;
		}
	}

	return bytes_read;
}

cy_uint skip_dstring_from_stream(stream* rs, const dstring* str_to_skip, int* error)
{
	const char* str_data = get_byte_array_dstring(str_to_skip);
	const cy_uint str_size = get_char_count_dstring(str_to_skip);

	cy_uint match_size = 0;

	for(; match_size < str_size; match_size++)
	{
		char byte;
		cy_uint byte_read = read_from_stream(rs, &byte, 1, error);

		if(byte_read == 0 || (*error))
			break;

		if(byte != str_data[match_size])
		{
			unread_from_stream(rs, &byte, 1, error);
			if((*error))
				return match_size + 1;
			break;
		}
	}

	// upon mismatch unread all
	if(match_size < str_size)
	{
		unread_from_stream(rs, str_data, match_size, error);
		if((*error))
			return match_size;
		return 0;
	}

	return str_size; // same as match_size
}

dstring read_until_dstring_from_stream(stream* rs, const dstring* until_str, const cy_uint* prefix_suffix_match_lengths_for_until_str, cy_uint max_bytes_to_read, int* error)
{
	const char* until_str_data = get_byte_array_dstring(until_str);
	const cy_uint until_str_size = get_char_count_dstring(until_str);

	dstring res;
	if(!init_empty_dstring(&res, 0))
	{
		(*error) = ALLOCATION_FAILURE_IN_STREAM;
		return res;
	}

	// the limit set too low to even include until_str_size
	if(until_str_size > max_bytes_to_read)
		return res;

	// how many characters of until_str matches with suffix of res
	cy_uint match_length = 0;

	while(match_length < until_str_size && get_char_count_dstring(&res) < max_bytes_to_read)
	{
		char byte;
		cy_uint byte_read = read_from_stream(rs, &byte, 1, error);

		if(byte_read == 0 || (*error))
			break;

		// append the character we just read to the res
		if(!concatenate_char(&res, byte))
		{
			(*error) = ALLOCATION_FAILURE_IN_STREAM;
			deinit_dstring(&res);
			return res;
		}

		// evaluate the new match length
		while(1)
		{
			if(byte == until_str_data[match_length])
			{
				match_length++;
				break;
			}
			else
			{
				if(match_length == 0)
					break;
				else
					match_length = prefix_suffix_match_lengths_for_until_str[match_length];
			}
		}
	}

	if((*error))
	{
		deinit_dstring(&res);
		return res;
	}

	// if we couldn't match with until_str
	if(match_length < until_str_size)
	{
		unread_dstring_from_stream(rs, &res, error);
		if(!(*error))
		{
			make_dstring_empty(&res);
			shrink_dstring(&res);
		}
		else
			deinit_dstring(&res);
	}

	return res;
}

dstring read_until_any_end_chars_from_stream(stream* rs, int (*is_end_char)(int is_end_of_stream, char c, const void* cntxt), const void* cntxt, int* last_byte, cy_uint max_bytes_to_read, int* error)
{
	dstring res;
	if(!init_empty_dstring(&res, 0))
	{
		(*error) = ALLOCATION_FAILURE_IN_STREAM;
		return res;
	}

	int end_encountered = 0;

	(*last_byte) = 257;

	while(!end_encountered)
	{
		char byte;
		cy_uint byte_read = read_from_stream(rs, &byte, 1, error);

		// if not an error, then set last_byte
		if(!(*error))
		{
			if(byte_read == 0)
				(*last_byte) = 256;
			else
				(*last_byte) = byte;
		}

		// if the current byte read is an end character, then set end_encountered
		if((!(*error)) && is_end_char(byte_read == 0, byte, cntxt))
			end_encountered = 1;

		if(byte_read == 0 || (*error))
			break;

		// append the character we just read to the res, only if number of bytes in res
		if(get_char_count_dstring(&res) < max_bytes_to_read)
		{
			if(!concatenate_char(&res, byte))
			{
				(*error) = ALLOCATION_FAILURE_IN_STREAM;
				deinit_dstring(&res);
				return res;
			}
		}
		else
		{
			unread_from_stream(rs, &byte, 1, error);
			// since we were not suppossed to read the last byte (due to size constraints on res)
			// any end_encountered is nullified
			end_encountered = 0;
			break;
		}
	}

	if((*error))
	{
		deinit_dstring(&res);
		return res;
	}

	if(!end_encountered)
	{
		unread_dstring_from_stream(rs, &res, error);
		if(!(*error))
		{
			make_dstring_empty(&res);
			shrink_dstring(&res);
			(*last_byte) = 257;
		}
		else
			deinit_dstring(&res);
	}

	return res;
}

void unread_dstring_from_stream(stream* rs, const dstring* str, int* error)
{
	return unread_from_stream(rs, get_byte_array_dstring(str), get_char_count_dstring(str), error);
}

cy_uint write_dstring_to_stream(stream* ws, const dstring* str, int* error)
{
	return write_to_stream(ws, get_byte_array_dstring(str), get_char_count_dstring(str), error);
}