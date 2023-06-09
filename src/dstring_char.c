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
	char whitespace_char[] = {' ', '\t', '\v', '\r', '\n', '\f'};
	for(int i = 0; i < sizeof(whitespace_char); i++)
	{
		if(c == whitespace_char[i])
			return 1;
	}
	return 0;
}

int is_lowercase_char(char c)
{
	return is_char_in_range(c, 'a', 'z');
}

int is_uppercase_char(char c)
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

cy_uint get_digits_required_to_represent_unsigned_long_long_int(unsigned int radix)
{
	unsigned long long int MAX_VAL = -1;
	cy_uint res = 0;
	while(MAX_VAL > 0)
	{
		res++;
		MAX_VAL /= radix;
	}
	return res;
}

unsigned int get_digit_from_char(char c, unsigned int radix)
{
	if(radix < 2 || radix > 36)
		return -1;

	unsigned int digit = -1;
	if(is_char_in_range(c, '0', '9'))
		digit = c - '0';
	else if(is_char_in_range(c, 'a', 'z'))
		digit = c - 'a' + 10;
	else if(is_char_in_range(c, 'A', 'Z'))
		digit = c - 'A' + 10;

	if(digit < radix)
		return digit;

	return -1;
}