#ifndef DSTRING_H
#define DSTRING_H

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

typedef struct dstring dstring;
struct dstring
{
	// actual c string with \0 at the end
	char* cstring;

	// number of bytes the string cstring occupies, 
	unsigned int bytes_occupied;
	unsigned int bytes_allocated;
};

dstring* get_dstring(const char* cstr_p, unsigned int additional_allocation);

// initializes am already existing but empty dstring variable, can be used when you have dstring struct on the stack
// enable to use dstring on stack or in an embedded form in other data structures
void init_dstring(dstring* str_p, const char* cstr_p, unsigned int additional_allocation);

// make the dstring empty, i.e. not containing any characters except '\0'
void make_dstring_empty(dstring* str_p);

// returns 1, if str_p1 > str_p2, returns 0 if they are equal else returns -1
int compare_dstring(const dstring* str_p1, const dstring* str_p2);
int compare_dstring_cstring(const dstring* str_p1, const char* str_p2);

// checks if str_p2 is prefix of str_p1, returns non-zero if str_p2 is prefix of str_p1, else 0
int is_prefix(const dstring* str_p1, const char* str_p2);

// takes a dstring as parameter, and returns a new dstring, that is of approximately double the size 
void expand_dstring(dstring* str_p, unsigned int additional_allocation);

// here occ is the number of bytes to be copied and appended from the cstr_p to str_p, without '\0' at the end
void appendn_to_dstring(dstring* str_p, char* cstr_p, unsigned int occ);

void append_to_dstring(dstring* str_p, char* cstr_p);

void concatenate_dstring(dstring* str_p1, dstring* str_p2);

// modifying the dstring case
void toLowercase(dstring* str_p);
void toUppercase(dstring* str_p);

void display_dstring(dstring* str_p);

// deinitializing a dstring will release all its memory,
// so you can init it again to reuse the dstring
// enable to use dstring on stack or in an embedded form in other data structures
void deinit_dstring(dstring* str_p);

void delete_dstring(dstring* str_p);

#endif