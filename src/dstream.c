#include<dstream.h>

#include<cutlery_stds.h>
#include<circular_buffer_array_util.h>

void initialize_dstream(dstream* strm, unsigned int capacity);
void initialize_dstream_with_allocator(dstream* strm, unsigned int capacity, memory_allocator buffer_allocator);
void initialize_dstream_with_memory(dstream* strm, unsigned int capacity, void* buffer);

unsigned int write_to_dstream(dstream* strm, const void* data, unsigned int data_size, dstream_operation_type op_type)
{
	return push_back_to_dstream(strm, data, data_size, op_type);
}

unsigned int read_from_dstream(dstream* strm, void* data, unsigned int data_size, dstream_operation_type op_type)
{
	return pop_front_from_dstream(strm, data, data_size, op_type);
}

unsigned int unread_to_dstream(dstream* strm, const void* data, unsigned int data_size, dstream_operation_type op_type)
{
	return push_front_to_dstream(strm, data, data_size, op_type);
}

unsigned int peek_front_of_dstream(const dstream* strm, void* data, unsigned int data_size, dstream_operation_type op_type)
{
	return get_front_of_dstream(strm, data, data_size, op_type);
}

unsigned int get_front_of_dstream(const dstream* strm, void* data, unsigned int data_size, dstream_operation_type op_type);
unsigned int get_back_of_dstream(const dstream* strm, void* data, unsigned int data_size, dstream_operation_type op_type);
unsigned int push_front_to_dstream(dstream* strm, const void* data, unsigned int data_size, dstream_operation_type op_type);
unsigned int push_back_to_dstream(dstream* strm, const void* data, unsigned int data_size, dstream_operation_type op_type);
unsigned int pop_front_from_dstream(dstream* strm, void* data, unsigned int data_size, dstream_operation_type op_type);
unsigned int pop_back_from_dstream(dstream* strm, void* data, unsigned int data_size, dstream_operation_type op_type);

void remove_all_from_dstream(dstream* strm);

int is_empty_dstream(const dstream* strm)
{
	return strm->byte_count == 0;
}

int is_full_dstream(const dstream* strm)
{
	return strm->byte_count == strm->buffer_capacity;
}

int resize_dstream(dstream* strm, unsigned int new_capacity);

void close_dstream_for_writer(dstream* strm);

void close_dstream_for_reader(dstream* strm);

int is_closed_for_writer(const dstream* strm)
{
	return strm->write_closed;
}

int is_closed_for_reader(const dstream* strm)
{
	return strm->read_closed;
}

void deinitialize_dstream();