#include<dstring_char.h>

static inline int is_char_in_range(char c, char lte, char gte)
{
	return (lte <= c) && (c <= gte);
}

int is_digit_char(char c)
{
	return is_char_in_range(c, '0', '9');
}

int is_whitespace_char(char c)
{

}

int is_lowercase_char(char c)
{
	return is_char_in_range(c, 'a', 'z');
}

int is_uppercase_char(char c);
{
	return is_char_in_range(c, 'A', 'Z');
}

char to_lowercase_char(char c)
{
	if(is_uppercase_char(c))
		return c - 'A' + 'a';
	return c;
}

char to_uppercase_char(char c)
{
	if(is_lowercase_char(c))
		return c - 'a' + 'A';
	return c;
}