#include<dstring.h>

#include<stdio.h>
#include<stdlib.h>
#include<alloca.h>
#include<stdarg.h>
#include<ctype.h>
#include<string.h>

dstring* get_dstring_data(const char* data, unsigned int data_size)
{
	dstring* str_p = malloc(sizeof(dstring));
	init_dstring_data(str_p, data, data_size);
	return str_p;
}

void init_dstring_slize(dstring* slize, char* data, unsigned int data_size)
{
	slize->cstring = data;
	slize->bytes_occupied = data_size;
	slize->bytes_allocated = 0;
}

void init_dstring(dstring* str_p, const char* cstr_p)
{
	init_dstring_data(str_p, cstr_p, ((cstr_p == NULL) ? 0 : strlen(cstr_p)));
}

void init_dstring_data(dstring* str_p, const char* data, unsigned int data_size)
{
	if(data == NULL || data_size == 0)
	{
		str_p->bytes_occupied = 0;
		str_p->bytes_allocated = 0;
		str_p->cstring = NULL;
		return;
	}
	str_p->bytes_occupied = data_size;
	str_p->bytes_allocated = data_size;
	str_p->cstring = malloc(data_size);
	memcpy(str_p->cstring, data, data_size);
}

void convert_slize_to_dstring(dstring* slize)
{
	init_dstring_data(slize, slize->cstring, slize->bytes_occupied);
}

void make_dstring_empty(dstring* str_p)
{
	str_p->bytes_occupied = 0;
}

static int compare_string_safe(const char* str1, unsigned int size1, const char* str2, unsigned int size2)
{
	if(size1 == size2)
		return strncmp(str1, str2, size1);
	unsigned int min_size = (size1 < size2) ? size1 : size2;
	int cmp = strncmp(str1, str2, min_size);
	if(cmp != 0)
		return cmp;
	if(min_size == size1)
		return 1;
	return -1;
}
int compare_dstring(const dstring* str_p1, const dstring* str_p2)
{
	return compare_string_safe(str_p1->cstring, str_p1->bytes_occupied, str_p2->cstring, str_p2->bytes_occupied);
}
int compare_dstring_cstring(const dstring* str_p1, const char* str_p2)
{
	return compare_string_safe(str_p1->cstring, str_p1->bytes_occupied, str_p2, strlen(str_p2));
}

static int case_compare_string_safe(const char* str1, unsigned int size1, const char* str2, unsigned int size2)
{
	if(size1 == size2)
		return strncasecmp(str1, str2, size1);
	unsigned int min_size = (size1 < size2) ? size1 : size2;
	int cmp = strncasecmp(str1, str2, min_size);
	if(cmp != 0)
		return cmp;
	if(min_size == size1)
		return 1;
	return -1;
}
int case_compare_dstring(const dstring* str_p1, const dstring* str_p2)
{
	return case_compare_string_safe(str_p1->cstring, str_p1->bytes_occupied, str_p2->cstring, str_p2->bytes_occupied);
}
int case_compare_dstring_cstring(const dstring* str_p1, const char* str_p2)
{
	return case_compare_string_safe(str_p1->cstring, str_p1->bytes_occupied, str_p2, strlen(str_p2));
}

void get_prefix_suffix_match_lengths(const dstring* str, unsigned int* suffix_prefix_match_length)
{
	// build the cache for the substring calculation
	suffix_prefix_match_length[0] = 0;
	suffix_prefix_match_length[1] = 0;
	for(unsigned int sub_length = 2; sub_length <= str->bytes_occupied; sub_length++) {
		// calculate the next match check location where the prefix equals the suffix of the substring
		if(str->cstring[sub_length-1] == str->cstring[suffix_prefix_match_length[sub_length-1]])
			suffix_prefix_match_length[sub_length] = suffix_prefix_match_length[sub_length-1] + 1;
		else
			suffix_prefix_match_length[sub_length] = 0;
	}
}
// KMP implementation for substring position in a given string
unsigned int contains_dstring(const dstring* str, const dstring* sub_str, unsigned int* suffix_prefix_match_length_for_sub_str)
{
	if(str->bytes_occupied < sub_str->bytes_occupied)
		return SUBSTRING_NOT_FOUND;

	// use KMP algorithm O(m + n)
	if(suffix_prefix_match_length_for_sub_str != NULL) {
		// iterate over the string to find the substring loaction
		for(unsigned int i = 0, substring_iter = 0; i <= str->bytes_occupied;) {
			if(str->cstring[i] == sub_str->cstring[substring_iter])
			{
				if(substring_iter < sub_str->bytes_occupied - 1)
				{
					substring_iter++;
					i++;
				}
				else
					return i - (sub_str->bytes_occupied - 1);
			}
			else if(substring_iter == 0)
				i++;
			else
				substring_iter = suffix_prefix_match_length_for_sub_str[substring_iter];
		}
	}
	else // use standard algorithm O(m * n)
	{

	}

	return SUBSTRING_NOT_FOUND;
}
unsigned int contains_cstring(const dstring* str, const char* sub_str)
{
	return contains_dstring(str, &((dstring){.cstring = (char*)sub_str, .bytes_occupied = strlen(sub_str)}), NULL);
}

int is_prefix(const dstring* str_p1, const char* str_p2)
{
	size_t prefix_length = strlen(str_p2);
	// prefix length must be smaller than or equal to dstring provided
	if(prefix_length > str_p1->bytes_occupied)
		return 0;
	return (strncmp(str_p1->cstring, str_p2, prefix_length) == 0);
}

void expand_dstring(dstring* str_p, unsigned int additional_allocation)
{
	dstring expanded_dstring;
	expanded_dstring.bytes_occupied = str_p->bytes_occupied;
	expanded_dstring.bytes_allocated = str_p->bytes_occupied + additional_allocation;

	// if expanded dstring does not result in real expansion, than just exit
	if(expanded_dstring.bytes_allocated <= str_p->bytes_allocated)
		return;

	expanded_dstring.cstring = malloc(expanded_dstring.bytes_allocated);
	memcpy(expanded_dstring.cstring, str_p->cstring, str_p->bytes_occupied);
	deinit_dstring(str_p);
	(*str_p) = expanded_dstring;
}

void appendn_to_dstring(dstring* str_p, const char* data, unsigned int data_size)
{
	if(data != NULL && data_size > 0)
	{
		// check if new data could fit, without expansion, else expand the dstring
		if(str_p->bytes_occupied + data_size > str_p->bytes_allocated)
			expand_dstring(str_p, str_p->bytes_occupied + 2 * data_size);

		// do appending as normal now
		memcpy(str_p->cstring + str_p->bytes_occupied, data, data_size);
		str_p->bytes_occupied += data_size;
	}
}

void append_to_dstring(dstring* str_p, const char* cstr_p)
{
	appendn_to_dstring(str_p, cstr_p, ((cstr_p == NULL) ? 0 : strlen(cstr_p)) );
}

void append_to_dstring_formatted(dstring* str_p, const char* cstr_format, ...)
{
	va_list var_args, var_args_dummy;
	va_start(var_args, cstr_format);

	va_copy(var_args_dummy, var_args);
	// this is the additional size that will be occupied by the final dstring over the current occupied size
	unsigned int size_extra_req = vsnprintf(NULL, 0, cstr_format, var_args_dummy);
	va_end(var_args_dummy);

	// expand str_p as needed
	if(size_extra_req + str_p->bytes_occupied > str_p->bytes_allocated)
		expand_dstring(str_p, str_p->bytes_occupied + 2 * size_extra_req);

	str_p->bytes_occupied += vsnprintf(str_p->cstring + str_p->bytes_occupied, str_p->bytes_allocated - str_p->bytes_occupied, cstr_format, var_args);
	va_end(var_args);
}

void concatenate_dstring(dstring* str_p1, const dstring* str_p2)
{
	// we shall send the length of the dstring without the '\0'
	appendn_to_dstring(str_p1, str_p2->cstring, str_p2->bytes_occupied);
}

void toLowercase(dstring* str_p)
{
	char* stemp = str_p->cstring;
	while(stemp < str_p->cstring + str_p->bytes_occupied)
	{
		*stemp = tolower(*stemp);
		stemp++;
	}
}

void toUppercase(dstring* str_p)
{
	char* stemp = str_p->cstring;
	while(stemp < str_p->cstring + str_p->bytes_occupied)
	{
		*stemp = toupper(*stemp);
		stemp++;
	}
}

void display_dstring(const dstring* str_p)
{
	if(str_p->cstring != NULL)
		printf("%.*s", str_p->bytes_occupied, str_p->cstring);
}

void deinit_dstring(dstring* str_p)
{
	if(str_p->bytes_allocated > 0 && str_p->cstring != NULL)
		free(str_p->cstring);
	str_p->cstring = NULL;
	str_p->bytes_allocated = 0;
	str_p->bytes_occupied = 0;
}

void delete_dstring(dstring* str_p)
{
	deinit_dstring(str_p);
	free(str_p);
}