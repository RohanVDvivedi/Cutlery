#include<dpipe.h>

#include<cutlery_math.h>
#include<cutlery_stds.h>
#include<circular_buffer_array_util.h>

void initialize_dpipe(dpipe* pipe, unsigned int capacity)
{
	initialize_dpipe_with_allocator(pipe, capacity, STD_C_mem_allocator);
}

void initialize_dpipe_with_allocator(dpipe* pipe, unsigned int capacity, memory_allocator buffer_allocator)
{
	pipe->is_closed = 0;
	pipe->buffer_capacity = capacity;
	pipe->first_byte = 0;
	pipe->byte_count = 0;
	pipe->buffer_allocator = buffer_allocator;
	pipe->buffer = (pipe->buffer_capacity == 0) ? NULL : allocate(pipe->buffer_allocator, &(pipe->buffer_capacity));
}

void initialize_dpipe_with_memory(dpipe* pipe, unsigned int capacity, void* buffer)
{
	pipe->is_closed = 0;
	pipe->buffer_capacity = capacity;
	pipe->first_byte = 0;
	pipe->byte_count = 0;
	pipe->buffer_allocator = NULL;
	pipe->buffer = buffer;
}

unsigned int write_to_dpipe(dpipe* pipe, const void* data, unsigned int data_size, dpipe_operation_type op_type)
{
	return push_back_to_dpipe(pipe, data, data_size, op_type);
}

unsigned int read_from_dpipe(dpipe* pipe, void* data, unsigned int data_size, dpipe_operation_type op_type)
{
	return pop_front_from_dpipe(pipe, data, data_size, op_type);
}

unsigned int unread_to_dpipe(dpipe* pipe, const void* data, unsigned int data_size, dpipe_operation_type op_type)
{
	return push_front_to_dpipe(pipe, data, data_size, op_type);
}

unsigned int peek_front_of_dpipe(const dpipe* pipe, void* data, unsigned int data_size, dpipe_operation_type op_type)
{
	return get_front_of_dpipe(pipe, data, data_size, op_type);
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
	// we can not write more than buffer_capacity number of bytes to the buffer
	bytes_to_write = min(bytes_to_write, buffer_capacity);

	// return value
	unsigned int bytes_written = bytes_to_write;

	// bytes writable until the buffer_capacity boundary hits use
	unsigned int bytes_readily_writable = min(bytes_to_write, buffer_capacity - offset);

	// perform a copy and update data to point to next available byte to copy from
	memory_move(buffer + offset, data, bytes_readily_writable);
	data += bytes_readily_writable;

	// update bytes_to_write
	bytes_to_write -= bytes_readily_writable;

	// write remaining bytes to first byte of the buffer i.e. buffer + 0
	if(bytes_to_write > 0)
		memory_move(buffer, data, bytes_to_write);

	return bytes_written;
}

unsigned int get_front_of_dpipe(const dpipe* pipe, void* data, unsigned int data_size, dpipe_operation_type op_type)
{
	// if the pipe is empty, then fail the read
	if(is_empty_dpipe(pipe))
		return 0;

	// if the op_type = ALL_OR_NONE and there isn't enough bytes to read then fail with a 0
	if(op_type == ALL_OR_NONE && get_bytes_readable_in_dpipe(pipe) < data_size)
		return 0;

	// total_bytes to read
	unsigned int bytes_to_read = min(get_bytes_readable_in_dpipe(pipe), data_size);

	// circular buffer copy starting at first_byte offset to be read
	copy_from_circular_buffer(pipe->buffer, pipe->buffer_capacity, pipe->first_byte, data, bytes_to_read);

	return bytes_to_read;
}

unsigned int get_back_of_dpipe(const dpipe* pipe, void* data, unsigned int data_size, dpipe_operation_type op_type)
{
	// if the pipe is empty, then fail the read
	if(is_empty_dpipe(pipe))
		return 0;

	// if the op_type = ALL_OR_NONE and there isn't enough bytes to read then fail with a 0
	if(op_type == ALL_OR_NONE && get_bytes_readable_in_dpipe(pipe) < data_size)
		return 0;

	// total_bytes to read
	unsigned int bytes_to_read = min(get_bytes_readable_in_dpipe(pipe), data_size);

	// circular buffer copy starting at first_byte offset to be read
	unsigned int first_byte_to_read = add_indexes(pipe->first_byte, get_bytes_readable_in_dpipe(pipe) - bytes_to_read, pipe->buffer_capacity);
	copy_from_circular_buffer(pipe->buffer, pipe->buffer_capacity, first_byte_to_read, data, bytes_to_read);

	return bytes_to_read;
}

unsigned int push_front_to_dpipe(dpipe* pipe, const void* data, unsigned int data_size, dpipe_operation_type op_type)
{
	// if the pipe is closed or is full, then fail the write
	if(is_dpipe_closed(pipe) || is_full_dpipe(pipe))
		return 0;

	// if the op_type = ALL_OR_NONE and all of the bytes of data can not be written then fail with a 0
	if(op_type == ALL_OR_NONE && get_bytes_writable_in_dpipe(pipe) < data_size)
		return 0;

	// total_bytes to write
	unsigned int bytes_to_write = min(get_bytes_writable_in_dpipe(pipe), data_size);

	// update the new first_byte
	pipe->first_byte = sub_indexes(pipe->first_byte, bytes_to_write, pipe->buffer_capacity);

	// copy the data to the first_byte in the buffer
	copy_to_circular_buffer(pipe->buffer, pipe->buffer_capacity, pipe->first_byte, data, bytes_to_write);

	// increment byte_count
	pipe->byte_count += bytes_to_write;

	return bytes_to_write;
}

unsigned int push_back_to_dpipe(dpipe* pipe, const void* data, unsigned int data_size, dpipe_operation_type op_type)
{
	// if the pipe is closed or is full, then fail the write
	if(is_dpipe_closed(pipe) || is_full_dpipe(pipe))
		return 0;

	// if the op_type = ALL_OR_NONE and all of the bytes of data can not be written then fail with a 0
	if(op_type == ALL_OR_NONE && get_bytes_writable_in_dpipe(pipe) < data_size)
		return 0;

	// total_bytes to write
	unsigned int bytes_to_write = min(get_bytes_writable_in_dpipe(pipe), data_size);

	// calculate the offset to end of the buffer and copy the data there
	unsigned int end_offset = add_indexes(pipe->first_byte, pipe->byte_count, pipe->buffer_capacity);
	copy_to_circular_buffer(pipe->buffer, pipe->buffer_capacity, end_offset, data, bytes_to_write);

	// increment byte_count
	pipe->byte_count += bytes_to_write;

	return bytes_to_write;
}

unsigned int pop_front_from_dpipe(dpipe* pipe, void* data, unsigned int data_size, dpipe_operation_type op_type)
{
	// read bytes_read number of bytes from front of dpipe into data (max capacity = data_size)
	unsigned int bytes_read = get_front_of_dpipe(pipe, data, data_size, op_type);

	// update the first_byte and the byte_count
	pipe->first_byte = add_indexes(pipe->first_byte, bytes_read, pipe->buffer_capacity);
	pipe->byte_count -= bytes_read;

	// reset the first_byte, if the byte_count = 0
	if(pipe->byte_count == 0)
		pipe->first_byte = 0;

	return bytes_read;
}

unsigned int pop_back_from_dpipe(dpipe* pipe, void* data, unsigned int data_size, dpipe_operation_type op_type)
{
	// read bytes_read number of bytes from back of dpipe into data (max capacity = data_size)
	unsigned int bytes_read = get_back_of_dpipe(pipe, data, data_size, op_type);

	// update the byte_count
	pipe->byte_count -= bytes_read;

	// reset the first_byte, if the byte_count = 0
	if(pipe->byte_count == 0)
		pipe->first_byte = 0;

	return bytes_read;
}

void remove_all_from_dpipe(dpipe* pipe)
{
	pipe->first_byte = 0;
	pipe->byte_count = 0;
}

unsigned int get_bytes_writable_in_dpipe(const dpipe* pipe)
{
	return pipe->buffer_capacity - pipe->byte_count;
}

unsigned int get_bytes_readable_in_dpipe(const dpipe* pipe)
{
	return pipe->byte_count;
}

unsigned int get_capacity_dpipe(const dpipe* pipe)
{
	return pipe->buffer_capacity;
}

int is_empty_dpipe(const dpipe* pipe)
{
	return pipe->byte_count == 0;
}

int is_full_dpipe(const dpipe* pipe)
{
	return pipe->byte_count == pipe->buffer_capacity;
}

int resize_dpipe(dpipe* pipe, unsigned int new_capacity)
{
	// can not skrink, if the new capacity can not fit the bytes that dpipe is holding
	if(new_capacity < pipe->byte_count)
		return 0;

	if(pipe->buffer_capacity == new_capacity)
		return 1;

	// if the byte count is 0, then resize must be successfull
	if(pipe->byte_count == 0)
	{
		pipe->buffer = reallocate(pipe->buffer_allocator, pipe->buffer, pipe->buffer_capacity, &new_capacity);
		pipe->buffer_capacity = new_capacity;
		pipe->first_byte = 0;
		return 1;
	}

	// calculate the offset to last byte
	unsigned int last_byte_offset = add_indexes(pipe->first_byte, pipe->byte_count - 1, pipe->buffer_capacity);

	if(pipe->buffer_capacity > new_capacity) // shrinking
	{
		// if the dpipe is not wound around, then shrink
		if(pipe->first_byte <= last_byte_offset)
		{
			if(last_byte_offset >= new_capacity) //if last_byte falls out of the new_capacity upon a shrink, then move the dpipe contents to offset 0, before shrinking
			{
				memory_move(pipe->buffer, pipe->buffer + pipe->first_byte, pipe->byte_count);
				pipe->first_byte = 0;
			}

			// shrink the dpipe
			pipe->buffer = reallocate(pipe->buffer_allocator, pipe->buffer, pipe->buffer_capacity, &new_capacity);
			pipe->buffer_capacity = new_capacity;
			return 1;
		}
		return 0; // else if we can't shrink
	}
	else // expanding
	{
		pipe->buffer = reallocate(pipe->buffer_allocator, pipe->buffer, pipe->buffer_capacity, &new_capacity);

		// if there is not wound around then resize is complete
		if(pipe->first_byte <= last_byte_offset)
		{
			pipe->buffer_capacity = new_capacity;
			return 1;
		}

		unsigned int buffer_head_bytes = pipe->buffer_capacity - pipe->first_byte;
		unsigned int buffer_tail_bytes = pipe->byte_count - buffer_head_bytes;

		if(buffer_head_bytes <= buffer_tail_bytes) // move buffer head to the end of the buffer
		{
			// move the buffer_head to the bottom of the buffer
			unsigned int new_first_byte = new_capacity - buffer_head_bytes;
			memory_move(pipe->buffer + new_first_byte, pipe->buffer + pipe->first_byte, buffer_head_bytes);

			// update the first byte
			pipe->first_byte = new_first_byte;
		}
		else // move part of buffer tail to the end of the buffer
		{
			unsigned int new_bytes_available_post_head = pipe->buffer_capacity - new_capacity;

			unsigned int buffer_tail_bytes_to_move = min(buffer_tail_bytes, new_bytes_available_post_head);

			memory_move(pipe->buffer + pipe->buffer_capacity, pipe->buffer, buffer_tail_bytes_to_move);

			// now move the remaining tail bytes to the 0th offset in the buffer
			memory_move(pipe->buffer, pipe->buffer + buffer_tail_bytes_to_move, buffer_tail_bytes - buffer_tail_bytes_to_move);
		}

		pipe->buffer_capacity = new_capacity;
		return 1;
	}
}

void close_dpipe(dpipe* pipe)
{
	pipe->is_closed = 1;
}

int is_dpipe_closed(const dpipe* pipe)
{
	return pipe->is_closed;
}

void deinitialize_dpipe(dpipe* pipe)
{
	close_dpipe(pipe);
	remove_all_from_dpipe(pipe);
	if(pipe->buffer_capacity > 0 && pipe->buffer != NULL && pipe->buffer_allocator != NULL)
		deallocate(pipe->buffer_allocator, pipe->buffer, pipe->buffer_capacity);
	pipe->buffer_allocator = NULL;
	pipe->buffer_capacity = 0;
	pipe->buffer = NULL;
}