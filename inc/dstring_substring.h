#ifndef DSTRING_SUBSTRING_H
#define DSTRING_SUBSTRING_H

#include<dstring.h>

// returns suffix_prefix_match_length
// if for first i characters of str, the prefix of str equals the suffix of str
// then its length is stored in suffix_prefix_match_length[i]
// the suffix_prefix_match_length must have space for (str->bytes_occupied + 1) unsigned ints
// result of this function is required for using KMP algorithm (pass suffix_prefix_match_length in contains_dstring function)
void get_prefix_suffix_match_lengths(const dstring* str, unsigned int* suffix_prefix_match_length);

// returns char pointer to the position of first substring match inside the str, else it returns NULL
// this function is equivalent to strnstr in c
char* contains_dstring(const dstring* str, const dstring* sub_str, unsigned int* suffix_prefix_match_length);
// KMP (Knuth–Morris–Pratt) O(m+n) will be used if you provide a non-NULL value for suffix_prefix_match_length (result of get_prefix_suffix_match_lengths)
// else it will use standard O(m*n) sub string algorithm

// returns 1 if prefix_p is prefix of string_p, else it returns 0
int is_prefix(const dstring* string_p, const dstring* prefix_p);

#endif