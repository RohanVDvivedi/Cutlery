#ifndef DSTRING_SUBSTRING_H
#define DSTRING_SUBSTRING_H

#include<dstring.h>



// returns suffix_prefix_match_length
// this function populates suffix_prefix_match_length unsigned int array that is atleast (get_char_count_dstring(sub_str) + 1) elements long
// this function must be used as a pre computation step of KMP algorithm
//
// upon return for sub_str[0..i] the length of prefix that matches the suffix is given by suffix_prefix_match_length[i-1]
void get_prefix_suffix_match_lengths(const dstring* sub_str, unsigned int* suffix_prefix_match_length);
// used as pre-computation of KMP contains_dstring algorithm
// this function has complexity of O(m), where m is the length of str




/*
**  contains_dstring_* are a group of functions required to find the position of a sub string dstring (sub_str) in a dstring (str)
**  their complexity are denoted in terms of m and n
**  where m is the length of sub_str and n is the length of str
**
**  params : str is the base large string, sub_str is the sub string that we need to find in the str
**
**  return : index to the position of first sub string (sub_str) match inside the str, else it returns INVALID_INDEX
*/

unsigned int contains_dstring_NAIVE(const dstring* str, const dstring* sub_str);
// Naive implementation O(m*n)

unsigned int contains_dstring_KMP(const dstring* str, const dstring* sub_str, const unsigned int* suffix_prefix_match_length);
// KMP (Knuth–Morris–Pratt) O(n)

unsigned int contains_dstring_RK(const dstring* str, const dstring* sub_str);
// RK (Rabin Karp) (best case O(m+n) and worst case O(m*n))

unsigned int contains_dstring_BM(const dstring* str, const dstring* sub_str);
// BM (Booyer Moore)




/*
**	is_prefix function checks if the given dstring prefix_p is prefic to another dstring string_p
**  return 1 if true, and 0 if not a prefix
*/

// returns 1 if prefix_p is prefix of string_p, else it returns 0
int is_prefix(const dstring* string_p, const dstring* prefix_p);

#endif