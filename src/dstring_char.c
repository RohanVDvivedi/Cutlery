#include<dstring_char.h>

static inline int is_char_in_range(char c, char lte, char gte)
{
	return (lte <= c) && (c <= gte);
}

int is_digit_char(char c);
int is_whitespace_char(char c);
int is_lowercase_char(char c);
int is_uppercase_char(char c);

char to_lowercase_char(char c);
char to_uppercase_char(char c);