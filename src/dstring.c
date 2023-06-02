#include<dstring.h>

#include<cutlery_stds.h>
#include<cutlery_math.h>
#include<memory_allocator_interface.h>

// default memory allocator for dstring is the STD_C memory allocator
memory_allocator DSTRING_mem_alloc = &STD_C_memory_allocator;

// accessor for dstring attribute type_n_SS_size
#define get_dstr_type(type_n_SS_size)		((type_n_SS_size) & 0x3)
#define get_dstr_SS_size(type_n_SS_size)	(((type_n_SS_size) >> 2) & 0x3f)

#define set_dstr_SS_size(type_n_SS_size, SS_size)	(type_n_SS_size) = ((((SS_size) << 2) & 0xfc) | ((type_n_SS_size) & 0x3))

#define SS_data_offset		((cy_uint)((&(((dstring*)0)->type_n_SS_size))+sizeof(unsigned char)))
#define SS_capacity			(sizeof(dstring)-SS_data_offset)

// BASE METHODS START

dstring_type get_dstring_type(const dstring* str_p)
{
	return get_dstr_type(str_p->type_n_SS_size);
}

void init_empty_dstring(dstring* str_p, cy_uint capacity)
{
	if(capacity > SS_capacity)
	{
		str_p->type_n_SS_size = LARGE_DSTR;
		str_p->bytes_occupied = 0;
		str_p->byte_array = allocate(DSTRING_mem_alloc, &capacity);
		str_p->bytes_allocated = (str_p->byte_array == NULL) ? 0 : capacity;
	}
	else 	// return a short dstring with size 0
		str_p->type_n_SS_size = SHORT_DSTR;
}

char* get_byte_array_dstring(const dstring* str_p)
{
	if(get_dstring_type(str_p) != SHORT_DSTR)
		return str_p->byte_array;
	return ((char*)str_p) + SS_data_offset;
}

cy_uint get_char_count_dstring(const dstring* str_p)
{
	if(get_dstring_type(str_p) != SHORT_DSTR)
		return str_p->bytes_occupied;
	return get_dstr_SS_size(str_p->type_n_SS_size);
}

int increment_char_count_dstring(dstring* str_p, cy_uint increment_by)
{
	if(get_dstring_type(str_p) == POINT_DSTR || increment_by > get_unused_capacity_dstring(str_p))
		return 0;
	if(get_dstring_type(str_p) == SHORT_DSTR)
	{
		cy_uint new_SS_size = get_char_count_dstring(str_p) + increment_by;
		set_dstr_SS_size(str_p->type_n_SS_size, new_SS_size);
	}
	else
		str_p->bytes_occupied += increment_by;
	return 1;
}

int decrement_char_count_dstring(dstring* str_p, cy_uint decrement_by)
{
	if(get_dstring_type(str_p) == POINT_DSTR || decrement_by > get_char_count_dstring(str_p))
		return 0;
	if(get_dstring_type(str_p) == SHORT_DSTR)
	{
		cy_uint new_SS_size = get_char_count_dstring(str_p) - decrement_by;
		set_dstr_SS_size(str_p->type_n_SS_size, new_SS_size);
	}
	else
		str_p->bytes_occupied -= decrement_by;
	return 1;
}

void make_dstring_empty(dstring* str_p)
{
	if(get_dstring_type(str_p) == SHORT_DSTR)
		set_dstr_SS_size(str_p->type_n_SS_size, 0);
	else
		str_p->bytes_occupied = 0;
}

cy_uint get_capacity_dstring(const dstring* str_p)
{
	if(get_dstring_type(str_p) == SHORT_DSTR)
		return SS_capacity;
	return str_p->bytes_allocated;
}

cy_uint get_unused_capacity_dstring(const dstring* str_p)
{
	// a POINT_DSTR has no memory associated with it, hence it does not even have unused capacity
	if(get_dstring_type(str_p) == POINT_DSTR)
		return 0;
	return get_capacity_dstring(str_p) - get_char_count_dstring(str_p);
}

void deinit_dstring(dstring* str_p)
{
	if(get_dstring_type(str_p) == LARGE_DSTR && str_p->bytes_allocated > 0 && str_p->byte_array != NULL)
		deallocate(DSTRING_mem_alloc, str_p->byte_array, str_p->bytes_allocated);
	str_p->type_n_SS_size = SHORT_DSTR;
	str_p->byte_array = NULL;
	str_p->bytes_allocated = 0;
	str_p->bytes_occupied = 0;
}

int expand_dstring(dstring* str_p, cy_uint additional_allocation)
{
	// if it is a POINT_DSTR it can not be expanded or shrunk
	if(get_dstring_type(str_p) == POINT_DSTR)
		return 0;

	// if no additional allocation required then return success
	if(additional_allocation == 0)
		return 1;

	char* str_data = get_byte_array_dstring(str_p);
	cy_uint str_size = get_char_count_dstring(str_p);
	cy_uint str_capacity = get_capacity_dstring(str_p);

	// compute the new capacity
	cy_uint new_capacity = str_size + additional_allocation;

	// check for overflow
	if(new_capacity < str_size || new_capacity < additional_allocation)
		return 0;

	// can not be smaller than SS_capacity
	if(new_capacity < SS_capacity)
		new_capacity = SS_capacity;

	// if expansion is not resulting in expansion, then return 0 (failure)
	if(new_capacity <= str_capacity)
		return 0;

	// you might have to make a large dstring larger
	if(get_dstring_type(str_p) == LARGE_DSTR)
	{
		char* new_byte_array = reallocate(DSTRING_mem_alloc, str_data, str_capacity, &new_capacity);

		// failed allocation
		if(new_byte_array == NULL && new_capacity > 0)
			return 0;

		// update with new allocation array and its size
		str_p->byte_array = new_byte_array;
		str_p->bytes_allocated = new_capacity;
	}
	// or convert a short dstring in to a large dstring
	else if(get_dstring_type(str_p) == SHORT_DSTR)
	{
		// create a new LARGE_DSTR of new_capacity as its capacity
		dstring* str_large_p = &((dstring){});
		init_empty_dstring(str_large_p, new_capacity);

		// append str_p's data to the str_large_p
		memory_move(get_byte_array_dstring(str_large_p), get_byte_array_dstring(str_p), get_char_count_dstring(str_p));
		increment_char_count_dstring(str_large_p, get_char_count_dstring(str_p));

		// copy contents of str_large_p to str_p
		*str_p = *str_large_p;
	}

	return 1;
}

int shrink_dstring(dstring* str_p)
{
	// if it is a POINT_DSTR it can not be expanded or shrunk
	if(get_dstring_type(str_p) == POINT_DSTR)
		return 0;

	char* str_data = get_byte_array_dstring(str_p);
	cy_uint str_size = get_char_count_dstring(str_p);
	cy_uint str_capacity = get_capacity_dstring(str_p);

	// compute the new capacity
	cy_uint new_capacity = str_size;
	if(new_capacity <= SS_capacity)
		new_capacity = SS_capacity;

	// if the new_capacity is more than or equal to the old capacity
	// i.e. resulting in no skrinking then return 0 (failure to shrik the string)
	// this condition will weed out if str_p is a SHORT_DSTR
	if(new_capacity >= str_capacity)
		return 0;

	// str_p can only be a LARGE_DSTR, from here onwards

	// an already LARGE_DSTR getting converted to a smaller LARGE_DSTR
	if(new_capacity > SS_capacity)
	{
		void* new_byte_array = reallocate(DSTRING_mem_alloc, str_data, str_capacity, &new_capacity);

		// failed allocation
		if(new_byte_array == NULL && new_capacity > 0)
			return 0;

		str_p->byte_array = new_byte_array;
		str_p->bytes_allocated = new_capacity;
	}
	// an LARGE_DSTR getting converted to a SHORT_DSTR
	else if(new_capacity == SS_capacity)
	{
		// create a new SHORT_DSTR
		dstring* str_short_p = &((dstring){});
		init_empty_dstring(str_short_p, new_capacity);

		// copy contents/data from str_p to str_short_p
		memory_move(get_byte_array_dstring(str_short_p), get_byte_array_dstring(str_p), get_char_count_dstring(str_p));
		increment_char_count_dstring(str_short_p, get_char_count_dstring(str_p));

		// deinitialize str_p
		deinit_dstring(str_p);

		// and copy str_short_p to str_p
		*str_p = *str_short_p;
	}

	return 1;
}

// BASE METHODS END

void init_dstring(dstring* str_p, const char* data, cy_uint data_size)
{
	init_empty_dstring(str_p, data_size);
	if(data != NULL && data_size > 0)
	{
		memory_move(get_byte_array_dstring(str_p), data, data_size);
		increment_char_count_dstring(str_p, data_size);
	}
}

void init_copy_dstring(dstring* str_p, const dstring* init_copy_from)
{
	init_dstring(str_p, get_byte_array_dstring(init_copy_from), get_char_count_dstring(init_copy_from));
}

dstring new_dstring(const char* data, cy_uint data_size)
{
	dstring str;
	init_dstring(&str, data, data_size);
	return str;
}

dstring new_copy_dstring(const dstring* init_copy_from)
{
	dstring str;
	init_copy_dstring(&str, init_copy_from);
	return str;
}

int is_empty_dstring(const dstring* str_p)
{
	return 0 == get_char_count_dstring(str_p);
}

int discard_chars_dstring(dstring* str_p, cy_uint start_index, cy_uint last_index)
{
	// if it is a POINT_DSTR, you can not remove characters from it
	if(get_dstring_type(str_p) == POINT_DSTR)
		return 0;

	// check that the ranges of start_index and last_index are valid
	if(start_index > last_index || last_index >= get_char_count_dstring(str_p))
		return 0;

	char* str_data = get_byte_array_dstring(str_p);
	cy_uint str_size = get_char_count_dstring(str_p);

	// move the characters after the last_index to the position at start_index
	memory_move(str_data + start_index, str_data + last_index + 1, str_size - (last_index + 1));

	// decrement the char_count of the dstring
	decrement_char_count_dstring(str_p, last_index - start_index + 1);

	return 1;
}

int discard_chars_from_front_dstring(dstring* str_p, cy_uint bytes_to_discard)
{
	cy_uint str_size = get_char_count_dstring(str_p);

	if(str_size < bytes_to_discard)
		return 0;

	if(bytes_to_discard == 0)
		return 1;

	if(get_dstring_type(str_p) == POINT_DSTR)
	{
		const char* str_data = get_byte_array_dstring(str_p);
		(*str_p) = get_dstring_pointing_to(str_data + bytes_to_discard, str_size - bytes_to_discard);
		return 1;
	}
	else
		return discard_chars_dstring(str_p, 0, bytes_to_discard - 1);
}

int discard_chars_from_back_dstring(dstring* str_p, cy_uint bytes_to_discard)
{
	cy_uint str_size = get_char_count_dstring(str_p);

	if(str_size < bytes_to_discard)
		return 0;

	if(bytes_to_discard == 0)
		return 1;

	if(get_dstring_type(str_p) == POINT_DSTR)
	{
		const char* str_data = get_byte_array_dstring(str_p);
		(*str_p) = get_dstring_pointing_to(str_data, str_size - bytes_to_discard);
		return 1;
	}
	else
		return discard_chars_dstring(str_p, str_size - bytes_to_discard, str_size - 1);
}

static int make_room_for_bytes_in_dstring(dstring* str_p, cy_uint incoming_byte_count)
{
	if(incoming_byte_count <= get_unused_capacity_dstring(str_p))
		return 1;

	cy_uint str_size = get_char_count_dstring(str_p);

	// check to make sure that the new size (after appending incoming_byte_count), does not overflow
	cy_uint new_possible_size = str_size + incoming_byte_count;
	if(new_possible_size < str_size || new_possible_size < incoming_byte_count)
		return 0;

	// suggest an additional allocaton that doubles the old capacity while also accomodating incoming bytes
	cy_uint additional_allocation = get_capacity_dstring(str_p) + incoming_byte_count;
	if(additional_allocation > get_capacity_dstring(str_p) && additional_allocation > incoming_byte_count
	 && expand_dstring(str_p, additional_allocation))
			return 1;

	// else just accomodate the incoming_bytes
	additional_allocation = incoming_byte_count - get_unused_capacity_dstring(str_p);
	if(expand_dstring(str_p, additional_allocation))
		return 1;
	
	return 0;
}

int concatenate_dstring(dstring* str_p1, const dstring* str_p2)
{
	// you cannot concatenate to a POINT_DSTR
	if(get_dstr_type(str_p1->type_n_SS_size) == POINT_DSTR)
		return 0;

	// concatenate only if we are able to make room for the incoming bytes
	if(!make_room_for_bytes_in_dstring(str_p1, get_char_count_dstring(str_p2)))
		return 0;

	const char* str2_data = get_byte_array_dstring(str_p2);
	cy_uint str2_size = get_char_count_dstring(str_p2);

	char* str1_data = get_byte_array_dstring(str_p1);
	cy_uint str1_size = get_char_count_dstring(str_p1);

	// do appending as normal now
	memory_move(str1_data + str1_size, str2_data, str2_size);
	increment_char_count_dstring(str_p1, str2_size);

	return 1;
}

int concatenate_char(dstring* str_p1, char chr)
{
	return concatenate_dstring(str_p1, &get_dstring_pointing_to(&chr, 1));
}

void to_lowercase_dstring(dstring* str_p)
{
	char* str_data = get_byte_array_dstring(str_p);
	cy_uint str_size = get_char_count_dstring(str_p);

	while(str_size)
	{
		*str_data = to_lowercase_char(*str_data);
		str_data++;
		str_size--;
	}
}

void to_uppercase_dstring(dstring* str_p)
{
	char* str_data = get_byte_array_dstring(str_p);
	cy_uint str_size = get_char_count_dstring(str_p);

	while(str_size)
	{
		*str_data = to_uppercase_char(*str_data);
		str_data++;
		str_size--;
	}
}

int compare_dstring(const dstring* str_p1, const dstring* str_p2)
{
	const char* str1_data = get_byte_array_dstring(str_p1);
	cy_uint str1_size = get_char_count_dstring(str_p1);

	const char* str2_data = get_byte_array_dstring(str_p2);
	cy_uint str2_size = get_char_count_dstring(str_p2);

	if(str1_size == str2_size)
		return memory_compare(str1_data, str2_data, str1_size);

	cy_uint min_size = min(str1_size, str2_size);
	int cmp = memory_compare(str1_data, str2_data, min_size);

	if(cmp != 0)
		return cmp;
	if(min_size == str1_size)
		return -1;
	return 1;
}

static int memory_case_compare(const char* data1, const char* data2, cy_uint size)
{
	if(data1 == data2 || size == 0)
		return 0;
	while(size)
	{
		char c1 = to_lowercase_char((*data1));
		char c2 = to_lowercase_char((*data2));
		if(c1 > c2)
			return 1;
		else if(c1 < c2)
			return -1;
		else
		{
			data1++;
			data2++;
		}
		size--;
	}
	return 0;
}

int case_compare_dstring(const dstring* str_p1, const dstring* str_p2)
{
	const char* str1_data = get_byte_array_dstring(str_p1);
	cy_uint str1_size = get_char_count_dstring(str_p1);

	const char* str2_data = get_byte_array_dstring(str_p2);
	cy_uint str2_size = get_char_count_dstring(str_p2);

	if(str1_size == str2_size)
		return memory_case_compare(str1_data, str2_data, str1_size);

	cy_uint min_size = min(str1_size, str2_size);
	int cmp = memory_case_compare(str1_data, str2_data, min_size);

	if(cmp != 0)
		return cmp;
	if(min_size == str1_size)
		return -1;
	return 1;
}

void sprint_chars(dstring* str_p, char chr, cy_uint count)
{
	for(cy_uint i = 0; i < count; i++)
		snprintf_dstring(str_p, "%c", chr);
}

cy_uint ltrim_dstring(dstring* str_p)
{
	char* str_data = get_byte_array_dstring(str_p);
	cy_uint str_size = get_char_count_dstring(str_p);

	cy_uint whitespaces_to_remove = 0;
	for(; whitespaces_to_remove < str_size && is_whitespace_char(str_data[whitespaces_to_remove]); whitespaces_to_remove++);

	discard_chars_from_front_dstring(str_p, whitespaces_to_remove);

	return whitespaces_to_remove;
}

cy_uint rtrim_dstring(dstring* str_p)
{
	char* str_data = get_byte_array_dstring(str_p);
	cy_uint str_size = get_char_count_dstring(str_p);

	cy_uint whitespaces_to_remove = 0;
	for(cy_uint i = str_size - 1; i != -1 && is_whitespace_char(str_data[i]); i--, whitespaces_to_remove++);

	discard_chars_from_back_dstring(str_p, whitespaces_to_remove);

	return whitespaces_to_remove;
}

cy_uint trim_dstring(dstring* str_p)
{
	return ltrim_dstring(str_p) + rtrim_dstring(str_p);
}

int get_unsigned_long_long_int_from_dstring(const dstring* str_p, unsigned int radix, unsigned long long int* result)
{
	if(radix < 2 || radix > 36)
		return 0;

	const char* str_data = get_byte_array_dstring(str_p);
	cy_uint str_size = get_char_count_dstring(str_p);

	(*result) = 0;

	for(cy_uint i = 0; i < str_size; i++)
	{
		unsigned int digit = get_digit_from_char(str_data[i], radix);
		if(digit == -1)
			return 0;
		(*result) *= radix;
		(*result) += digit;
	}

	return 1;
}

// all method below this point are non permanent methods of dstring of cutlery library

#include<stdio.h>
#include<stdarg.h>

int vsnprintf_dstring(dstring* str_p, const char* cstr_format, va_list var_args)
{
	// a POINT_DSTR can not be concatenated to
	if(get_dstring_type(str_p) == POINT_DSTR)
		return 0;

	va_list var_args_dummy;

	va_copy(var_args_dummy, var_args);
	// this is the additional size that will be occupied by the final dstring over the current occupied size
	cy_uint size_extra_req = vsnprintf(NULL, 0, cstr_format, var_args_dummy) + 1; // yes this last ( + 1) is important, remove it and live in misery
	va_end(var_args_dummy);

	// append only if we are able to make room for the incoming size_extra_req bytes
	if(!make_room_for_bytes_in_dstring(str_p, size_extra_req))
		return 0;

	// call all accessor methods of dstring again after expanding
	char* str_data = get_byte_array_dstring(str_p);
	cy_uint str_size = get_char_count_dstring(str_p);

	size_extra_req = vsnprintf(str_data + str_size, get_unused_capacity_dstring(str_p), cstr_format, var_args);
	increment_char_count_dstring(str_p, size_extra_req);

	return 1;
}

int snprintf_dstring(dstring* str_p, const char* cstr_format, ...)
{
	va_list var_args;
	va_start(var_args, cstr_format);

	int res = vsnprintf_dstring(str_p, cstr_format, var_args);

	va_end(var_args);

	return res;
}

#include<string.h>

int concatenate_c_string(dstring* str_p1, const char* c_string)
{
	return concatenate_dstring(str_p1, &get_dstring_pointing_to_cstring(c_string));
}