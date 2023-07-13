#include<dpipe.h>

#include<cutlery_math.h>
#include<cutlery_stds.h>
#include<circular_buffer_array_util.h>

void initialize_dpipe(dpipe* pipe, cy_uint capacity)
{
	initialize_dpipe_with_allocator(pipe, capacity, STD_C_mem_allocator);
}

void initialize_dpipe_with_allocator(dpipe* pipe, cy_uint capacity, memory_allocator buffer_allocator)
{
	pipe->is_closed = 0;
	pipe->buffer_capacity = capacity;
	pipe->first_byte = 0;
	pipe->byte_count = 0;
	pipe->buffer_allocator = buffer_allocator;
	pipe->buffer = (pipe->buffer_capacity == 0) ? NULL : allocate(pipe->buffer_allocator, &(pipe->buffer_capacity));
}

void initialize_dpipe_with_memory(dpipe* pipe, cy_uint capacity, void* buffer)
{
	pipe->is_closed = 0;
	pipe->buffer_capacity = capacity;
	pipe->first_byte = 0;
	pipe->byte_count = 0;
	pipe->buffer_allocator = NULL;
	pipe->buffer = buffer;
}

cy_uint write_to_dpipe(dpipe* pipe, const void* data, cy_uint data_size, dpipe_operation_type op_type)
{
	return push_back_to_dpipe(pipe, data, data_size, op_type);
}

cy_uint read_from_dpipe(dpipe* pipe, void* data, cy_uint data_size, dpipe_operation_type op_type)
{
	cy_uint bytes_read = get_front_of_dpipe(pipe, data, data_size, op_type);
	pop_front_from_dpipe(pipe, bytes_read);
	return bytes_read;
}

cy_uint unread_to_dpipe(dpipe* pipe, const void* data, cy_uint data_size, dpipe_operation_type op_type)
{
	return push_front_to_dpipe(pipe, data, data_size, op_type);
}

cy_uint peek_from_dpipe(const dpipe* pipe, void* data, cy_uint data_size, dpipe_operation_type op_type)
{
	return get_front_of_dpipe(pipe, data, data_size, op_type);
}

int discard_from_dpipe(dpipe* pipe, cy_uint data_size)
{
	return pop_front_from_dpipe(pipe, data_size);
}

const void* peek_max_consecutive_from_dpipe(const dpipe* pipe, cy_uint* bytes_available)
{
	return get_max_consecutive_from_front_of_dpipe(pipe, bytes_available);
}

static inline cy_uint copy_from_circular_buffer(const void* buffer, cy_uint buffer_capacity, cy_uint offset, void* data, cy_uint bytes_to_read)
{
	// we can not read more than buffer_capacity number of bytes from buffer
	bytes_to_read = min(bytes_to_read, buffer_capacity);

	// return value
	cy_uint bytes_read = bytes_to_read;

	// bytes readable until the buffer_capacity boundary hits use
	cy_uint bytes_readily_readable = min(bytes_to_read, buffer_capacity - offset);

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

static inline cy_uint copy_to_circular_buffer(void* buffer, cy_uint buffer_capacity, cy_uint offset, const void* data, cy_uint bytes_to_write)
{
	// we can not write more than buffer_capacity number of bytes to the buffer
	bytes_to_write = min(bytes_to_write, buffer_capacity);

	// return value
	cy_uint bytes_written = bytes_to_write;

	// bytes writable until the buffer_capacity boundary hits use
	cy_uint bytes_readily_writable = min(bytes_to_write, buffer_capacity - offset);

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

cy_uint get_front_of_dpipe(const dpipe* pipe, void* data, cy_uint data_size, dpipe_operation_type op_type)
{
	// if the pipe is empty, then fail the read
	if(is_empty_dpipe(pipe) || data_size == 0)
		return 0;

	// if the op_type = ALL_OR_NONE and there isn't enough bytes to read then fail with a 0
	if(op_type == ALL_OR_NONE && get_bytes_readable_in_dpipe(pipe) < data_size)
		return 0;

	// total_bytes to read
	cy_uint bytes_to_read = min(get_bytes_readable_in_dpipe(pipe), data_size);

	// circular buffer copy starting at first_byte offset to be read
	copy_from_circular_buffer(pipe->buffer, pipe->buffer_capacity, pipe->first_byte, data, bytes_to_read);

	return bytes_to_read;
}

cy_uint get_back_of_dpipe(const dpipe* pipe, void* data, cy_uint data_size, dpipe_operation_type op_type)
{
	// if the pipe is empty, then fail the read
	if(is_empty_dpipe(pipe) || data_size == 0)
		return 0;

	// if the op_type = ALL_OR_NONE and there isn't enough bytes to read then fail with a 0
	if(op_type == ALL_OR_NONE && get_bytes_readable_in_dpipe(pipe) < data_size)
		return 0;

	// total_bytes to read
	cy_uint bytes_to_read = min(get_bytes_readable_in_dpipe(pipe), data_size);

	// circular buffer copy starting at first_byte offset to be read
	// which is same as the end offset of the front of the bytes not to be read
	cy_uint first_byte_to_read = get_end_index(pipe->first_byte, get_bytes_readable_in_dpipe(pipe) - bytes_to_read, pipe->buffer_capacity);
	copy_from_circular_buffer(pipe->buffer, pipe->buffer_capacity, first_byte_to_read, data, bytes_to_read);

	return bytes_to_read;
}

cy_uint push_front_to_dpipe(dpipe* pipe, const void* data, cy_uint data_size, dpipe_operation_type op_type)
{
	// if the pipe is closed or is full, then fail the write
	if(is_dpipe_closed(pipe) || is_full_dpipe(pipe) || data_size == 0)
		return 0;

	// if the op_type = ALL_OR_NONE and all of the bytes of data can not be written then fail with a 0
	if(op_type == ALL_OR_NONE && get_bytes_writable_in_dpipe(pipe) < data_size)
		return 0;

	// total_bytes to write
	cy_uint bytes_to_write = min(get_bytes_writable_in_dpipe(pipe), data_size);

	// update the new first_byte
	pipe->first_byte = sub_circularly(pipe->first_byte, bytes_to_write, pipe->buffer_capacity);

	// copy the data to the first_byte in the buffer
	copy_to_circular_buffer(pipe->buffer, pipe->buffer_capacity, pipe->first_byte, data, bytes_to_write);

	// increment byte_count
	pipe->byte_count += bytes_to_write;

	return bytes_to_write;
}

cy_uint push_back_to_dpipe(dpipe* pipe, const void* data, cy_uint data_size, dpipe_operation_type op_type)
{
	// if the pipe is closed or is full, then fail the write
	if(is_dpipe_closed(pipe) || is_full_dpipe(pipe) || data_size == 0)
		return 0;

	// if the op_type = ALL_OR_NONE and all of the bytes of data can not be written then fail with a 0
	if(op_type == ALL_OR_NONE && get_bytes_writable_in_dpipe(pipe) < data_size)
		return 0;

	// total_bytes to write
	cy_uint bytes_to_write = min(get_bytes_writable_in_dpipe(pipe), data_size);

	// calculate the offset to end of the buffer and copy the data there
	cy_uint end_offset = get_end_index(pipe->first_byte, pipe->byte_count, pipe->buffer_capacity);
	copy_to_circular_buffer(pipe->buffer, pipe->buffer_capacity, end_offset, data, bytes_to_write);

	// increment byte_count
	pipe->byte_count += bytes_to_write;

	return bytes_to_write;
}

int pop_front_from_dpipe(dpipe* pipe, cy_uint data_size)
{
	// if data_size is greater than bytes in dpipe, then return failure
	if(data_size > pipe->byte_count)
		return 0;

	if(data_size == 0)
		return 1;

	// update the first_byte and the byte_count
	pipe->first_byte = add_circularly(pipe->first_byte, data_size, pipe->buffer_capacity);
	pipe->byte_count -= data_size;

	// reset the first_byte, if the byte_count = 0
	if(pipe->byte_count == 0)
		pipe->first_byte = 0;

	// pop success
	return 1;
}

int pop_back_from_dpipe(dpipe* pipe, cy_uint data_size)
{
	// if data_size is greater than bytes in dpipe, then return failure
	if(data_size > pipe->byte_count)
		return 0;

	if(data_size == 0)
		return 1;

	// update the byte_count
	pipe->byte_count -= data_size;

	// reset the first_byte, if the byte_count = 0
	if(pipe->byte_count == 0)
		pipe->first_byte = 0;

	// pop success
	return 1;
}

const void* get_max_consecutive_from_front_of_dpipe(const dpipe* pipe, cy_uint* bytes_available)
{
	// return NULL, if there are no bytes available
	if(pipe->byte_count == 0)
	{
		(*bytes_available) = 0;
		return NULL;
	}

	// construct internal buffer to the first byte and set number of consecutive number of bytes at that address in bytes_available 
	const void* internal = pipe->buffer + pipe->first_byte;
	(*bytes_available) = min(pipe->buffer_capacity - pipe->first_byte, pipe->byte_count);

	return internal;
}

const void* get_max_consecutive_from_back_of_dpipe(const dpipe* pipe, cy_uint* bytes_available)
{
	// return NULL, if there are no bytes available
	if(pipe->byte_count == 0)
	{
		(*bytes_available) = 0;
		return NULL;
	}

	const void* internal = NULL;

	if(pipe->buffer_capacity - pipe->first_byte >= pipe->byte_count) // case when all bytes are consecutive
	{
		internal = pipe->buffer + pipe->first_byte;
		(*bytes_available) = pipe->byte_count;
	}
	else // else we need to search for tail bytes that start at first_byte
	{
		internal = pipe->buffer;
		(*bytes_available) = pipe->byte_count - (pipe->buffer_capacity - pipe->first_byte);
	}

	return internal;
}

void remove_all_from_dpipe(dpipe* pipe)
{
	pipe->first_byte = 0;
	pipe->byte_count = 0;
}

cy_uint get_bytes_writable_in_dpipe(const dpipe* pipe)
{
	return pipe->buffer_capacity - pipe->byte_count;
}

cy_uint get_bytes_readable_in_dpipe(const dpipe* pipe)
{
	return pipe->byte_count;
}

cy_uint get_capacity_dpipe(const dpipe* pipe)
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

int resize_dpipe(dpipe* pipe, cy_uint new_capacity)
{
	// can not skrink, if the new capacity can not fit the bytes that dpipe is holding
	if(new_capacity < pipe->byte_count)
		return 0;

	if(pipe->buffer_capacity == new_capacity)
		return 1;

	// if the byte count is 0, then resize must be successfull
	if(pipe->byte_count == 0)
	{
		pipe->buffer = reallocate(pipe->buffer_allocator, pipe->buffer, ((cy_uint)pipe->buffer_capacity), &new_capacity);
		pipe->buffer_capacity = new_capacity;
		pipe->first_byte = 0;
		return 1;
	}

	// calculate the offset to last byte
	cy_uint last_byte_offset = get_last_index(pipe->first_byte, pipe->byte_count, pipe->buffer_capacity);

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
			pipe->buffer = reallocate(pipe->buffer_allocator, pipe->buffer, ((cy_uint)pipe->buffer_capacity), &new_capacity);
			pipe->buffer_capacity = new_capacity;
			return 1;
		}
		return 0; // else if we can't shrink
	}
	else // expanding
	{
		pipe->buffer = reallocate(pipe->buffer_allocator, pipe->buffer, ((cy_uint)pipe->buffer_capacity), &new_capacity);

		// if there is not wound around then resize is complete
		if(pipe->first_byte <= last_byte_offset)
		{
			pipe->buffer_capacity = new_capacity;
			return 1;
		}

		cy_uint buffer_head_bytes = pipe->buffer_capacity - pipe->first_byte;
		cy_uint buffer_tail_bytes = pipe->byte_count - buffer_head_bytes;

		if(buffer_head_bytes <= buffer_tail_bytes) // move buffer head to the end of the buffer
		{
			// move the buffer_head to the bottom of the buffer
			cy_uint new_first_byte = new_capacity - buffer_head_bytes;
			memory_move(pipe->buffer + new_first_byte, pipe->buffer + pipe->first_byte, buffer_head_bytes);

			// update the first byte
			pipe->first_byte = new_first_byte;
		}
		else // move part of buffer tail to the end of the buffer
		{
			cy_uint new_bytes_available_post_head = pipe->buffer_capacity - new_capacity;

			cy_uint buffer_tail_bytes_to_move = min(buffer_tail_bytes, new_bytes_available_post_head);

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
		deallocate(pipe->buffer_allocator, pipe->buffer, ((cy_uint)pipe->buffer_capacity));
	pipe->buffer_allocator = NULL;
	pipe->buffer_capacity = 0;
	pipe->buffer = NULL;
}