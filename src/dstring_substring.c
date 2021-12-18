#include<dstring_substring.h>

#include<cutlery_stds.h>

void get_prefix_suffix_match_lengths(const dstring* str, unsigned int* suffix_prefix_match_length)
{
	// build the cache for the substring calculation
	suffix_prefix_match_length[0] = 0;
	suffix_prefix_match_length[1] = 0;
	unsigned int string_length = 2;
	while(string_length <= get_char_count_dstring(str))
	{
		unsigned int prefix_length_old = suffix_prefix_match_length[string_length - 1];
		while(1)
		{
			if(get_byte_array_dstring(str)[string_length-1] == get_byte_array_dstring(str)[prefix_length_old])
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
	if(get_char_count_dstring(str) < get_char_count_dstring(sub_str))
		return NULL;

	// use KMP algorithm O(m + n)
	if(suffix_prefix_match_length_for_sub_str != NULL) {
		// iterate over the string to find the substring location
		for(unsigned int i = 0, substring_iter = 0; i < get_char_count_dstring(str);)
		{
			if(get_byte_array_dstring(str)[i] == get_byte_array_dstring(sub_str)[substring_iter])
			{
				if(substring_iter < get_char_count_dstring(sub_str) - 1)
				{
					substring_iter++;
					i++;
				}
				else
					return get_byte_array_dstring(str) + i - (get_char_count_dstring(sub_str) - 1);
			}
			else if(substring_iter == 0)
				i++;
			else
				substring_iter = suffix_prefix_match_length_for_sub_str[substring_iter];
		}
	}
	else // use standard algorithm O(m * n)
	{
		for(unsigned int i = 0; i <= get_char_count_dstring(str) - get_char_count_dstring(sub_str); i++)
		{
			unsigned int found = 1;
			for(unsigned int j = 0; j < get_char_count_dstring(sub_str); j++)
			{
				if(get_byte_array_dstring(str)[i + j] != get_byte_array_dstring(sub_str)[j])
				{
					found = 0;
					break;
				}
			}
			if(found)
				return get_byte_array_dstring(str) + i;
		}
	}

	return NULL;
}

int is_prefix(const dstring* string_p, const dstring* prefix_p)
{
	// prefix must have a length smaller than or equal to provided main string
	if(get_char_count_dstring(string_p) >= get_char_count_dstring(prefix_p))
		return 0 == memory_compare(get_byte_array_dstring(string_p), get_byte_array_dstring(prefix_p), get_char_count_dstring(prefix_p));
	return 0;
}