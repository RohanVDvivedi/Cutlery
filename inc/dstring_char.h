#ifndef DSTRING_CHAR_H
#define DSTRING_CHAR_H

int is_digit_char(char c);
int is_whitespace_char(char c);
int is_lowercase_char(char c);
int is_uppercase_char(char c);

char to_lowercase_char(char c);
char to_uppercase_char(char c);

// radix can be any value from 2 to 36
// returns INVALID_INDEX when radix is invalid or the character c is not valid
// i.e. for radix = 16, the valid characters are 0 - 9 and a - f or A - F
unsigned int get_digit_from_char(char c, unsigned int radix);

#endif