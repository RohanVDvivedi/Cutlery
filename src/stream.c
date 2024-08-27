#include<stream.h>

#include<cutlery_math.h>

int initialize_stream(stream* strm,
						void* stream_context,
						cy_uint (*read_from_stream_context)(void* stream_context, void* data, cy_uint data_size, int* error),
						cy_uint (*write_to_stream_context)(void* stream_context, const void* data, cy_uint data_size, int* error),
						void (*close_stream_context)(void* stream_context, int* error),
						void (*destroy_stream_context)(void* stream_context),
						void (*post_flush_callback_stream_context)(void* stream_context, int* error),
						cy_uint max_unflushed_bytes_count)
{
	strm->stream_context = stream_context;
	if(!initialize_dpipe(&(strm->unread_data), 0))
		return 0;
	if(!initialize_dpipe(&(strm->unflushed_data), 0))
	{
		deinitialize_dpipe(&(strm->unread_data));
		return 0;
	}
	strm->max_unflushed_bytes_count = max_unflushed_bytes_count;
	strm->read_from_stream_context = read_from_stream_context;
	strm->end_of_stream_received = 0;
	strm->write_to_stream_context = write_to_stream_context;
	strm->close_stream_context = close_stream_context;
	strm->destroy_stream_context = destroy_stream_context;
	strm->post_flush_callback_stream_context = post_flush_callback_stream_context;
	strm->last_error = 0;
	return 1;
}

int initialize_stream_with_initialized_dpipes(
						stream* strm,
						void* stream_context,
						dpipe unread_data_dpipe,
						dpipe unflushed_data_dpipe,
						cy_uint (*read_from_stream_context)(void* stream_context, void* data, cy_uint data_size, int* error),
						cy_uint (*write_to_stream_context)(void* stream_context, const void* data, cy_uint data_size, int* error),
						void (*close_stream_context)(void* stream_context, int* error),
						void (*destroy_stream_context)(void* stream_context),
						void (*post_flush_callback_stream_context)(void* stream_context, int* error),
						cy_uint max_unflushed_bytes_count
					)
{
	strm->stream_context = stream_context;
	strm->unread_data = unread_data_dpipe;
	strm->unflushed_data = unread_data_dpipe;
	strm->max_unflushed_bytes_count = max_unflushed_bytes_count;
	strm->read_from_stream_context = read_from_stream_context;
	strm->end_of_stream_received = 0;
	strm->write_to_stream_context = write_to_stream_context;
	strm->close_stream_context = close_stream_context;
	strm->destroy_stream_context = destroy_stream_context;
	strm->post_flush_callback_stream_context = post_flush_callback_stream_context;
	strm->last_error = 0;
	return 1;
}

int is_readable_stream(stream* strm)
{
	return strm->read_from_stream_context != NULL;
}

int is_writable_stream(stream* strm)
{
	return strm->write_to_stream_context != NULL;
}

cy_uint read_from_stream(stream* strm, void* data, cy_uint data_size, int* error)
{
	// intialize error to 0
	(*error) = 0;

	if(strm->read_from_stream_context == NULL)
	{
		(*error) = INACCESSIBLE_STREAM_FUNCTIONALITY;
		return 0;
	}

	// the buffer must have some space for the new data to be read into
	// for read_from_stream data_size == 0, is an invalid parameter, but we are handling this here so that your code may not need to panic
	// here we return 0, but this may not be an end_of_stream
	if(data_size == 0)
		return 0;

	cy_uint bytes_read = 0;

	// if the unread_data pipe has remaining bytes then read data from there
	if(!is_empty_dpipe(&(strm->unread_data)))
	{
		bytes_read = read_from_dpipe(&(strm->unread_data), data, data_size, PARTIAL_ALLOWED);

		// shrink unread_data dpipe if it is larger than 2 times what it is required
		if((get_capacity_dpipe(&(strm->unread_data)) / 2) > get_bytes_readable_in_dpipe(&(strm->unread_data)))
			resize_dpipe(&(strm->unread_data), get_bytes_readable_in_dpipe(&(strm->unread_data))); // we do not care if this call fails

		// there can be no failure, reading data from unread_data
		return bytes_read;
	}

	// if there was a end_of_stream_received, then we can not issue any more read calls
	if(strm->end_of_stream_received)
		return 0;

	// if you have reached here, then the unread_data is empty and end_of_stream_received == 0

	// if data_size to be read is lesser than 128 bytes then, we attempt to make a read for a kilo byte from the stream context and then cache remaining bytes
	if(data_size < 128)
	{
		// the below macro must be greater than or equal to 128
		#define data_cache_read_capacity MAX_UNREAD_BYTES_COUNT

		// if unread data does not have enough space, then expand it
		if(get_bytes_writable_in_dpipe(&(strm->unread_data)) < data_cache_read_capacity &&
			!resize_dpipe(&(strm->unread_data), get_bytes_readable_in_dpipe(&(strm->unread_data)) + data_cache_read_capacity)) // here we already know that the readable bytes in unread_data dpipe is 0
		{
			// this is a case when there is not enough memeory in the unread_data buffer and the expansion failed
			// so we go to FALLBACK
			goto FALLBACK;
		}

		// read a kilo byte worth of data, from the stream context and cache it
		char data_cache_read[data_cache_read_capacity];
		cy_uint data_cache_read_size = strm->read_from_stream_context(strm->stream_context, data_cache_read, data_cache_read_capacity, error);

		// if it is EOF, then set the end_of_stream_received flag
		if(data_cache_read_size == 0 && (!(*error)))
			strm->end_of_stream_received = 1;

		// move the front of cached bytes to output buffer
		// these many bytes from the cached data can be moved to the output buffer
		bytes_read = min(data_size, data_cache_read_size);
		memory_move(data, data_cache_read, bytes_read);

		// if there are still bytes left in the cached buffer, then move those bytes to the unread buffer
		if(data_cache_read_size > bytes_read)
		{
			// write the remaining bytes from data_cache_read to unread_data
			void* cache_bytes_to_write = data_cache_read + bytes_read;
			cy_uint cache_bytes_to_write_size = data_cache_read_size - bytes_read;

			write_to_dpipe(&(strm->unread_data), cache_bytes_to_write, cache_bytes_to_write_size, ALL_OR_NONE);
		}

		// if an error occurred, then register it as the last one
		if(*error)
			strm->last_error = (*error);

		// shrink unread_data dpipe if it is larger than 2 times what it is required
		// this may happen, if the read_from_stream_context returned far lesser than what we expected
		if((get_capacity_dpipe(&(strm->unread_data)) / 2) > get_bytes_readable_in_dpipe(&(strm->unread_data)))
			resize_dpipe(&(strm->unread_data), get_bytes_readable_in_dpipe(&(strm->unread_data))); // we do not care if this call fails

		return bytes_read;
	}

	FALLBACK:;
	// make a read call to the stream context, and return data directly
	{
		bytes_read = strm->read_from_stream_context(strm->stream_context, data, data_size, error);

		// if it is EOF, then set the end_of_stream_received flag
		if(bytes_read == 0 && (!(*error)))
			strm->end_of_stream_received = 1;

		// if an error occurred, then register it as the last one
		if(*error)
			strm->last_error = (*error);

		return bytes_read;
	}
}

void unread_from_stream(stream* strm, const void* data, cy_uint data_size, int* error)
{
	// intialize error to 0
	(*error) = 0;

	if(strm->read_from_stream_context == NULL)
	{
		(*error) = INACCESSIBLE_STREAM_FUNCTIONALITY;
		return ;
	}

	// calculate the required_unread_data_capacity, if it overflows then fail
	if(will_unsigned_sum_overflow(cy_uint, get_bytes_readable_in_dpipe(&(strm->unread_data)), data_size))
	{
		(*error) = FAILED_TO_APPEND_TO_UNREAD_BUFFER_IN_STREAM;
		return ;
	}
	cy_uint required_unread_data_capacity = get_bytes_readable_in_dpipe(&(strm->unread_data)) + data_size;

	// stream allows atmost of MAX_UNREAD_BYTES_COUNT bytes of unread data to exist in unread buffers
	if(required_unread_data_capacity > MAX_UNREAD_BYTES_COUNT)
	{
		(*error) = FAILED_TO_APPEND_TO_UNREAD_BUFFER_IN_STREAM;
		return ;
	}

	if(get_bytes_writable_in_dpipe(&(strm->unread_data)) < data_size)
	{
		// first attempt to expand by almost doubling unread_data capacity
		cy_uint attempted_unread_data_capacity = MAX_UNREAD_BYTES_COUNT;
		if(!will_unsigned_mul_overflow(cy_uint, required_unread_data_capacity, 2))
			attempted_unread_data_capacity = min(attempted_unread_data_capacity, required_unread_data_capacity * 2);

		// alocate for double the required capacity OR allocate exact
		if(!resize_dpipe(&(strm->unread_data), attempted_unread_data_capacity) && !resize_dpipe(&(strm->unread_data), required_unread_data_capacity))
		{
			// this is a case when there is not enough memory in the unread_data buffer and the expansion failed
			// so we return an error
			(*error) = FAILED_TO_APPEND_TO_UNREAD_BUFFER_IN_STREAM;
			return ;
		}
	}

	// just push the data to unread_data pipe, this must succeed
	unread_to_dpipe(&(strm->unread_data), data, data_size, ALL_OR_NONE);
}

// INTERNAL FUNCTION ONLY - to be only used by write and flush_all functions
// this function will be used to write unflushed bytes, i.e. to flush them, i.e. to perfrom actual write to stream context
// return value suggests the number of bytes flushed, regardless of the error
static cy_uint write_flushable_bytes(stream* strm, const void* data, cy_uint data_size, int* error)
{
	// return value
	cy_uint bytes_written = 0;
	
	// clear error
	(*error) = 0;

	while(bytes_written < data_size && (*error) == 0)
		bytes_written += strm->write_to_stream_context(strm->stream_context, data + bytes_written, data_size - bytes_written, error);

	return bytes_written;
}

// INTERNAL FUNCTION ONLY - to be only used by write and flush_all functions
// it only flushes all unflushed bytes from stream unflushed_data pipe of the stream
// it may fail on an error, in which case it will return 0 with (*error) being set to respective value
static void flush_all_unflushed_data(stream* strm, int* error)
{
	(*error) = 0;

	while(get_bytes_readable_in_dpipe(&(strm->unflushed_data)) > 0 && (*error) == 0)
	{
		cy_uint data_size;
		const void* data = peek_max_consecutive_from_dpipe(&(strm->unflushed_data), &data_size);

		cy_uint bytes_flushed = write_flushable_bytes(strm, data, data_size, error);

		discard_from_dpipe(&(strm->unflushed_data), bytes_flushed);
	}
}

cy_uint write_to_stream(stream* strm, const void* data, cy_uint data_size, int* error)
{
	// intialize error to 0
	(*error) = 0;

	if(strm->write_to_stream_context == NULL)
	{
		(*error) = INACCESSIBLE_STREAM_FUNCTIONALITY;
		return 0;
	}

	// data_size == 0, nothing to write, then make this function a no-op
	if(data_size == 0)
		return 0;

	// calculate the required_unflushed_data_capacity, if it overflows goto fallback
	if(will_unsigned_sum_overflow(cy_uint, get_bytes_readable_in_dpipe(&(strm->unflushed_data)), data_size))
		goto FALLBACK;
	cy_uint required_unflushed_data_capacity = get_bytes_readable_in_dpipe(&(strm->unflushed_data)) + data_size;

	// if the required_unflushed_data_capacity > strm->max_unflushed_bytes_count, then we can not push the data to unflushed_data, so goto fallback
	if(required_unflushed_data_capacity > strm->max_unflushed_bytes_count)
		goto FALLBACK;

	if(get_bytes_writable_in_dpipe(&(strm->unflushed_data)) < data_size)
	{
		// first attempt to expand by almost doubling unflushed_data capacity
		cy_uint attempted_unflushed_data_capacity = strm->max_unflushed_bytes_count;
		if(!will_unsigned_mul_overflow(cy_uint, required_unflushed_data_capacity, 2))
			attempted_unflushed_data_capacity = min(attempted_unflushed_data_capacity, required_unflushed_data_capacity * 2);

		// alocate for double the required capacity OR allocate exact
		if(!resize_dpipe(&(strm->unflushed_data), attempted_unflushed_data_capacity) && !resize_dpipe(&(strm->unflushed_data), required_unflushed_data_capacity))
			goto FALLBACK;
	}

	//DEFAULT_HAPPY_CASE:; // if we come here, then we are sure that we can push the data to unflushed_data
	{
		cy_uint bytes_written = write_to_dpipe(&(strm->unflushed_data), data, data_size, ALL_OR_NONE);

		return bytes_written;
	}

	// else, we fallback to flush not just the unflushed_data, but also the new data that has arrived
	FALLBACK:;
	{
		// flush all of the unflushed_data first
		flush_all_unflushed_data(strm, error);

		if(*error)
		{
			strm->last_error = (*error);
			return 0; // no bytes from data written, hence we return a 0 here
		}

		// then flush the new arriving data
		cy_uint bytes_written = write_flushable_bytes(strm, data, data_size, error);

		if(*error)
		{
			strm->last_error = (*error);
			return bytes_written;
		}

		// no error, only then we may call post_flush_callback_stream_context
		if(strm->post_flush_callback_stream_context != NULL)
		{
			strm->post_flush_callback_stream_context(strm->stream_context, error);
			if(*error)
				strm->last_error = (*error);
		}

		return bytes_written;
	}
}

void flush_all_from_stream(stream* strm, int* error)
{
	// intialize error to 0
	(*error) = 0;

	if(strm->write_to_stream_context == NULL)
	{
		(*error) = INACCESSIBLE_STREAM_FUNCTIONALITY;
		return ;
	}

	// flush all of the unflushed_data
	flush_all_unflushed_data(strm, error);

	resize_dpipe(&(strm->unflushed_data), get_bytes_readable_in_dpipe(&(strm->unflushed_data))); // we do not care if this call fails

	if(*error)
	{
		strm->last_error = (*error);
		return;
	}

	// no error, only then we may call post_flush_callback_stream_context
	if(strm->post_flush_callback_stream_context != NULL)
	{
		strm->post_flush_callback_stream_context(strm->stream_context, error);
		if(*error)
			strm->last_error = (*error);
	}
}

void close_stream(stream* strm, int* error)
{
	// intialize error to 0
	(*error) = 0;

	strm->close_stream_context(strm->stream_context, error);

	// if an error occurred, then register it as the last one
	if(*error)
		strm->last_error = (*error);
}

void deinitialize_stream(stream* strm)
{
	deinitialize_dpipe(&(strm->unread_data));
	deinitialize_dpipe(&(strm->unflushed_data));
	strm->destroy_stream_context(strm->stream_context);
	*strm = (stream){};
}