#include<stream_for_file_descriptor.h>

#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<limits.h>

#include<cutlery_math.h>

// must always be lesser than or equal to SSIZE_MAX
#define MAX_IO_SIZE (SSIZE_MAX >> 4)

/*
	I regret making this change of storing int inside a void*
	I wanted to just save some malloc calls in this stream, avoid such small allocations to avoid fragmentation and also extra computation
	But I strongly believe that void* should generally be atleast larger than an integer on most systems
	Intermediate typecasts to `long long int` is just to keep the compiler happy, regardless of -Wall
*/

static cy_uint read_from_fd(void* stream_context, void* data, cy_uint data_size, int* error)
{
	ssize_t ret = read((long long int)stream_context, data, min(data_size, MAX_IO_SIZE));
	if(ret == -1)
	{
		*error = errno;
		return 0;
	}
	return ret;
}

static cy_uint write_to_fd(void* stream_context, const void* data, cy_uint data_size, int* error)
{
	ssize_t ret = write((long long int)stream_context, data, min(data_size, MAX_IO_SIZE));
	if(ret == -1)
	{
		*error = errno;
		return 0;
	}
	return ret;
}

static void close_stream_context_fd(void* stream_context, int* error)
{
	if(close((long long int)stream_context) == -1)
		*error = errno;
}

static void destroy_stream_context_fd(void* stream_context)
{
	// NOP
}

int initialize_stream_for_fd(stream* strm, int fd)
{
	if(!initialize_stream(strm, (void*)((long long int)fd), read_from_fd, write_to_fd, close_stream_context_fd, destroy_stream_context_fd, NULL, DEFAULT_MAX_UNFLUSHED_BYTES_COUNT))
	{
		// since we never opened this stream_context (i.e. the fd), so we do not close it
		destroy_stream_context_fd((void*)((long long int)fd));
		return 0;
	}

	return 1;
}