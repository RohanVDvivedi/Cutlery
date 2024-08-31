#include<stream_to_read_until_dstring.h>

#include<stdlib.h>

#include<cutlery_math.h>

static cy_uint read_from_stream_context(void* stream_context, void* data, cy_uint data_size, int* error)
{
	read_until_dstring_stream_context* sc = stream_context;

	cy_uint data_size_res = 0;

	const char* read_until_dstr_data = get_byte_array_dstring(&(sc->read_until_dstr));
	const cy_uint read_until_dstr_size = get_char_count_dstring(&(sc->read_until_dstr));

	while(sc->matched_length < read_until_dstr_size)
	{
		while(get_bytes_readable_in_dpipe(&(sc->cached_bytes)) > sc->matched_length && data_size_res < data_size)
			data_size_res += read_from_dpipe(&(sc->cached_bytes), data + data_size_res, min(data_size - data_size_res, get_bytes_readable_in_dpipe(&(sc->cached_bytes)) - sc->matched_length), PARTIAL_ALLOWED);

		if(data_size_res == data_size)
			break;

		char c;
		int uerror = 0;
		cy_uint byte_read = read_from_stream(sc->underlying_strm, &c, 1, &uerror);
		if(uerror)
		{
			(*error) = UNDERLYING_STREAM_ERROR;
			break;
		}
		if(byte_read == 0)
		{
			(*error) = UNDERLYING_STREAM_FINISHED_BEFORE_READ_UNTIL_DSTRING;
			break;
		}

		write_to_dpipe(&(sc->cached_bytes), &c, 1, PARTIAL_ALLOWED);

		while(1)
		{
			if(c == read_until_dstr_data[sc->matched_length])
			{
				sc->matched_length++;
				break;
			}
			else if(sc->matched_length == 0)
				break;
			else
				sc->matched_length = sc->read_until_dstr_spml[sc->matched_length];
		}
	}

	return data_size_res;
}

static void close_stream_context(void* stream_context, int* error)
{
	// NOP
}

static void destroy_stream_context(void* stream_context)
{
	read_until_dstring_stream_context* sc = stream_context;
	free(sc->read_until_dstr_spml);
	deinit_dstring(&(sc->read_until_dstr));
	deinitialize_dpipe(&(sc->cached_bytes));
	free(sc);
}

static read_until_dstring_stream_context*  new_reading_until_dstring_stream_context(stream* underlying_strm, const dstring* read_until_dstr, cy_uint* read_until_dstr_spml)
{
	read_until_dstring_stream_context* sc = malloc(sizeof(read_until_dstring_stream_context));
	if(sc == NULL)
		return NULL;
	sc->underlying_strm = underlying_strm;
	sc->matched_length = 0;
	if(!initialize_dpipe(&(sc->cached_bytes), get_char_count_dstring(read_until_dstr)))
	{
		free(sc);
		return NULL;
	}
	if(!init_copy_dstring(&(sc->read_until_dstr), read_until_dstr))
	{
		deinitialize_dpipe(&(sc->cached_bytes));
		free(sc);
		return NULL;
	}
	sc->read_until_dstr_spml = read_until_dstr_spml;
	return sc;
}

int initialize_stream_for_reading_until_dstring(stream* strm, stream* underlying_strm, const dstring* read_until_dstr)
{
	// create suffix prefix match lengths for read_until_dtr
	cy_uint* read_until_dstr_spml = malloc(sizeof(cy_uint) * (get_char_count_dstring(read_until_dstr) + 1));
	if(read_until_dstr_spml == NULL)
		return 0;
	get_prefix_suffix_match_lengths(read_until_dstr, read_until_dstr_spml);

	// create stream context
	read_until_dstring_stream_context* sc = new_reading_until_dstring_stream_context(underlying_strm, read_until_dstr, read_until_dstr_spml);
	if(sc == NULL)
	{
		free(read_until_dstr_spml);
		return 0;
	}

	// initialize stream
	if(!initialize_stream(strm, sc, read_from_stream_context, NULL, close_stream_context, destroy_stream_context, NULL, 0))
	{
		int error = 0;
		close_stream_context(sc, &error);
		destroy_stream_context(sc);
		return 0;
	}

	return 1;
}

int initialize_stream_for_reading_until_dstring2(stream* strm, stream* underlying_strm, const dstring* read_until_dstr, const cy_uint* read_until_dstr_spml)
{
	// create a copy suffix prefix match lengths (read_until_dst_spml)
	cy_uint* read_until_dstr_spml_l = malloc(sizeof(cy_uint) * (get_char_count_dstring(read_until_dstr) + 1));
	if(read_until_dstr_spml_l == NULL)
		return 0;
	for(cy_uint i = 0; i <= get_char_count_dstring(read_until_dstr); i++)
		read_until_dstr_spml_l[i] = read_until_dstr_spml[i];

	// create stream context
	read_until_dstring_stream_context* sc = new_reading_until_dstring_stream_context(underlying_strm, read_until_dstr, read_until_dstr_spml_l);
	if(sc == NULL)
	{
		free(read_until_dstr_spml_l);
		return 0;
	}

	// initialize stream
	if(!initialize_stream(strm, sc, read_from_stream_context, NULL, close_stream_context, destroy_stream_context, NULL, 0))
	{
		int error = 0;
		close_stream_context(sc, &error);
		destroy_stream_context(sc);
		return 0;
	}

	return 1;
}