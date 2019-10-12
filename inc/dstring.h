#ifndef DSTRING_H
#define DSTRING_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct dstring dstring;
struct dstring
{
	// actual c string with \0 at the end
	char* cstring;

	// number of bytes the string cstring occupies, 
	unsigned long long int bytes_occupied;
	unsigned long long int bytes_allocated;

	unsigned long long int state_level;
};

dstring* get_dstring(const char* cstr_p, unsigned long long int additional_allocation);

// returns 1, if str_p1 > str_p2, returns 0 if they are equal else returns -1
int compare(const dstring* str_p1, const dstring* str_p2);

// takes a dstring as parameter, and returns a new dstring, that is of approximately double the size 
dstring* expand(dstring* str_p, unsigned long long int additional_allocation);

dstring* append(dstring* str_p, char* cstr_p);

dstring* concatenate(dstring* str_p1, dstring* str_p2);

void display(dstring* str_p);

void delete_dstring(dstring* str_p);

#endif