#include<dstring.h>

#include<cutlery_stds.h>
#include<memory_allocator_interface.h>

// default memory allocator for dstring is the STD_C memory allocator
memory_allocator DSTRING_mem_alloc = &STD_C_memory_allocator;

void init_dstring(dstring* str_p, const char* data, unsigned int data_size)
{
	str_p->bytes_occupied = data_size;
	str_p->bytes_allocated = data_size;
	str_p->byte_array = allocate(DSTRING_mem_alloc, data_size);
	if(data != NULL && data_size > 0)
		memory_move(str_p->byte_array, data, data_size);
}

void init_empty_dstring(dstring* str_p, unsigned int init_size)
{
	str_p->bytes_occupied = 0;
	str_p->bytes_allocated = init_size;
	str_p->byte_array = allocate(DSTRING_mem_alloc, init_size);
}

dstring get_dstring(const char* data, unsigned int data_size)
{
	dstring str;
	init_dstring(&str, data, data_size);
	return str;
}

char* get_byte_array_dstring(const dstring* str_p)
{
	return str_p->byte_array;
}

unsigned int get_char_count_dstring(const dstring* str_p)
{
	return str_p->bytes_occupied;
}

unsigned int get_capacity_dstring(const dstring* str_p)
{
	return str_p->bytes_allocated;
}

void make_dstring_empty(dstring* str_p)
{
	str_p->bytes_occupied = 0;
}

void deinit_dstring(dstring* str_p)
{
	if(str_p->bytes_allocated > 0 && str_p->byte_array != NULL)
		deallocate(DSTRING_mem_alloc, str_p->byte_array, str_p->bytes_allocated);
	str_p->byte_array = NULL;
	str_p->bytes_allocated = 0;
	str_p->bytes_occupied = 0;
}

int expand_dstring(dstring* str_p, unsigned int additional_allocation)
{
	unsigned int new_allocated_size = get_char_count_dstring(str_p) + additional_allocation;

	// if expansion is not resulting in expansion
	if(new_allocated_size <= get_capacity_dstring(str_p))
		return 0;

	void* new_byte_array = reallocate(DSTRING_mem_alloc, get_byte_array_dstring(str_p), get_capacity_dstring(str_p), new_allocated_size);

	// failed allocation
	if(new_byte_array == NULL && new_allocated_size > 0)
		return 0;

	str_p->byte_array = new_byte_array;
	str_p->bytes_allocated = new_allocated_size;
	return 1;
}

int shrink_dstring(dstring* str_p)
{
	if(get_capacity_dstring(str_p) <= get_char_count_dstring(str_p))
		return 0;

	unsigned int new_allocated_size = get_char_count_dstring(str_p);
	void* new_byte_array = reallocate(DSTRING_mem_alloc, get_byte_array_dstring(str_p), get_capacity_dstring(str_p), new_allocated_size);

	// failed allocation
	if(new_byte_array == NULL && new_allocated_size > 0)
		return 0;

	str_p->byte_array = new_byte_array;
	str_p->bytes_allocated = new_allocated_size;
	return 1;
}

void concatenate_dstring(dstring* str_p1, const dstring* str_p2)
{
	if(get_byte_array_dstring(str_p2) != NULL && get_char_count_dstring(str_p2) > 0)
	{
		// check if new data could fit, without expansion, else expand the dstring
		if(get_char_count_dstring(str_p1) + get_char_count_dstring(str_p2) > get_capacity_dstring(str_p1))
			expand_dstring(str_p1, get_char_count_dstring(str_p1) + 2 * get_char_count_dstring(str_p2));

		// do appending as normal now
		memory_move(get_byte_array_dstring(str_p1) + get_char_count_dstring(str_p1), get_byte_array_dstring(str_p2), get_char_count_dstring(str_p2));
		str_p1->bytes_occupied += get_char_count_dstring(str_p2);
	}
}

// converts a char to uppercase macro
#define toLowercaseChar(c) ((('A' <= (c)) && ((c) <= 'Z')) ? ((c) - 'A' + 'a') : (c))

void toLowercase(dstring* str_p)
{
	char* stemp = get_byte_array_dstring(str_p);
	while(stemp < get_byte_array_dstring(str_p) + get_char_count_dstring(str_p))
	{
		*stemp = toLowercaseChar(*stemp);
		stemp++;
	}
}

// converts a char to uppercase macro
#define toUppercaseChar(c) ((('a' <= (c)) && ((c) <= 'z')) ? ((c) - 'a' + 'A') : (c))

void toUppercase(dstring* str_p)
{
	char* stemp = get_byte_array_dstring(str_p);
	while(stemp < get_byte_array_dstring(str_p) + get_char_count_dstring(str_p))
	{
		*stemp = toUppercaseChar(*stemp);
		stemp++;
	}
}

int compare_dstring(const dstring* str_p1, const dstring* str_p2)
{
	if(get_char_count_dstring(str_p1) == get_char_count_dstring(str_p2))
		return memory_compare(get_byte_array_dstring(str_p1), get_byte_array_dstring(str_p2), get_char_count_dstring(str_p2));

	unsigned int min_size = (get_char_count_dstring(str_p1) < get_char_count_dstring(str_p2)) ? get_char_count_dstring(str_p1) : get_char_count_dstring(str_p2);
	int cmp = memory_compare(get_byte_array_dstring(str_p1), get_byte_array_dstring(str_p2), min_size);

	if(cmp != 0)
		return cmp;
	if(min_size == get_char_count_dstring(str_p1))
		return -1;
	return 1;
}

static int memory_case_compare(const char* data1, const char* data2, unsigned int size)
{
	if(data1 == data2 || size == 0)
		return 0;
	while(size)
	{
		char c1 = toLowercaseChar((*data1));
		char c2 = toLowercaseChar((*data2));
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
	if(get_char_count_dstring(str_p1) == get_char_count_dstring(str_p2))
		return memory_case_compare(get_byte_array_dstring(str_p1), get_byte_array_dstring(str_p2), get_char_count_dstring(str_p2));

	unsigned int min_size = (get_char_count_dstring(str_p1) < get_char_count_dstring(str_p2)) ? get_char_count_dstring(str_p1) : get_char_count_dstring(str_p2);
	int cmp = memory_case_compare(get_byte_array_dstring(str_p1), get_byte_array_dstring(str_p2), min_size);

	if(cmp != 0)
		return cmp;
	if(min_size == get_char_count_dstring(str_p1))
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

void snprintf_dstring(dstring* str_p, const char* cstr_format, ...)
{
	va_list var_args, var_args_dummy;
	va_start(var_args, cstr_format);

	va_copy(var_args_dummy, var_args);
	// this is the additional size that will be occupied by the final dstring over the current occupied size
	unsigned int size_extra_req = vsnprintf(NULL, 0, cstr_format, var_args_dummy) + 1;
	va_end(var_args_dummy);

	// expand str_p as needed
	if(size_extra_req + get_char_count_dstring(str_p) > get_capacity_dstring(str_p))
		expand_dstring(str_p, get_char_count_dstring(str_p) + 2 * size_extra_req);

	str_p->bytes_occupied += vsnprintf(get_byte_array_dstring(str_p) + get_char_count_dstring(str_p), get_capacity_dstring(str_p) - get_char_count_dstring(str_p), cstr_format, var_args);
	va_end(var_args);
}