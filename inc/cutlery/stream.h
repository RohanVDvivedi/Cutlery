#ifndef STREAM_H
#define STREAM_H

#include<cutlery/cutlery_stds.h>
#include<cutlery/dpipe.h>

/*
	stream provides you with a wrapper over your io ports and devices
	it will also club smaller reads into larger reads, and smaller writes into larger writes
*/

typedef struct stream stream;
struct stream
{
	void* stream_context;

	// this pipe stores data that was unread_to_stream
	dpipe unread_data;

	// this is the maximum compile time constant number of bytes that can exist at any moment in the unread_data dpipe
	// this value must be >= 200, and atmost 2048 for optimum performance
	// please ensure that this value fits the cy_uint integer type
	#define MAX_UNREAD_BYTES_COUNT 1024

	// out of these MAX_UNREAD_BYTES_COUNT unread_data bytes,
	// and you may only accumulate no more than (MAX_UNREAD_BYTES_COUNT/2) as bytes to unread
	// the rest is used for bulk reading

	// this pipe stores all data that has been written to the stream
	// but has not been flushed yet, using the write_to_stream_context function call
	dpipe unflushed_data;

	// this is the threshold of maximum unflushed_data that can exist in the unflushed_data pipe
	// beyond which a flush will be called, from the write call
	cy_uint max_unflushed_bytes_count;
	#define DEFAULT_MAX_UNFLUSHED_BYTES_COUNT 4096

	// returns bytes read from data, (atmost data_size number of bytes will be touched i.e. returned)
	// on error return 0 bytes read and set the value of (non-zero) error
	// reading 0 bytes, when data_size > 0 and strm->error == 0, then this implies an end_of_stream EOF
	// no read calls will be made after an EOF is received
	cy_uint (*read_from_stream_context)(void* stream_context, void* data, cy_uint data_size, int* error);

	// if this flag is set then, it implies that the last read call returned 0, i.e. EOF was received we call it end of stream
	// after this no read_from_stream_context calls will be made, only unread data will be returned
	// this flag is for internal use only
	int end_of_stream_received;

	// returns bytes written from data (that consists of data_size number of bytes to be written)
	// on error return 0 bytes read and set the value of (non-zero) error
	cy_uint (*write_to_stream_context)(void* stream_context, const void* data, cy_uint data_size, int* error);

	// closes the stream context
	void (*close_stream_context)(void* stream_context, int* error);

	// this function will be called to destroy the stream_context
	void (*destroy_stream_context)(void* stream_context);

	// An optional callback function, that gets called after a flush is performed on the stream
	void (*post_flush_callback_stream_context)(void* stream_context, int* error);

	// the error that was returned after the last call to read/write or close stream functions
	int last_error;
};

// this function may fail only of the underlying dpipe-s could not be initialized
int initialize_stream(
						stream* strm,
						void* stream_context,
						cy_uint (*read_from_stream_context)(void* stream_context, void* data, cy_uint data_size, int* error),
						cy_uint (*write_to_stream_context)(void* stream_context, const void* data, cy_uint data_size, int* error),
						void (*close_stream_context)(void* stream_context, int* error),
						void (*destroy_stream_context)(void* stream_context),
						void (*post_flush_callback_stream_context)(void* stream_context, int* error),
						cy_uint max_unflushed_bytes_count
					);

// initializes stream, using the above params, but with the dpipes provided
// after a success from this function, the ownership of the dpipes unread_data_dpipe and unflushed_data_dpipe are transferred and you no longer own them
// i.e. you may no longer call any functions on them
// and they will be destroyed when this stream is deinitialized
// this is done to allow easy initialization of stream with complex memory allocation strategies, as you will initialize the necessary dpipes, you can do this as you please
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
					);

int is_readable_stream(stream* strm);

int is_writable_stream(stream* strm);

// a return value of 0 from this function implies end of input/socket closed from the other end
// after that no more calls should be made and you must exit your read loop
cy_uint read_from_stream(stream* rs, void* data, cy_uint data_size, int* error);

// unreads to stream's internal buffer, the contents can be re-read on a read_from_stream call
// an error is thrown, then the unread call has failed
// you must use this function very wisely, and only unread bytes that you actually read, and unreads must never accumulate more than (MAX_UNREAD_BYTES_COUNT/2) bytes
// if you every need to totally accumulate more than (MAX_UNREAD_BYTES_COUNT/2) bytes then use an external buffer
void unread_from_stream(stream* rs, const void* data, cy_uint data_size, int* error);

// return value of this function suggests, the number of bytes from data_size, that were either
// pushed to unflushed_data pipe, or actually wrriten to the stream_context (this happens if the incomming bytes and the unflushed_data bytes sum to more than max_unflushed_bytes_count)
cy_uint write_to_stream(stream* ws, const void* data, cy_uint data_size, int* error);

// flushes all the written data using the write_to_stream_context function call on the underlying stream context
void flush_all_from_stream(stream* ws, int* error);

// to close the stream
// you must close the stream even on an error, i.e. if it was initialized then you must close it
void close_stream(stream* strm, int* error);

void deinitialize_stream(stream* strm);

// use UNDERLYING_STREAM_ERROR as error code when write_to_stream_context has failed,
// due to failure to write to an underlying stream (probably because of memory allocation failure)
#define UNDERLYING_STREAM_ERROR (SIGNED_MIN_VALUE_OF(int)+0)

// use this error in your custom stream functionality, when a malloc (or any other memory allocation function) fails in your stream implementation
#define ALLOCATION_FAILURE_IN_STREAM (SIGNED_MIN_VALUE_OF(int)+1)

// below errors are returned when an append to stream's internal dpipe buffers fail, below errors are non-recoverable
#define FAILED_TO_APPEND_TO_UNFLUSHED_BUFFER_IN_STREAM (SIGNED_MIN_VALUE_OF(int)+2)
#define FAILED_TO_APPEND_TO_UNREAD_BUFFER_IN_STREAM (SIGNED_MIN_VALUE_OF(int)+3)

// below errors are returned when you read or unread to a non readable stream OR when you write of flush an only readable stream
#define INACCESSIBLE_STREAM_FUNCTIONALITY (SIGNED_MIN_VALUE_OF(int)+4)

#endif