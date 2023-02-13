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
	unsigned int buffer_capacity;

	// the offset of the first byte in the buffer
	// it is also the byte of the front of the dpipe
	unsigned int first_byte;

	// number of bytes occupied in the buffer starting at first_byte
	// remember this is a circular buffer
	unsigned int byte_count;

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
**	For every function here that takes data and data_size as parameters and returns an unsigned int
**	The return value indicates what number of bytes were read/written from the given data,
**	which will always be lesser than or equal to data_size
*/

void initialize_dpipe(dpipe* pipe, unsigned int capacity);
void initialize_dpipe_with_allocator(dpipe* pipe, unsigned int capacity, memory_allocator buffer_allocator);
void initialize_dpipe_with_memory(dpipe* pipe, unsigned int capacity, void* buffer);

// write bytes to dpipe ==> push_back_to_dpipe
unsigned int write_to_dpipe(dpipe* pipe, const void* data, unsigned int data_size, dpipe_operation_type op_type);

// read next bytes from dpipe ==> get_front_of_dpipe and then pop_front_from_dpipe
unsigned int read_from_dpipe(dpipe* pipe, void* data, unsigned int data_size, dpipe_operation_type op_type);

// unread given bytes (that we already read) back to the pipe ==> push_front_to_dpipe
// the first byte at data will be read first upon calling read
unsigned int unread_to_dpipe(dpipe* pipe, const void* data, unsigned int data_size, dpipe_operation_type op_type);

// peek data_size number of readabale bytes (from read end), that were suppossed to be read next ==> get_front_of_dpipe
unsigned int peek_from_dpipe(const dpipe* pipe, void* data, unsigned int data_size, dpipe_operation_type op_type);

// discard next data_size number of readable bytes (from read end), that were suppossed to be read next ==> pop_front_from_dpipe
int discard_from_dpipe(dpipe* pipe, unsigned int data_size);

// use push, pop and get functions only if you know what you are doing
// pop_*_from_dpipe will only discard data_size number of bytes from front or back of dpipe, so it must be used in conjunction with get_*_of_dpipe
unsigned int get_front_of_dpipe(const dpipe* pipe, void* data, unsigned int data_size, dpipe_operation_type op_type);
unsigned int get_back_of_dpipe(const dpipe* pipe, void* data, unsigned int data_size, dpipe_operation_type op_type);
unsigned int push_front_to_dpipe(dpipe* pipe, const void* data, unsigned int data_size, dpipe_operation_type op_type);
unsigned int push_back_to_dpipe(dpipe* pipe, const void* data, unsigned int data_size, dpipe_operation_type op_type);
int pop_front_from_dpipe(dpipe* pipe, unsigned int data_size);
int pop_back_from_dpipe(dpipe* pipe, unsigned int data_size);

// removes all bytes of the dpipe i.e. makes byte_count and first_byte = 0
void remove_all_from_dpipe(dpipe* pipe);

// returns, number of byte you can write or read, without any resize operation
unsigned int get_bytes_writable_in_dpipe(const dpipe* pipe);
unsigned int get_bytes_readable_in_dpipe(const dpipe* pipe);

// returns number of bytes allocated at buffer in the pipe
unsigned int get_capacity_dpipe(const dpipe* pipe);

int is_empty_dpipe(const dpipe* pipe);
int is_full_dpipe(const dpipe* pipe);

// can be used to expand or shrink the dpipe, the new_capacity becomes the new capacity
int resize_dpipe(dpipe* pipe, unsigned int new_capacity);

void close_dpipe(dpipe* pipe);

int is_dpipe_closed(const dpipe* pipe);

void deinitialize_dpipe(dpipe* pipe);

#endif