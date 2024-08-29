#include<stream_for_dstring.h>

#include<cutlery_stds.h>
#include<cutlery_math.h>

static cy_uint read_from_dstring(void* stream_context, void* data, cy_uint data_size, int* error)
{
	dstring* str = stream_context;
	const char* str_data = get_byte_array_dstring(str);
	cy_uint str_size = get_char_count_dstring(str);

	cy_uint bytes_to_read = min(str_size, data_size);
	memory_move(data, str_data, data_size);

	// now discard those bytes_to_read from the prefix of the str
	if(discard_chars_from_front_dstring(str, bytes_to_read))
		return bytes_to_read;
	(*error) = UNDERLYING_STREAM_ERROR;
	return 0;
}

static cy_uint write_to_dstring(void* stream_context, const void* data, cy_uint data_size, int* error)
{
	if(concatenate_dstring((dstring*)stream_context, &get_dstring_pointing_to(data, data_size)))
		return data_size;
	(*error) = UNDERLYING_STREAM_ERROR;
	return 0;
}

static void close_stream_context_for_dstring(void* stream_context, int* error)
{
	// close is a NOP
}

static void destroy_stream_context_for_dstring(void* stream_context)
{
	// destroy is a NOP
	// you need to manually destroy the dstring
}

int initialize_dstring_stream(stream* strm, dstring* str)
{
	if(str == NULL)
		return 0;

	if(!initialize_stream(strm, str, read_from_dstring, write_to_dstring, close_stream_context_for_dstring, destroy_stream_context_for_dstring, NULL, DEFAULT_MAX_UNFLUSHED_BYTES_COUNT))
	{
		int error = 0;
		close_stream_context_for_dstring(str, &error);
		destroy_stream_context_for_dstring(str);
		return 0;
	}

	return 1;
}