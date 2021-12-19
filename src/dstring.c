#include<dstring.h>

#include<cutlery_stds.h>
#include<memory_allocator_interface.h>

// default memory allocator for dstring is the STD_C memory allocator
memory_allocator DSTRING_mem_alloc = &STD_C_memory_allocator;

// BASE METHODS START

void init_dstring(dstring* str_p, const char* data, unsigned int data_size)
{
	str_p->bytes_occupied = data_size;
	str_p->bytes_allocated = data_size;
	str_p->byte_array = allocate(DSTRING_mem_alloc, data_size);
	if(data != NULL && data_size > 0)
		memory_move(str_p->byte_array, data, data_size);
}

void init_empty_dstring(dstring* str_p, unsigned int capacity)
{
	str_p->bytes_occupied = 0;
	str_p->bytes_allocated = capacity;
	str_p->byte_array = allocate(DSTRING_mem_alloc, capacity);
}

char* get_byte_array_dstring(const dstring* str_p)
{
	return str_p->byte_array;
}

unsigned int get_char_count_dstring(const dstring* str_p)
{
	return str_p->bytes_occupied;
}

int increment_char_count_dstring(dstring* str_p, unsigned int increment_by)
{
	if(increment_by > get_unused_capacity_dstring(str_p))
		return 0;
	str_p->bytes_occupied += increment_by;
	return 1;
}

int decrement_char_count_dstring(dstring* str_p, unsigned int decrement_by)
{
	if(decrement_by > get_char_count_dstring(str_p))
		return 0;
	str_p->bytes_occupied -= decrement_by;
	return 1;
}

unsigned int get_capacity_dstring(const dstring* str_p)
{
	return str_p->bytes_allocated;
}

unsigned int get_unused_capacity_dstring(const dstring* str_p)
{
	return str_p->bytes_allocated - str_p->bytes_occupied;
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
	char* str_data = get_byte_array_dstring(str_p);
	unsigned int str_size = get_char_count_dstring(str_p);
	unsigned int str_capacity = get_capacity_dstring(str_p);

	unsigned int new_allocated_size = str_size + additional_allocation;

	// if expansion is not resulting in expansion, then return 0 (failure)
	if(new_allocated_size <= str_capacity)
		return 0;

	char* new_byte_array = reallocate(DSTRING_mem_alloc, str_data, str_capacity, new_allocated_size);

	// failed allocation
	if(new_byte_array == NULL && new_allocated_size > 0)
		return 0;

	// update with new allocation array and its size
	str_p->byte_array = new_byte_array;
	str_p->bytes_allocated = new_allocated_size;
	return 1;
}

int shrink_dstring(dstring* str_p)
{
	char* str_data = get_byte_array_dstring(str_p);
	unsigned int str_size = get_char_count_dstring(str_p);
	unsigned int str_capacity = get_capacity_dstring(str_p);

	if(str_capacity <= str_size)
		return 0;

	unsigned int new_allocated_size = str_size;
	void* new_byte_array = reallocate(DSTRING_mem_alloc, str_data, str_capacity, new_allocated_size);

	// failed allocation
	if(new_byte_array == NULL && new_allocated_size > 0)
		return 0;

	str_p->byte_array = new_byte_array;
	str_p->bytes_allocated = new_allocated_size;
	return 1;
}

// BASE METHODS END

void init_copy_dstring(dstring* str_p, const dstring* init_copy_from)
{
	init_dstring(str_p, get_byte_array_dstring(init_copy_from), get_char_count_dstring(init_copy_from));
}

dstring get_dstring(const char* data, unsigned int data_size)
{
	dstring str;
	init_dstring(&str, data, data_size);
	return str;
}

dstring get_copy_dstring(const dstring* init_copy_from)
{
	dstring str;
	init_copy_dstring(&str, init_copy_from);
	return str;
}

void concatenate_dstring(dstring* str_p1, const dstring* str_p2)
{
	const char* str2_data = get_byte_array_dstring(str_p2);
	unsigned int str2_size = get_char_count_dstring(str_p2);

	if(str2_data != NULL && str2_size > 0)
	{
		// check if new data could fit, without expansion, else expand the dstring
		if(str2_size > get_unused_capacity_dstring(str_p1))
			expand_dstring(str_p1, 2 * str2_size);

		char* str1_data = get_byte_array_dstring(str_p1);
		unsigned int str1_size = get_char_count_dstring(str_p1);

		// do appending as normal now
		memory_move(str1_data + str1_size, str2_data, str2_size);
		increment_char_count_dstring(str_p1, str2_size);
	}
}

// converts a char to uppercase macro
#define toLowercaseChar(c) ((('A' <= (c)) && ((c) <= 'Z')) ? ((c) - 'A' + 'a') : (c))

void toLowercase(dstring* str_p)
{
	char* str_data = get_byte_array_dstring(str_p);
	unsigned int str_size = get_char_count_dstring(str_p);

	while(str_size)
	{
		*str_data = toLowercaseChar(*str_data);
		str_data++;
		str_size--;
	}
}

// converts a char to uppercase macro
#define toUppercaseChar(c) ((('a' <= (c)) && ((c) <= 'z')) ? ((c) - 'a' + 'A') : (c))

void toUppercase(dstring* str_p)
{
	char* str_data = get_byte_array_dstring(str_p);
	unsigned int str_size = get_char_count_dstring(str_p);

	while(str_size)
	{
		*str_data = toUppercaseChar(*str_data);
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

void snprintf_dstring(dstring* str_p, const char* cstr_format, ...)
{
	va_list var_args, var_args_dummy;
	va_start(var_args, cstr_format);

	va_copy(var_args_dummy, var_args);
	// this is the additional size that will be occupied by the final dstring over the current occupied size
	unsigned int size_extra_req = vsnprintf(NULL, 0, cstr_format, var_args_dummy) + 1;
	va_end(var_args_dummy);

	// expand str_p as needed
	if(size_extra_req > get_unused_capacity_dstring(str_p))
		expand_dstring(str_p, 2 * size_extra_req);

	// call all accessor methods of dstring again after expanding
	char* str_data = get_byte_array_dstring(str_p);
	unsigned int str_size = get_char_count_dstring(str_p);

	size_extra_req = vsnprintf(str_data + str_size, get_unused_capacity_dstring(str_p), cstr_format, var_args);
	increment_char_count_dstring(str_p, size_extra_req);

	va_end(var_args);
}