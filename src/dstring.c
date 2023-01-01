#include<dstring.h>

#include<cutlery_stds.h>
#include<memory_allocator_interface.h>

// default memory allocator for dstring is the STD_C memory allocator
memory_allocator DSTRING_mem_alloc = &STD_C_memory_allocator;

// accessor for dstring attribute type_n_SS_size
#define get_dstr_type(type_n_SS_size)		((type_n_SS_size) & 0x3)
#define get_dstr_SS_size(type_n_SS_size)	(((type_n_SS_size) >> 2) & 0x3f)

#define set_dstr_SS_size(type_n_SS_size, SS_size)	(type_n_SS_size) = ((((SS_size) << 2) & 0xfc) | ((type_n_SS_size) & 0x3))

#define SS_data_offset		((unsigned long int)((&(((dstring*)0)->type_n_SS_size))+sizeof(unsigned char)))
#define SS_capacity			(sizeof(dstring)-SS_data_offset)

// BASE METHODS START

void init_empty_dstring(dstring* str_p, unsigned int capacity)
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
	if(get_dstr_type(str_p->type_n_SS_size) != SHORT_DSTR)
		return str_p->byte_array;
	return ((char*)str_p) + SS_data_offset;
}

unsigned int get_char_count_dstring(const dstring* str_p)
{
	if(get_dstr_type(str_p->type_n_SS_size) != SHORT_DSTR)
		return str_p->bytes_occupied;
	return get_dstr_SS_size(str_p->type_n_SS_size);
}

int increment_char_count_dstring(dstring* str_p, unsigned int increment_by)
{
	if(get_dstr_type(str_p->type_n_SS_size) == POINT_DSTR || increment_by > get_unused_capacity_dstring(str_p))
		return 0;
	if(get_dstr_type(str_p->type_n_SS_size) == SHORT_DSTR)
	{
		unsigned int new_SS_size = get_char_count_dstring(str_p) + increment_by;
		set_dstr_SS_size(str_p->type_n_SS_size, new_SS_size);
	}
	else
		str_p->bytes_occupied += increment_by;
	return 1;
}

int decrement_char_count_dstring(dstring* str_p, unsigned int decrement_by)
{
	if(get_dstr_type(str_p->type_n_SS_size) == POINT_DSTR || decrement_by > get_char_count_dstring(str_p))
		return 0;
	if(get_dstr_type(str_p->type_n_SS_size) == SHORT_DSTR)
	{
		unsigned int new_SS_size = get_char_count_dstring(str_p) - decrement_by;
		set_dstr_SS_size(str_p->type_n_SS_size, new_SS_size);
	}
	else
		str_p->bytes_occupied -= decrement_by;
	return 1;
}

void make_dstring_empty(dstring* str_p)
{
	if(get_dstr_type(str_p->type_n_SS_size) == SHORT_DSTR)
		set_dstr_SS_size(str_p->type_n_SS_size, 0);
	else
		str_p->bytes_occupied = 0;
}

unsigned int get_capacity_dstring(const dstring* str_p)
{
	if(get_dstr_type(str_p->type_n_SS_size) == SHORT_DSTR)
		return SS_capacity;
	return str_p->bytes_allocated;
}

unsigned int get_unused_capacity_dstring(const dstring* str_p)
{
	return get_capacity_dstring(str_p) - get_char_count_dstring(str_p);
}

void deinit_dstring(dstring* str_p)
{
	if(get_dstr_type(str_p->type_n_SS_size) == LARGE_DSTR && str_p->bytes_allocated > 0 && str_p->byte_array != NULL)
		deallocate(DSTRING_mem_alloc, str_p->byte_array, str_p->bytes_allocated);
	str_p->type_n_SS_size = SHORT_DSTR;
	str_p->byte_array = NULL;
	str_p->bytes_allocated = 0;
	str_p->bytes_occupied = 0;
}

int expand_dstring(dstring* str_p, unsigned int additional_allocation)
{
	// if it is a POINT_DSTR it can not be expanded or shrunk
	if(get_dstr_type(str_p->type_n_SS_size) == POINT_DSTR)
		return 0;

	char* str_data = get_byte_array_dstring(str_p);
	unsigned int str_size = get_char_count_dstring(str_p);
	unsigned int str_capacity = get_capacity_dstring(str_p);

	// compute the new capacity
	unsigned int new_capacity = str_size + additional_allocation;
	if(new_capacity < SS_capacity)
		new_capacity = SS_capacity;

	// if expansion is not resulting in expansion, then return 0 (failure)
	if(new_capacity <= str_capacity)
		return 0;

	// you might have to make a large dstring larger
	if(get_dstr_type(str_p->type_n_SS_size) == LARGE_DSTR)
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
	else if(get_dstr_type(str_p->type_n_SS_size) == SHORT_DSTR)
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
	if(get_dstr_type(str_p->type_n_SS_size) == POINT_DSTR)
		return 0;

	char* str_data = get_byte_array_dstring(str_p);
	unsigned int str_size = get_char_count_dstring(str_p);
	unsigned int str_capacity = get_capacity_dstring(str_p);

	// compute the new capacity
	unsigned int new_capacity = str_size;
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

void init_dstring(dstring* str_p, const char* data, unsigned int data_size)
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

dstring new_dstring(const char* data, unsigned int data_size)
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

int discard_chars_dstring(dstring* str_p, unsigned int start_index, unsigned int last_index)
{
	// check that the ranges of start_index and last_index are valid
	if(start_index > last_index || last_index >= get_char_count_dstring(str_p))
		return 0;

	char* str_data = get_byte_array_dstring(str_p);
	unsigned int str_size = get_char_count_dstring(str_p);

	// move the characters after the last_index to the position at start_index
	memory_move(str_data + start_index, str_data + last_index + 1, str_size - (last_index + 1));

	// decrement the char_count of the dstring
	decrement_char_count_dstring(str_p, last_index - start_index + 1);

	return 1;
}

int concatenate_dstring(dstring* str_p1, const dstring* str_p2)
{
	const char* str2_data = get_byte_array_dstring(str_p2);
	unsigned int str2_size = get_char_count_dstring(str_p2);

	if(str2_data != NULL && str2_size > 0)
	{
		// check if new data could fit, without expansion, else expand the dstring
		if(str2_size > get_unused_capacity_dstring(str_p1))
		{
			// try allocating atleast twice the original size and the size required to accomodate the new bytes
			if(expand_dstring(str_p1, get_capacity_dstring(str_p1) + str2_size))
				goto EXP_SUCC;

			// the previous call failed so now we try to callocate only the additional required bytes
			if(expand_dstring(str_p1, str2_size - get_unused_capacity_dstring(str_p1)))
				goto EXP_SUCC;

			// concatenation failed
			return 0;

			EXP_SUCC:;
		}

		char* str1_data = get_byte_array_dstring(str_p1);
		unsigned int str1_size = get_char_count_dstring(str_p1);

		// do appending as normal now
		memory_move(str1_data + str1_size, str2_data, str2_size);
		increment_char_count_dstring(str_p1, str2_size);
	}

	return 1;
}

int concatenate_char(dstring* str_p1, char chr)
{
	return concatenate_dstring(str_p1, &get_literal_dstring(&chr, 1));
}

void to_lowercase_dstring(dstring* str_p)
{
	char* str_data = get_byte_array_dstring(str_p);
	unsigned int str_size = get_char_count_dstring(str_p);

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
	unsigned int str_size = get_char_count_dstring(str_p);

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
	unsigned int str1_size = get_char_count_dstring(str_p1);

	const char* str2_data = get_byte_array_dstring(str_p2);
	unsigned int str2_size = get_char_count_dstring(str_p2);

	if(str1_size == str2_size)
		return memory_compare(str1_data, str2_data, str1_size);

	unsigned int min_size = (str1_size < str2_size) ? str1_size : str2_size;
	int cmp = memory_compare(str1_data, str2_data, min_size);

	if(cmp != 0)
		return cmp;
	if(min_size == str1_size)
		return -1;
	return 1;
}

static int memory_case_compare(const char* data1, const char* data2, unsigned int size)
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
	unsigned int str1_size = get_char_count_dstring(str_p1);

	const char* str2_data = get_byte_array_dstring(str_p2);
	unsigned int str2_size = get_char_count_dstring(str_p2);

	if(str1_size == str2_size)
		return memory_case_compare(str1_data, str2_data, str1_size);

	unsigned int min_size = (str1_size < str2_size) ? str1_size : str2_size;
	int cmp = memory_case_compare(str1_data, str2_data, min_size);

	if(cmp != 0)
		return cmp;
	if(min_size == str1_size)
		return -1;
	return 1;
}

void sprint_chars(dstring* str_p, char chr, unsigned int count)
{
	for(unsigned int i = 0; i < count; i++)
		snprintf_dstring(str_p, "%c", chr);
}

// all method below this point are non permanent methods of dstring of cutlery library

#include<stdio.h>
#include<stdarg.h>

int snprintf_dstring(dstring* str_p, const char* cstr_format, ...)
{
	va_list var_args, var_args_dummy;
	va_start(var_args, cstr_format);

	va_copy(var_args_dummy, var_args);
	// this is the additional size that will be occupied by the final dstring over the current occupied size
	unsigned int size_extra_req = vsnprintf(NULL, 0, cstr_format, var_args_dummy) + 1; // yes this last ( + 1) is important, remove it and live in misery
	va_end(var_args_dummy);

	// expand str_p as needed
	if(size_extra_req > get_unused_capacity_dstring(str_p))
	{
		// try allocating atleast twice the original size and the size required to accomodate the new bytes
		if(expand_dstring(str_p, get_capacity_dstring(str_p) + size_extra_req))
			goto EXP_SUCC;

		// the previous call failed so now we try to callocate only the additional required bytes
		if(expand_dstring(str_p, size_extra_req - get_unused_capacity_dstring(str_p)))
			goto EXP_SUCC;

		// concatenation failed
		return 0;

		EXP_SUCC:;
	}

	// call all accessor methods of dstring again after expanding
	char* str_data = get_byte_array_dstring(str_p);
	unsigned int str_size = get_char_count_dstring(str_p);

	size_extra_req = vsnprintf(str_data + str_size, get_unused_capacity_dstring(str_p), cstr_format, var_args);
	increment_char_count_dstring(str_p, size_extra_req);

	va_end(var_args);

	return 1;
}

#include<string.h>

int concatenate_c_string(dstring* str_p1, const char* c_string)
{
	return concatenate_dstring(str_p1, &get_literal_cstring(c_string));
}