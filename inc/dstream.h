#ifndef DSTREAM_H
#define DSTREAM_H

#include<memory_allocator_interface.h>

/*
** This is a stream data type, it can be used to pass any data between a set of readers and writers
** It can also be used as stack or a queue for your custom data structures
** It is mainly designed to stream bytes, and this allows it to be used even with other custom data types
** 
** note:: (according to me,) without external locking this data-structure is useless
** note:: the buffer_capacity must be greater than the largest data_size that you may write/push to dstream with operation_type = ACCEPT_ALL_OR_NONE
*/

typedef struct dstream dstream;
struct dstream
{
	// total bytes currently allocated at buffer
	unsigned int buffer_capacity;

	// the offset of the first byte in the buffer
	// it is also the byte of the front of the dstream
	unsigned int first_byte;

	// number of bytes occupied in the buffer starting at first_byte
	// remember this is a circular buffer
	unsigned int byte_count;

	// if the dstream is read_closed, no bytes can be read/popped from it
	int read_closed:1;

	// if the dstream is write_closed, no bytes can be written/pushed to it
	int write_closed:1;

	// actual storage of the data is at the buffer
	void* buffer;

	// memory allocator for the buffer of dstream
	memory_allocator mem_allocator;
};

typedef enum dstream_operation_type dstream_operation_type;
enum dstream_operation_type
{
	ACCEPT_ALL_OR_NONE,
	ACCEPT_PARTIAL,
};

#endif