#ifndef DPIPE_H
#define DPIPE_H

#include<memory_allocator_interface.h>

/*
** This is a effectively a unix pipe like data type, it can be used to pass any data between a set of readers and writers
** It can be made to work like a pipe if the readers and writers know what they are doing
** It can also be used as stack or a queue for your custom data structures
** It is mainly designed to pipe bytes, and this allows it to be used even with other custom data types
** 
** note:: (according to me,) without external locking this data-structure is useless
** note:: the buffer_capacity must be greater than the largest data_size that you may write/push to dpipe with operation_type = ACCEPT_ALL_OR_NONE
*/

/*
	Important note: you can close the dpipe, once closed no write, push, unread calls can be made. (i.e. data can not be added to the dpipe)
	but the get, pop, read functions will work as usual until the dpipe has data.
*/

typedef struct dpipe dpipe;
struct dpipe
{
	// total bytes currently allocated at buffer
	cy_uint buffer_capacity;

	// the offset of the first byte in the buffer
	// it is also the byte of the front of the dpipe
	cy_uint first_byte;

	// number of bytes occupied in the buffer starting at first_byte
	// remember this is a circular buffer
	cy_uint byte_count;

	// if the dpipe is closed, no bytes can be written/pushed to it
	// BUT you can still read/pop bytes as long as get_bytes_readable_in_dpipe(pipe) > 0
	int is_closed;

	// actual storage of the data is at the buffer
	void* buffer;

	// memory allocator for the buffer of dpipe
	memory_allocator buffer_allocator;
};

typedef enum dpipe_operation_type dpipe_operation_type;
enum dpipe_operation_type
{
	ALL_OR_NONE,
	PARTIAL_ALLOWED,
};

/*
**	For every function here that takes data and data_size as parameters and returns an cy_uint
**	The return value indicates what number of bytes were read/written from the given data,
**	which will always be lesser than or equal to data_size
*/

int initialize_dpipe(dpipe* pipe, cy_uint capacity);
int initialize_dpipe_with_allocator(dpipe* pipe, cy_uint capacity, memory_allocator buffer_allocator);
int initialize_dpipe_with_memory(dpipe* pipe, cy_uint capacity, void* buffer);

// write bytes to dpipe ==> push_back_to_dpipe
cy_uint write_to_dpipe(dpipe* pipe, const void* data, cy_uint data_size, dpipe_operation_type op_type);

// read next bytes from dpipe ==> get_front_of_dpipe and then pop_front_from_dpipe OR peek_from_dpipe and then discard from dpipe
cy_uint read_from_dpipe(dpipe* pipe, void* data, cy_uint data_size, dpipe_operation_type op_type);

// unread given bytes (that we already read) back to the pipe ==> push_front_to_dpipe
// the first byte at data will be read first upon calling read
cy_uint unread_to_dpipe(dpipe* pipe, const void* data, cy_uint data_size, dpipe_operation_type op_type);

// peek data_size number of readabale bytes (from read end), that were suppossed to be read next ==> get_front_of_dpipe
cy_uint peek_from_dpipe(const dpipe* pipe, void* data, cy_uint data_size, dpipe_operation_type op_type);

// discard next data_size number of readable bytes (from read end), that were suppossed to be read next ==> pop_front_from_dpipe
int discard_from_dpipe(dpipe* pipe, cy_uint data_size);

// peeks max consecutive from readabale end of dpipe, these are the bytes that were suppossed to be read next => get_max_consecutive_from_front_of_dpipe
// you may use this function to avoid copy
// after a call to this function you may call discard_from_dpipe on the bytes_available, inorder to pop these bytes
// since this function returns pointer to the internal buffer of the dpipe, you may not modify it
const void* peek_max_consecutive_from_dpipe(const dpipe* pipe, cy_uint* bytes_available);

// use push, get and pop functions only if you know what you are doing
// pop_*_from_dpipe will only discard data_size number of bytes from front or back of dpipe, so it must be used in conjunction with get_*_of_dpipe
cy_uint get_front_of_dpipe(const dpipe* pipe, void* data, cy_uint data_size, dpipe_operation_type op_type);
cy_uint get_back_of_dpipe(const dpipe* pipe, void* data, cy_uint data_size, dpipe_operation_type op_type);
cy_uint push_front_to_dpipe(dpipe* pipe, const void* data, cy_uint data_size, dpipe_operation_type op_type);
cy_uint push_back_to_dpipe(dpipe* pipe, const void* data, cy_uint data_size, dpipe_operation_type op_type);
int pop_front_from_dpipe(dpipe* pipe, cy_uint data_size);
int pop_back_from_dpipe(dpipe* pipe, cy_uint data_size);

// below function will return pointer to the internal buffer of the dpipe and the consecutive bytes count that it points to
// since these pointer point to bytes in internal buffer of dpipe, you may not modify or access out of bound
// these functions are provided for critical code segments, when you may like to avoid copy from dpipe buffer
// you may pop bytes_available after these calls
// use these fucntion only if you know what you are doing
const void* get_max_consecutive_from_front_of_dpipe(const dpipe* pipe, cy_uint* bytes_available);
const void* get_max_consecutive_from_back_of_dpipe(const dpipe* pipe, cy_uint* bytes_available);

// removes all bytes of the dpipe i.e. makes byte_count and first_byte = 0
void remove_all_from_dpipe(dpipe* pipe);

// returns, number of byte you can write or read, without any resize operation
cy_uint get_bytes_writable_in_dpipe(const dpipe* pipe);
cy_uint get_bytes_readable_in_dpipe(const dpipe* pipe);

// returns number of bytes allocated at buffer in the pipe
cy_uint get_capacity_dpipe(const dpipe* pipe);

int is_empty_dpipe(const dpipe* pipe);
int is_full_dpipe(const dpipe* pipe);

// can be used to expand or shrink the dpipe, the new_capacity becomes the new capacity
int resize_dpipe(dpipe* pipe, cy_uint new_capacity);

void close_dpipe(dpipe* pipe);

int is_dpipe_closed(const dpipe* pipe);

void deinitialize_dpipe(dpipe* pipe);

#endif