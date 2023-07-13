#ifndef CIRCULAR_BUFFER_ARRAY_UTIL_H
#define CIRCULAR_BUFFER_ARRAY_UTIL_H

#include<cutlery_stds.h>

// ==== BASE FUNTIONS START ====

/*
**	For all base functions
**		0 <= i1, i2 <= buffer_capacity AND buffer_capacity > 0
**	i.e. i1 and i2 are indices/sizes of elements in a circular array buffer with capacity of buffer_capacity
*/

// equivalent to => (i1 + i2) % buffer_capacity
static inline cy_uint add_circularly(cy_uint i1, cy_uint i2, cy_uint buffer_capacity)
{
	// normalize i1 and i2, i.e. if they are equal to buffer_capcity then set then to 0
	if(i1 == buffer_capacity)
		i1 = 0;
	if(i2 == buffer_capacity)
		i2 = 0;

	// i1 or i2 >= buffer_capacity, beyond this point is illegal

	if(buffer_capacity - i1 > i2)
		return i1 + i2;
	else
		return i2 - (buffer_capacity - i1);
}

// equivalent to => (buffer_capacity + i1 - i2) % buffer_capacity
static inline cy_uint sub_circularly(cy_uint i1, cy_uint i2, cy_uint buffer_capacity)
{
	// normalize i1 and i2, i.e. if they are equal to buffer_capcity then set then to 0
	if(i1 == buffer_capacity)
		i1 = 0;
	if(i2 == buffer_capacity)
		i2 = 0;

	// i1 or i2 >= buffer_capacity, beyond this point is illegal

	if(i1 >= i2)
		return i1 - i2;
	else
		return buffer_capacity - (i2 - i1);
}

// ==== BASE FUNCTIONS END ====

// ==== OTHER FUNCTIONS

/*
**	for all below functions 
**		0 <= i, first_index < buffer_capacity		(although i, first_index == buffer_capacity is not invalid, but they are suppossed to be indexes in the circular buffer)
**		0 <= element_count <= buffer_capacity
** and buffer_capacity > 0
**
**	first_index and element_count will have meanings as their (so obvious) names depict in the respective functions
*/

#define get_circular_next(i, buffer_capacity) add_circularly(i, 1, buffer_capacity)
#define get_circular_prev(i, buffer_capacity) sub_circularly(i, 1, buffer_capacity)

// only for the get_last_index function, element_count > 0, which is obvious since there is no last index-ed element, if the element_count = 0
#define get_last_index(first_index, element_count, buffer_capacity) \
			add_circularly(first_index, element_count - 1, buffer_capacity)

#define get_end_index(first_index, element_count, buffer_capacity) \
			add_circularly(first_index, element_count, buffer_capacity)

/*
	This file is used by following data structures
	* arraylist
	* circular queue (implemented as queue in cutlery, but in the backend its just an arraylist)
	* circular buffers used for network stacks (future)
*/

#endif