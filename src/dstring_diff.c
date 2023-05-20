#include<dstring_diff.h>

#include<cutlery_stds.h>
#include<memory_allocator_interface.h>
#include<multi_dim_array_util.h>

static unsigned int minimum_of(unsigned int a, unsigned int b, unsigned int c)
{
	unsigned int min_a_b = (a < b) ? a : b;
	return (min_a_b < c) ? min_a_b : c;
}

unsigned int levenshtein_distance(const dstring* str0, const dstring* str1)
{
	const char* str0_data = get_byte_array_dstring(str0);
	unsigned int str0_len = get_char_count_dstring(str0);

	const char* str1_data = get_byte_array_dstring(str1);
	unsigned int str1_len = get_char_count_dstring(str1);

	unsigned int dp_dims[] = {str0_len + 1, str1_len + 1};
	cy_uint dp_bytes = sizeof(unsigned int) * ((cy_uint)dp_dims[0]) * ((cy_uint)dp_dims[1]);
	unsigned int* dp = allocate(DSTRING_mem_alloc, &dp_bytes);

	for(unsigned int i = 0; i < dp_dims[1]; i++)
	{
		for(unsigned int j = 0; j < dp_dims[0]; j++)
		{
			unsigned int curr = get_accessor_from_indices((unsigned int[]){j, i}, dp_dims, 2);

			if(i == 0)
				dp[curr] = j;
			else if(j == 0)
				dp[curr] = i;
			else
			{
				unsigned int top_left = get_accessor_from_indices((unsigned int[]){j-1, i-1}, dp_dims, 2);
				unsigned int top = get_accessor_from_indices((unsigned int[]){j, i-1}, dp_dims, 2);
				unsigned int left = get_accessor_from_indices((unsigned int[]){j-1, i}, dp_dims, 2);

				if(str1_data[i - 1] == str0_data[j - 1])
					dp[curr] = dp[top_left];
				else
					dp[curr] = minimum_of(  1 + dp[top_left], // replace
											1 + dp[top],      // insert in str1
											1 + dp[left] );   // insert in str0
			}
		}
	}

	// result at the last element
	unsigned int result = dp[(dp_dims[0] * dp_dims[1]) - 1];

	deallocate(DSTRING_mem_alloc, dp, sizeof(unsigned int) * ((cy_uint)dp_dims[0]) * ((cy_uint)dp_dims[1]));

	return result;
}

static unsigned int maximum_of(unsigned int a, unsigned int b, unsigned int c)
{
	unsigned int max_a_b = (a > b) ? a : b;
	return (max_a_b > c) ? max_a_b : c;
}

unsigned int length_of_longest_common_subsequence(const dstring* str0, const dstring* str1)
{
	const char* str0_data = get_byte_array_dstring(str0);
	unsigned int str0_len = get_char_count_dstring(str0);

	const char* str1_data = get_byte_array_dstring(str1);
	unsigned int str1_len = get_char_count_dstring(str1);

	unsigned int dp_dims[] = {str0_len + 1, str1_len + 1};
	cy_uint dp_bytes = sizeof(unsigned int) * ((cy_uint)dp_dims[0]) * ((cy_uint)dp_dims[1]);
	unsigned int* dp = allocate(DSTRING_mem_alloc, &dp_bytes);

	for(unsigned int i = 0; i < dp_dims[1]; i++)
	{
		for(unsigned int j = 0; j < dp_dims[0]; j++)
		{
			unsigned int curr = get_accessor_from_indices((unsigned int[]){j, i}, dp_dims, 2);

			if(i == 0)
				dp[curr] = 0;
			else if(j == 0)
				dp[curr] = 0;
			else
			{
				unsigned int top_left = get_accessor_from_indices((unsigned int[]){j-1, i-1}, dp_dims, 2);
				unsigned int top = get_accessor_from_indices((unsigned int[]){j, i-1}, dp_dims, 2);
				unsigned int left = get_accessor_from_indices((unsigned int[]){j-1, i}, dp_dims, 2);

				if(str1_data[i - 1] == str0_data[j - 1])
					dp[curr] = 1 + dp[top_left];
				else
					dp[curr] = maximum_of(  dp[top_left],
											dp[top],
											dp[left] );
			}
		}
	}

	// result at the last element
	unsigned int result = dp[(dp_dims[0] * dp_dims[1]) - 1];

	deallocate(DSTRING_mem_alloc, dp, sizeof(unsigned int) * ((cy_uint)dp_dims[0]) * ((cy_uint)dp_dims[1]));

	return result;
}