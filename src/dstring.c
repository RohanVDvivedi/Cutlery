#include<dstring.h>

#include<cutlery_stds.h>
#include<memory_allocator_interface.h>

#include<stdio.h>
#include<stdarg.h>
#include<string.h>

// default memory allocator for dstring is the STD_C memory allocator
memory_allocator DSTRING_mem_alloc = STD_C_mem_allocator;

void init_dstring(dstring* str_p, const char* data, unsigned int data_size)
{
	str_p->bytes_occupied = data_size;
	str_p->bytes_allocated = data_size;
	str_p->cstring = allocate(DSTRING_mem_alloc, data_size);
	if(data != NULL && data_size > 0)
		memory_move(str_p->cstring, data, data_size);
}

void init_empty_dstring(dstring* str_p, unsigned int init_size)
{
	str_p->bytes_occupied = 0;
	str_p->bytes_allocated = init_size;
	str_p->cstring = allocate(DSTRING_mem_alloc, init_size);
}

void make_dstring_empty(dstring* str_p)
{
	str_p->bytes_occupied = 0;
}

int compare_dstring(const dstring* str_p1, const dstring* str_p2)
{
	if(str_p1->bytes_occupied == str_p2->bytes_occupied)
		return strncmp(str_p1->cstring, str_p2->cstring, str_p1->bytes_occupied);

	unsigned int min_size = (str_p1->bytes_occupied < str_p2->bytes_occupied) ? str_p1->bytes_occupied : str_p2->bytes_occupied;
	int cmp = strncmp(str_p1->cstring, str_p2->cstring, min_size);

	if(cmp != 0)
		return cmp;
	if(min_size == str_p1->bytes_occupied)
		return -1;
	return 1;
}
int case_compare_dstring(const dstring* str_p1, const dstring* str_p2)
{
	if(str_p1->bytes_occupied == str_p2->bytes_occupied)
		return strncasecmp(str_p1->cstring, str_p2->cstring, str_p1->bytes_occupied);

	unsigned int min_size = (str_p1->bytes_occupied < str_p2->bytes_occupied) ? str_p1->bytes_occupied : str_p2->bytes_occupied;
	int cmp = strncasecmp(str_p1->cstring, str_p2->cstring, min_size);

	if(cmp != 0)
		return cmp;
	if(min_size == str_p1->bytes_occupied)
		return -1;
	return 1;
}

void expand_dstring(dstring* str_p, unsigned int additional_allocation)
{
	dstring expanded_dstring = {.bytes_occupied = str_p->bytes_occupied, .bytes_allocated = str_p->bytes_occupied + additional_allocation};

	// if expanded dstring does not result in real expansion, than just exit
	if(expanded_dstring.bytes_allocated <= str_p->bytes_allocated)
		return;

	expanded_dstring.cstring = allocate(DSTRING_mem_alloc, expanded_dstring.bytes_allocated);
	memory_move(expanded_dstring.cstring, str_p->cstring, str_p->bytes_occupied);

	deinit_dstring(str_p);
	(*str_p) = expanded_dstring;
}

int shrink_dstring(dstring* str_p)
{
	if(str_p->bytes_allocated <= str_p->bytes_occupied)
		return 0;

	unsigned int new_allocated_size = str_p->bytes_occupied;
	void* new_cstring = reallocate(DSTRING_mem_alloc, str_p->cstring, str_p->bytes_allocated, new_allocated_size);

	if(new_cstring != NULL)
	{
		str_p->cstring = new_cstring;
		str_p->bytes_allocated = new_allocated_size;
		return 1
	}
	else
		return 0;
}

void concatenate_dstring(dstring* str_p1, const dstring* str_p2)
{
	if(str_p2->cstring != NULL && str_p2->bytes_occupied > 0)
	{
		// check if new data could fit, without expansion, else expand the dstring
		if(str_p1->bytes_occupied + str_p2->bytes_occupied > str_p1->bytes_allocated)
			expand_dstring(str_p1, str_p1->bytes_occupied + 2 * str_p2->bytes_occupied);

		// do appending as normal now
		memory_move(str_p1->cstring + str_p1->bytes_occupied, str_p2->cstring, str_p2->bytes_occupied);
		str_p1->bytes_occupied += str_p2->bytes_occupied;
	}
}

void snprintf_dstring(dstring* str_p, const char* cstr_format, ...)
{
	va_list var_args, var_args_dummy;
	va_start(var_args, cstr_format);

	va_copy(var_args_dummy, var_args);
	// this is the additional size that will be occupied by the final dstring over the current occupied size
	unsigned int size_extra_req = vsnprintf(NULL, 0, cstr_format, var_args_dummy) + 1;
	va_end(var_args_dummy);

	// expand str_p as needed
	if(size_extra_req + str_p->bytes_occupied > str_p->bytes_allocated)
		expand_dstring(str_p, str_p->bytes_occupied + 2 * size_extra_req);

	str_p->bytes_occupied += vsnprintf(str_p->cstring + str_p->bytes_occupied, str_p->bytes_allocated - str_p->bytes_occupied, cstr_format, var_args);
	va_end(var_args);
}

void sprint_chars(dstring* str_p, char chr, unsigned int count)
{
	for(unsigned int i = 0; i < count; i++)
		snprintf_dstring(str_p, "%c", chr);
}

#define toLowercaseChar(c) ((('A' <= (c)) && ((c) <= 'Z')) ? ((c) - 'A' + 'a') : (c))
void toLowercase(dstring* str_p)
{
	char* stemp = str_p->cstring;
	while(stemp < str_p->cstring + str_p->bytes_occupied)
	{
		*stemp = toLowercaseChar(*stemp);
		stemp++;
	}
}

#define toUppercaseChar(c) ((('a' <= (c)) && ((c) <= 'z')) ? ((c) - 'a' + 'A') : (c))
void toUppercase(dstring* str_p)
{
	char* stemp = str_p->cstring;
	while(stemp < str_p->cstring + str_p->bytes_occupied)
	{
		*stemp = toUppercaseChar(*stemp);
		stemp++;
	}
}

void deinit_dstring(dstring* str_p)
{
	if(str_p->bytes_allocated > 0 && str_p->cstring != NULL)
		deallocate(DSTRING_mem_alloc, str_p->cstring, str_p->bytes_allocated);
	str_p->cstring = NULL;
	str_p->bytes_allocated = 0;
	str_p->bytes_occupied = 0;
}