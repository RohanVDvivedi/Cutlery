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
	unsigned int buffer_capacity;

	unsigned int first_byte_index;

	unsigned int bytes_occupied;

	int read_closed:1;

	int write_closed:1;

	const void* buffer;

	memory_allocator mem_allocator;
};

typedef enum dstream_operation_type dstream_operation_type;
enum dstream_operation_type
{
	ACCEPT_ALL_OR_NONE,
	ACCEPT_PARTIAL,
};

#endif