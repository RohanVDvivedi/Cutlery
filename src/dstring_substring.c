#include<cutlery/dstring_substring.h>

#include<cutlery/cutlery_stds.h>

void get_prefix_suffix_match_lengths(const dstring* sub_str, cy_uint* suffix_prefix_match_length)
{
	const char* sub_str_data = get_byte_array_dstring(sub_str);
	cy_uint sub_str_size = get_char_count_dstring(sub_str);

	for(cy_uint string_length = 0; ; string_length++)
	{
		if(string_length <= 1)
			suffix_prefix_match_length[string_length] = 0;
		else
		{
			cy_uint prefix_length_old = suffix_prefix_match_length[string_length - 1];
			while(1)
			{
				if(sub_str_data[string_length-1] == sub_str_data[prefix_length_old])
				{
					suffix_prefix_match_length[string_length] = prefix_length_old + 1;
					break;
				}
				else
				{
					// only after ensuring that prefix_length_old == 0 could not contribute, we break
					if(prefix_length_old == 0)
					{
						suffix_prefix_match_length[string_length] = 0;
						break;
					}
					prefix_length_old = suffix_prefix_match_length[prefix_length_old];
				}
			}
		}

		// when all the possible lengths of sub_str have been processed then quit the loop
		if(string_length == sub_str_size)
			break;
	}
}

cy_uint contains_dstring_NAIVE(const dstring* str, const dstring* sub_str)
{
	const char* str_data = get_byte_array_dstring(str);
	cy_uint str_size = get_char_count_dstring(str);

	const char* sub_str_data = get_byte_array_dstring(sub_str);
	cy_uint sub_str_size = get_char_count_dstring(sub_str);

	// sub_string to be found can not be smaller than the string
	// also the string to find the substring in can not be empty
	if(str_size < sub_str_size || str_size == 0)
		return INVALID_INDEX;

	// if the substring is empty then any result would suffice
	if(sub_str_size == 0)
		return 0;

	// in the str dstring iterate from i => 0 to (str_size - sub_str_size) both inclusive
	for(cy_uint i = 0; i <= str_size - sub_str_size; i++)
	{
		int found = 1;
		for(cy_uint j = 0; j < sub_str_size; j++)
		{
			if(str_data[i + j] != sub_str_data[j])
			{
				found = 0;
				break;
			}
		}
		if(found)
			return i;
	}

	// substring sub_str does not occur in the str
	return INVALID_INDEX;
}

cy_uint contains_dstring_KMP(const dstring* str, const dstring* sub_str, const cy_uint* suffix_prefix_match_length)
{
	const char* str_data = get_byte_array_dstring(str);
	cy_uint str_size = get_char_count_dstring(str);

	const char* sub_str_data = get_byte_array_dstring(sub_str);
	cy_uint sub_str_size = get_char_count_dstring(sub_str);

	// sub_string to be found can not be smaller than the string
	// also the string to find the substring in can not be empty
	if(str_size < sub_str_size || str_size == 0 || suffix_prefix_match_length == NULL)
		return INVALID_INDEX;

	// if the substring is empty then any result would suffice
	if(sub_str_size == 0)
		return 0;

	// Knuth Morris Pratt
	// iterate over the string str form i => 0 to (str_size - 1) both inclusive
	for(cy_uint i = 0, substring_iter = 0; i < str_size;)
	{
		if(str_data[i] == sub_str_data[substring_iter])
		{
			// unless last character match, increment both the iters
			if(substring_iter < sub_str_size - 1)
			{
				substring_iter++;
				i++;
			}
			else
				return i - (sub_str_size - 1);
		}
		// increment string iter i, if substring_iter == 0
		// i.e. unmatch on the first character
		else if(substring_iter == 0)
			i++;
		// else find the next substring_iter to continue from
		else
			substring_iter = suffix_prefix_match_length[substring_iter];
	}

	// substring sub_str does not occur in the str
	return INVALID_INDEX;
}

cy_uint contains_dstring_RK(const dstring* str, const dstring* sub_str)
{
	const char* str_data = get_byte_array_dstring(str);
	cy_uint str_size = get_char_count_dstring(str);

	const char* sub_str_data = get_byte_array_dstring(sub_str);
	cy_uint sub_str_size = get_char_count_dstring(sub_str);

	// sub_string to be found can not be smaller than the string
	// also the string to find the substring in can not be empty
	if(str_size < sub_str_size || str_size == 0)
		return INVALID_INDEX;

	// if the substring is empty then any result would suffice
	if(sub_str_size == 0)
		return 0;

	// Rabin Krap
	// hash(str) => summation of all ((i + 1) * str_data[i]) form i = 0 to (str_size - 1)

	// find the hash and char_sum of the sub_str,
	// and hash of first sub_str_size characters of str
	// and the sum of first sub_str_size characters of str
	unsigned long long int sub_str_hash = 0;
	unsigned long long int sub_str_char_sum = 0;
	unsigned long long int str_hash_rolling = 0;
	unsigned long long int str_char_sum_rolling = 0;
	for(cy_uint i = 0; i < sub_str_size; i++)
	{
		sub_str_hash += ((i + 1) * ((unsigned long long int)sub_str_data[i]));
		sub_str_char_sum += ((unsigned long long int)sub_str_data[i]);
		str_hash_rolling += ((i + 1) * ((unsigned long long int)str_data[i]));
		str_char_sum_rolling += ((unsigned long long int)str_data[i]);
	}

	for(cy_uint i = 0; ; i++)
	{
		if(sub_str_char_sum == str_char_sum_rolling && sub_str_hash == str_hash_rolling)
		{
			int found = 1;
			for(cy_uint j = 0; j < sub_str_size; j++)
			{
				if(str_data[i + j] != sub_str_data[j])
				{
					found = 0;
					break;
				}
			}
			if(found)
				return i;
		}

		// except when it is the last iteration
		if(i + sub_str_size == str_size)
			break;

		// prepare str_hash_rolling and str_sum_rolling for the next iteration
		str_hash_rolling -= str_char_sum_rolling;
		str_hash_rolling += (sub_str_size * ((unsigned long long int)str_data[i + sub_str_size]));
		str_char_sum_rolling -= ((unsigned long long int)str_data[i]);
		str_char_sum_rolling += ((unsigned long long int)str_data[i + sub_str_size]);
	}

	// substring sub_str does not occur in the str
	return INVALID_INDEX;
}

int is_prefix_of_dstring(const dstring* string_p, const dstring* prefix_p)
{
	// prefix must have a length smaller than or equal to provided main string
	if(get_char_count_dstring(string_p) >= get_char_count_dstring(prefix_p))
		return 0 == memory_compare(get_byte_array_dstring(string_p), get_byte_array_dstring(prefix_p), get_char_count_dstring(prefix_p));
	return 0;
}

int is_suffix_of_dstring(const dstring* string_p, const dstring* suffix_p)
{
	// suffix must have a length smaller than or equal to provided main string
	if(get_char_count_dstring(string_p) >= get_char_count_dstring(suffix_p))
		return 0 == memory_compare(get_byte_array_dstring(string_p) + get_char_count_dstring(string_p) - get_char_count_dstring(suffix_p), get_byte_array_dstring(suffix_p), get_char_count_dstring(suffix_p));
	return 0;
}

dstring split_dstring(const dstring* str, const dstring* delim, dstring* split)
{
	const char* str_data = get_byte_array_dstring(str);
	cy_uint str_size = get_char_count_dstring(str);

	cy_uint delim_size = get_char_count_dstring(delim);

	cy_uint delim_index = contains_dstring_RK(str, delim);

	if(delim_index == INVALID_INDEX)
	{
		(*split) = get_dstring_pointing_to(str_data, str_size);
		return get_dstring_pointing_to(NULL, 0);
	}

	(*split) = get_dstring_pointing_to(str_data, delim_index);
	return get_dstring_pointing_to(str_data + delim_index + delim_size, str_size - delim_index - delim_size);
}