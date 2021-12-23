#ifndef CIRCULAR_BUFFER_ARRAY_UTIL_H
#define CIRCULAR_BUFFER_ARRAY_UTIL_H

// ==== BASE FUNTIONS START ====

/*
**	For all base functions
**		0 <= i1, i2 < buffer_capacity
**	i.e. i1 and i2 are indices of elements in a curcular array buffer with capacity of buffer_capacity
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
	if(i1 + 1 > i2)
		return i1 - i2;
	else
		return (buffer_capacity - 1) - (i2 - (i1 + 1));
}

// ==== BASE FUNCTIONS END ====

// ==== OTHER FUNCTIONS


/*
	This file is used by following data structures
	* arraylist
	* circular queue (implemented as queue in cutlery, but in the backend its just an arraylist)
	* circular buffers used for network stacks (future)
*/

#endif