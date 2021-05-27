#include<dstring_substring.h>

#include<string.h>

void get_prefix_suffix_match_lengths(const dstring* str, unsigned int* suffix_prefix_match_length)
{
	// build the cache for the substring calculation
	suffix_prefix_match_length[0] = 0;
	suffix_prefix_match_length[1] = 0;
	unsigned int string_length = 2;
	while(string_length <= str->bytes_occupied)
	{
		unsigned int prefix_length_old = suffix_prefix_match_length[string_length - 1];
		while(1)
		{
			if(str->cstring[string_length-1] == str->cstring[prefix_length_old])
			{
				suffix_prefix_match_length[string_length] = prefix_length_old + 1;
				break;
			}
			else
			{
				prefix_length_old = suffix_prefix_match_length[prefix_length_old];
				if(prefix_length_old == 0)
				{
					suffix_prefix_match_length[string_length] = 0;
					break;
				}
			}
		}
		string_length++;
	}
}
// KMP implementation for substring position in a given string
char* contains_dstring(const dstring* str, const dstring* sub_str, unsigned int* suffix_prefix_match_length_for_sub_str)
{
	if(str->bytes_occupied < sub_str->bytes_occupied)
		return NULL;

	// use KMP algorithm O(m + n)
	if(suffix_prefix_match_length_for_sub_str != NULL) {
		// iterate over the string to find the substring location
		for(unsigned int i = 0, substring_iter = 0; i < str->bytes_occupied;)
		{
			if(str->cstring[i] == sub_str->cstring[substring_iter])
			{
				if(substring_iter < sub_str->bytes_occupied - 1)
				{
					substring_iter++;
					i++;
				}
				else
					return str->cstring + i - (sub_str->bytes_occupied - 1);
			}
			else if(substring_iter == 0)
				i++;
			else
				substring_iter = suffix_prefix_match_length_for_sub_str[substring_iter];
		}
	}
	else // use standard algorithm O(m * n)
	{
		for(unsigned int i = 0; i <= str->bytes_occupied - sub_str->bytes_occupied; i++)
		{
			unsigned int found = 1;
			for(unsigned int j = 0; j < sub_str->bytes_occupied; j++)
			{
				if(str->cstring[i + j] != sub_str->cstring[j])
				{
					found = 0;
					break;
				}
			}
			if(found)
				return str->cstring + i;
		}
	}

	return NULL;
}

int is_prefix(const dstring* string_p, const dstring* prefix_p)
{
	// prefix must have a length smaller than or equal to provided main string
	if(string_p->bytes_occupied >= prefix_p->bytes_occupied)
		return 0 == strncmp(string_p->cstring, prefix_p->cstring, prefix_p->bytes_occupied);
	return 0;
}