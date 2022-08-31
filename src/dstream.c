#include<dstream.h>

#include<cutlery_math.h>
#include<cutlery_stds.h>
#include<circular_buffer_array_util.h>

void initialize_dstream(dstream* strm, unsigned int capacity)
{
	initialize_dstream_with_allocator(strm, capacity, STD_C_mem_allocator);
}

void initialize_dstream_with_allocator(dstream* strm, unsigned int capacity, memory_allocator buffer_allocator)
{
	strm->read_closed = 0;
	strm->write_closed = 0;
	strm->buffer_capacity = capacity;
	strm->first_byte = 0;
	strm->byte_count = 0;
	strm->buffer_allocator = buffer_allocator;
	strm->buffer = allocate(strm->buffer_allocator, &(strm->buffer_capacity));
}

void initialize_dstream_with_memory(dstream* strm, unsigned int capacity, void* buffer)
{
	strm->read_closed = 0;
	strm->write_closed = 0;
	strm->buffer_capacity = capacity;
	strm->first_byte = 0;
	strm->byte_count = 0;
	strm->buffer_allocator = NULL;
	strm->buffer = buffer;
}

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

static inline unsigned int copy_from_circular_buffer(const void* buffer, unsigned int buffer_capacity, unsigned int offset, void* data, unsigned int bytes_to_read)
{
	// we can not read more than buffer_capacity number of bytes from buffer
	bytes_to_read = min(bytes_to_read, buffer_capacity);

	// return value
	unsigned int bytes_read = bytes_to_read;

	// bytes readable until the buffer_capacity boundary hits use
	unsigned int bytes_readily_readable = min(bytes_to_read, buffer_capacity - offset);

	// perform a copy and update data to point to next available byte to copy to
	memory_move(data, buffer + offset, bytes_readily_readable);
	data += bytes_readily_readable;

	// update bytes_to_read
	bytes_to_read -= bytes_readily_readable;

	// read remaining bytes from first byte of the buffer i.e. buffer + 0
	if(bytes_to_read > 0)
		memory_move(data, buffer, bytes_to_read);

	return bytes_read;
}

static inline unsigned int copy_to_circular_buffer(void* buffer, unsigned int buffer_capacity, unsigned int offset, const void* data, unsigned int bytes_to_write)
{

}

unsigned int get_front_of_dstream(const dstream* strm, void* data, unsigned int data_size, dstream_operation_type op_type)
{
	// if the stream is read_closed, then fail the read
	if(is_closed_for_reader(strm) || is_empty_dstream(strm))
		return 0;

	// if the op_type = ALL_OR_NONE and there isn't enough bytes to read then fail with a 0
	if(op_type == ALL_OR_NONE && get_bytes_readable_in_dstream(strm) < data_size)
		return 0;

	// total_bytes to read
	unsigned int bytes_to_read = min(get_bytes_readable_in_dstream(strm), data_size);

	// circular buffer copy starting at first_byte offset to be read
	copy_from_circular_buffer(strm->buffer, strm->buffer_capacity, strm->first_byte, data, bytes_to_read);

	return bytes_to_read;
}

unsigned int get_back_of_dstream(const dstream* strm, void* data, unsigned int data_size, dstream_operation_type op_type)
{
	// if the stream is read_closed, then fail the read
	if(is_closed_for_reader(strm) || is_empty_dstream(strm))
		return 0;

	// if the op_type = ALL_OR_NONE and there isn't enough bytes to read then fail with a 0
	if(op_type == ALL_OR_NONE && get_bytes_readable_in_dstream(strm) < data_size)
		return 0;

	// total_bytes to read
	unsigned int bytes_to_read = min(get_bytes_readable_in_dstream(strm), data_size);

	// circular buffer copy starting at first_byte offset to be read
	unsigned int first_byte_to_read = add_indexes(strm->first_byte, get_bytes_readable_in_dstream(strm) - bytes_to_read, strm->buffer_capacity);
	copy_from_circular_buffer(strm->buffer, strm->buffer_capacity, first_byte_to_read, data, bytes_to_read);

	return bytes_to_read;
}

unsigned int push_front_to_dstream(dstream* strm, const void* data, unsigned int data_size, dstream_operation_type op_type)
{
	// if the stream is write_closed or id full, then fail the write
	if(is_closed_for_writer(strm) || is_full_dstream(strm))
		return 0;

	// if the op_type = ALL_OR_NONE and all of the bytes of data can not be written then fail with a 0
	if(op_type == ALL_OR_NONE && get_bytes_writable_in_dstream(strm) < data_size)
		return 0;

	// total_bytes to write
	unsigned int bytes_to_write = min(get_bytes_writable_in_dstream(strm), data_size);

	// update the new first_byte
	strm->first_byte = sub_indexes(strm->first_byte, bytes_to_write, strm->buffer_capacity);

	// copy the data to the first_byte in the buffer
	copy_to_circular_buffer(strm->buffer, strm->buffer_capacity, strm->first_byte, data, bytes_to_write);

	// increment byte_count
	strm->byte_count += bytes_to_write;

	return bytes_to_write;
}

unsigned int push_back_to_dstream(dstream* strm, const void* data, unsigned int data_size, dstream_operation_type op_type)
{
	// if the stream is write_closed, then fail the write
	if(is_closed_for_writer(strm) || is_full_dstream(strm))
		return 0;

	// if the op_type = ALL_OR_NONE and all of the bytes of data can not be written then fail with a 0
	if(op_type == ALL_OR_NONE && get_bytes_writable_in_dstream(strm) < data_size)
		return 0;

	// total_bytes to write
	unsigned int bytes_to_write = min(get_bytes_writable_in_dstream(strm), data_size);

	// calculate the offset to end of the buffer and copy the data there
	unsigned int end_offset = add_indexes(strm->first_byte, strm->byte_count, strm->buffer_capacity);
	copy_to_circular_buffer(strm->buffer, strm->buffer_capacity, end_offset, data, bytes_to_write);

	// increment byte_count
	strm->byte_count += bytes_to_write;

	return bytes_to_write;
}

unsigned int pop_front_from_dstream(dstream* strm, void* data, unsigned int data_size, dstream_operation_type op_type)
{
	// read bytes_read number of bytes from front of dstream into data (max capacity = data_size)
	unsigned int bytes_read = get_front_of_dstream(strm, data, data_size, op_type);

	// update the first_byte and the byte_count
	strm->first_byte = add_indexes(strm->first_byte, bytes_read, strm->buffer_capacity);
	strm->byte_count -= bytes_read;

	return bytes_read;
}

unsigned int pop_back_from_dstream(dstream* strm, void* data, unsigned int data_size, dstream_operation_type op_type)
{
	// read bytes_read number of bytes from back of dstream into data (max capacity = data_size)
	unsigned int bytes_read = get_back_of_dstream(strm, data, data_size, op_type);

	// update the byte_count
	strm->byte_count -= bytes_read;

	return bytes_read;
}

void remove_all_from_dstream(dstream* strm)
{
	strm->first_byte = 0;
	strm->byte_count = 0;
}

unsigned int get_bytes_writable_in_dstream(const dstream* strm)
{
	return strm->buffer_capacity - strm->byte_count;
}

unsigned int get_bytes_readable_in_dstream(const dstream* strm)
{
	return strm->byte_count;
}

int is_empty_dstream(const dstream* strm)
{
	return strm->byte_count == 0;
}

int is_full_dstream(const dstream* strm)
{
	return strm->byte_count == strm->buffer_capacity;
}

int resize_dstream(dstream* strm, unsigned int new_capacity);

void close_dstream_for_writer(dstream* strm)
{
	strm->write_closed = 1;
}

void close_dstream_for_reader(dstream* strm)
{
	strm->read_closed = 1;
}

int is_closed_for_writer(const dstream* strm)
{
	return strm->write_closed;
}

int is_closed_for_reader(const dstream* strm)
{
	return strm->read_closed;
}

void deinitialize_dstream(dstream* strm)
{
	is_closed_for_writer(strm);
	is_closed_for_reader(strm);
	remove_all_from_dstream(strm);
	if(strm->buffer_capacity > 0 && strm->buffer != NULL && strm->buffer_allocator != NULL)
		deallocate(strm->buffer_allocator, strm->buffer, strm->buffer_capacity);
	strm->buffer_allocator = NULL;
	strm->buffer_capacity = 0;
	strm->buffer = NULL;
}