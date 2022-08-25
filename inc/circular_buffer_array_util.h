#ifndef CIRCULAR_BUFFER_ARRAY_UTIL_H
#define CIRCULAR_BUFFER_ARRAY_UTIL_H

// ==== BASE FUNTIONS START ====

/*
**	For all base functions
**		0 <= i1, i2 < buffer_capacity
**	i.e. i1 and i2 are indices of elements in a circular array buffer with capacity of buffer_capacity
*/

// equivalent to => (i1 + i2) % buffer_capacity
static inline unsigned int add_indexes(unsigned int i1, unsigned int i2, unsigned int buffer_capacity)
{
	if(buffer_capacity - i1 > i2)
		return i1 + i2;
	else
		return i2 - (buffer_capacity - i1);
}

// equivalent to => (buffer_capacity + i1 - i2) % buffer_capacity
static inline unsigned int sub_indexes(unsigned int i1, unsigned int i2, unsigned int buffer_capacity)
{
	if(i1 >= i2)
		return i1 - i2;
	else
		return buffer_capacity - (i2 - i1);
}

// ==== BASE FUNCTIONS END ====

// ==== OTHER FUNCTIONS

/*
**	for all below functions 
**		0 <= i, first_index < buffer_capacity
**		1 <= element_count <= buffer_capacity
** and buffer_capacity >= 1
**
**	first_index and element_count will have meanings as their (so obvious) names depict in the respective functions
*/

#define get_circular_next(i, buffer_capacity) add_indexes(i, 1, buffer_capacity)
#define get_circular_prev(i, buffer_capacity) sub_indexes(i, 1, buffer_capacity)

#define get_last_index(first_index, element_count, buffer_capacity) \
			add_indexes(first_index, element_count - 1, buffer_capacity)

#define get_end_index(first_index, element_count, buffer_capacity) \
			get_circular_next(get_last_index(first_index, element_count, buffer_capacity), buffer_capacity)

/*
	This file is used by following data structures
	* arraylist
	* circular queue (implemented as queue in cutlery, but in the backend its just an arraylist)
	* circular buffers used for network stacks (future)
*/

#endif