#ifndef DSTRING_H
#define DSTRING_H

#include<memory_allocator_interface.h>

typedef enum dstring_type dstring_type;
enum dstring_type
{
	LARGE_DSTR = 0b11,	// dstring that is allocated at byte_array (using size bytes_occipied from bytes_occupied)
	SHORT_DSTR = 0b00,  // dstring that is stored in the dstring struct itself after type_n_SS_size (does not require allocation)
	POINT_DSTR = 0b01,  // dstring that uses byte_array and bytes_occupied, but points to another dstring's allocation (does not manage its own memory)
};

typedef struct dstring dstring;
struct dstring
{
	// type of dstring and bytes_occupied by the short string
	unsigned char type_n_SS_size;

	// number of bytes in the byte_array
	cy_uint bytes_occupied;

	// maxumum number of bytes at byte_array
	cy_uint bytes_allocated;

	// non null terminating string
	char* byte_array;
};

// BASE METHODS START
// only BASE methods are allowed to directly access the dstring struct

dstring_type get_dstring_type(const dstring* str_p);

// failure returns 0, which implies failure to allocate memory
// you can safely assume that this function will never fail with capacity parameter = 0
int init_empty_dstring(dstring* str_p, cy_uint capacity);

#define get_dstring_pointing_to(data, data_size) \
	((const dstring){.type_n_SS_size = POINT_DSTR, .byte_array = ((char*)(data)), .bytes_occupied = (data_size), .bytes_allocated = 0})
#define get_dstring_pointing_to_dstring(dstr) get_dstring_pointing_to(get_byte_array_dstring((dstr)), get_char_count_dstring((dstr)))
#define get_dstring_pointing_to_cstring(cstr) get_dstring_pointing_to((cstr), strlen((cstr)))
#define get_dstring_pointing_to_literal_cstring(cstr) get_dstring_pointing_to((cstr), sizeof((cstr))-1)

void deinit_dstring(dstring* str_p);

char* get_byte_array_dstring(const dstring* str_p);
cy_uint get_char_count_dstring(const dstring* str_p);
int increment_char_count_dstring(dstring* str_p, cy_uint increment_by);
int decrement_char_count_dstring(dstring* str_p, cy_uint decrement_by);
cy_uint get_capacity_dstring(const dstring* str_p);
cy_uint get_unused_capacity_dstring(const dstring* str_p);

void make_dstring_empty(dstring* str_p);

// increases the size of dstring by additional_size number of bytes
int expand_dstring(dstring* str_p, cy_uint additional_size);
// shrinks dstring to its bytes_occupied size
int shrink_dstring(dstring* str_p);

// BASE METHODS END

int init_dstring(dstring* str_p, const char* data, cy_uint data_size);
int init_copy_dstring(dstring* str_p, const dstring* init_copy_from);

dstring new_dstring(const char* data, cy_uint data_size);
dstring new_copy_dstring(const dstring* init_copy_from);

// checks if the dstring is empty, returns 1 if it is empty
// equivalent to get_char_count(str_p) == 0
int is_empty_dstring(const dstring* str_p);

// compare 2 dstrings
int compare_dstring(const dstring* str_p1, const dstring* str_p2);
int case_compare_dstring(const dstring* str_p1, const dstring* str_p2);

// discard chars in dstring between start_index and last_index both inclusive
// it won't shrink the dstring
// this function will fail for POINT_DSTR
int discard_chars_dstring(dstring* str_p, cy_uint start_index, cy_uint last_index);

// this function can be called on all types of dstrings
int discard_chars_from_front_dstring(dstring* str_p, cy_uint bytes_to_discard);
int discard_chars_from_back_dstring(dstring* str_p, cy_uint bytes_to_discard);

// concatenates str_p2 to str_p1, this functions fail only if memory allocation fails (a worrysome situation)
int concatenate_dstring(dstring* str_p1, const dstring* str_p2);
int concatenate_c_string(dstring* str_p1, const char* c_string);
int concatenate_char(dstring* str_p1, char chr);

// append count number of chr char at the end of the dstring str_p
void sprint_chars(dstring* str_p, char chr, cy_uint count);

// marcos for io using dstring
#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)
#define printf_dstring_format				STRINGIFY(%.*s)
#define printf_dstring_params(str_p)		((int)get_char_count_dstring((str_p))), get_byte_array_dstring((str_p))
#define printf_dstring(str_p)				((get_byte_array_dstring((str_p)) != NULL && get_char_count_dstring((str_p)) > 0) ? printf(printf_dstring_format, printf_dstring_params((str_p))) : 0)

#include<dstring_char.h>

void to_lowercase_dstring(dstring* str_p);
void to_uppercase_dstring(dstring* str_p);

// trim white spaces from dstring, returns the number of white spaces removed
// they won't shrink the dstring
cy_uint ltrim_dstring(dstring* str_p);
cy_uint rtrim_dstring(dstring* str_p);
cy_uint trim_dstring(dstring* str_p);

// returns 1 in successfull conversion
// it will return 0 if a character other than a radix character is found
// this function does not check for overflow, hence it must be you who takes care of that, by checking the length of the string provided
int get_unsigned_long_long_int_from_dstring(const dstring* str_p, unsigned int radix, unsigned long long int* result);

#include<dstring_substring.h>
#include<dstring_diff.h>
#include<dstring_index_accessed_interface.h>

// This is the global memory allocator that will be used for contents 
// of every dstring that ever gets created/modified
extern memory_allocator DSTRING_mem_alloc;

#include<stdarg.h>

// append_to_dstring_formatted is a snprintf implementation for dstrings
// fails if the memory allocation calls fail
int vsnprintf_dstring(dstring* str_p, const char* cstr_format, va_list args);
int snprintf_dstring(dstring* str_p, const char* cstr_format, ...);

#endif
