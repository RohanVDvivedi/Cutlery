#ifndef DSTRING_H
#define DSTRING_H

typedef struct dstring dstring;
struct dstring
{
	// actual string it does not terminate with '\0'
	char* cstring;

	// number of bytes in the string
	unsigned int bytes_occupied;

	// this are the number of bytes that are allocated
	// i.e you may reference cstring for this many no of bytes until seg fault
	// yet this value is 0, for slize or slices
	// slices are dstrings that point to other cstrings, and have cstring pointing to theinr data
	// a slice could be used to represent a substring of a dstring 
	unsigned int bytes_allocated;
};

dstring* get_dstring_data(const char* data, unsigned int data_size);
void init_dstring_slize(dstring* slize, char* data, unsigned int data_size);
void init_dstring(dstring* str_p, const char* cstr_p);
void init_dstring_data(dstring* str_p, const char* data, unsigned int data_size);

void convert_slize_to_dstring(dstring* slize);

void make_dstring_empty(dstring* str_p);

int compare_dstring(const dstring* str_p1, const dstring* str_p2);
int compare_dstring_cstring(const dstring* str_p1, const char* str_p2);
int case_compare_dstring(const dstring* str_p1, const dstring* str_p2);
int case_compare_dstring_cstring(const dstring* str_p1, const char* str_p2);

// KMP (Knuth–Morris–Pratt) implementation for substring position in a given string
// returns pointer to the position of first substring match, else NULL
char* contains_dstring(const dstring* str, const dstring* sub_str);
char* contains_cstring(const dstring* str, const char* sub_str);

int is_prefix(const dstring* str_p1, const char* str_p2);

void expand_dstring(dstring* str_p, unsigned int additional_allocation);
void appendn_to_dstring(dstring* str_p, const char* data, unsigned int data_size);
void append_to_dstring(dstring* str_p, const char* cstr_p);
void append_to_dstring_formatted(dstring* str_p, const char* cstr_format, ...);
void concatenate_dstring(dstring* str_p1, const dstring* str_p2);

void toLowercase(dstring* str_p);
void toUppercase(dstring* str_p);

void display_dstring(const dstring* str_p);

void deinit_dstring(dstring* str_p);
void delete_dstring(dstring* str_p);

#endif