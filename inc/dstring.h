#ifndef DSTRING_H
#define DSTRING_H

#include<memory_allocator_interface.h>

typedef struct dstring dstring;
struct dstring
{
	// number of bytes in the byte_array
	unsigned int bytes_occupied;

	// maxumum number of bytes at byte_array
	unsigned int bytes_allocated;

	// non null terminating string
	char* byte_array;
};

void init_dstring(dstring* str_p, const char* data, unsigned int data_size);
void init_empty_dstring(dstring* str_p, unsigned int init_size);

dstring get_dstring(const char* data, unsigned int data_size);

#define get_literal_dstring(data, data_size) \
	((const dstring){.byte_array = ((const char*)(data)), .bytes_occupied = (data_size), .bytes_allocated = 0})
#define get_literal_cstring(cstr) get_literal_dstring((cstr), strlen((cstr)))

char* get_byte_array_dstring(const dstring* str_p);
unsigned int get_char_count_dstring(const dstring* str_p);
unsigned int get_capacity_dstring(const dstring* str_p);

void make_dstring_empty(dstring* str_p);

int compare_dstring(const dstring* str_p1, const dstring* str_p2);
int case_compare_dstring(const dstring* str_p1, const dstring* str_p2);

// increases the size of dstring by additional_size number of bytes
int expand_dstring(dstring* str_p, unsigned int additional_size);
// shrinks dstring to its bytes_occupied size
int shrink_dstring(dstring* str_p);

// concatenates str_p2 to str_p1
void concatenate_dstring(dstring* str_p1, const dstring* str_p2);

// append_to_dstring_formatted is a snprintf implementation for dstrings
void snprintf_dstring(dstring* str_p, const char* cstr_format, ...);

// append count number of chr char at the end of the dstring str_p
void sprint_chars(dstring* str_p, char chr, unsigned int count);

// marcos for io using dstring
#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)
#define printf_dstring_format				STRINGIFY(%.*s)
#define printf_dstring_params(str_p)		get_char_count_dstring((str_p)), get_byte_array_dstring((str_p))
#define printf_dstring(str_p)				((get_byte_array_dstring((str_p)) != NULL && get_char_count_dstring((str_p)) > 0) ? printf(printf_dstring_format, printf_dstring_params(str_p)) : 0)

void toLowercase(dstring* str_p);
void toUppercase(dstring* str_p);

#include<dstring_substring.h>
#include<dstring_diff.h>

void deinit_dstring(dstring* str_p);

// This is the global memory allocator that will be used for contents 
// of every dstring that ever gets created/modified
extern memory_allocator DSTRING_mem_alloc;

#endif