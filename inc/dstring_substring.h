#ifndef DSTRING_SUBSTRING_H
#define DSTRING_SUBSTRING_H

#include<dstring.h>



// returns suffix_prefix_match_length
// this function populates suffix_prefix_match_length cy_uint array that is atleast (get_char_count_dstring(sub_str) + 1) elements long
// this function must be used as a pre computation step of KMP algorithm
//
// upon return for sub_str[0..i] the length of prefix that matches the suffix is given by suffix_prefix_match_length[i-1]
void get_prefix_suffix_match_lengths(const dstring* sub_str, cy_uint* suffix_prefix_match_length);
// used as pre-computation of KMP contains_dstring algorithm
// this function has complexity of O(m), where m is the length of str




/*
**  contains_dstring_* are a group of functions required to find the position of a sub string dstring (sub_str) in a dstring (str)
**  their complexity are denoted in terms of m and n
**  where m is the length of sub_str and n is the length of str
**
**  params : str is the base large string, sub_str is the sub string that we need to find in the str
**
**  return : first index to the position of first sub string (sub_str) match inside the str, else it returns INVALID_INDEX
*/

cy_uint contains_dstring_NAIVE(const dstring* str, const dstring* sub_str);
// Naive implementation O(m*n)

cy_uint contains_dstring_KMP(const dstring* str, const dstring* sub_str, const cy_uint* suffix_prefix_match_length);
// KMP (Knuth–Morris–Pratt) O(m+n)

cy_uint contains_dstring_RK(const dstring* str, const dstring* sub_str);
// RK (Rabin Karp) (best case O(m+n) and worst case O(m*n))




/*
**	is_prefix_of_dstring function checks if the given dstring prefix_p is prefix to another dstring string_p
**  return 1 if true, and 0 if not a prefix
*/

// returns 1 if prefix_p is prefix of string_p, else it returns 0
int is_prefix_of_dstring(const dstring* string_p, const dstring* prefix_p);

// returns 1 if suffix_p is suffix of string_p, else it returns 0
int is_suffix_of_dstring(const dstring* string_p, const dstring* suffix_p);



/*
**  split_dstring function splits a dstring into 2 parts separated by the first occurence of the delimeter dstring
**  none of the 2 parts will contain the delimeter
**  it uses contains_dstring_RK function to find the index of the delimeter dstring
**
**  dstring remaining = split_dstring(const dstring* str, const dstring* delim, dstring* split);
**  here the split contains the first part of the split and the remaining string will be returned
**
**	in absence of delimeter, the split will contain the entire dstring, while the remaining dstring will have its byte_array pointing to NULL
**	i.e. for the last iteration get_byte_array_dstring(&remaining) == NULL
**
**  both the split and the remaining dstrings will be POINT_DSTR dstring, i.e. they will only point to str
**  if an instance of delim is not found then the remaining will be an empty_dstring
**  since both split and remaining will be POINT_DSTR pointing to the original str, you do not need to deinit_dstring them (it would be a NOP)
*/

dstring split_dstring(const dstring* str, const dstring* delim, dstring* split);

#include<cutlery_stds.h>

// below is an easy way to iterate over all the split strings of the (dstring*) str split by a (dstring*) delimeter
#define for_each_split_by_delim(split, str, delim) for(dstring split, CAT(__remaining, __LINE__) = split_dstring(str, delim, &split); get_byte_array_dstring(&split) != NULL; CAT(__remaining, __LINE__) = split_dstring(&CAT(__remaining, __LINE__), delim, &split))

/*
	usage of the above macro

	dstring str = get_dstring_pointing_to_literal_cstring("Vipul, Rupa, Devashree, Rohan, , Jumbo");

	for_each_split_by_delim(split, &str, get_dstring_pointing_to_literal_cstring(", "))
	{
		// do here what every you like with split
	}
*/

#endif
