#ifndef DSTRING_CHAR_H
#define DSTRING_CHAR_H

#include<cutlery_stds.h>

int is_digit_char(char c);
int is_whitespace_char(char c);
int is_lowercase_char(char c);
int is_uppercase_char(char c);

char to_lowercase_char(char c);
char to_uppercase_char(char c);

int compare_chars_ascending(char c1, char c2);
int compare_chars_descending(char c1, char c2);

// any radix value is allowed here
// this is the maximum number of digits (for a specific radix) required to store an unsigned long long int
cy_uint get_digits_required_to_represent_unsigned_long_long_int(unsigned int radix);

// radix can be any value from 2 to 36
// returns INVALID_INDEX when radix is invalid or the character c is not valid
// i.e. for radix = 16, the valid characters are 0 - 9 and a - f or A - F
unsigned int get_digit_from_char(char c, unsigned int radix);

#endif