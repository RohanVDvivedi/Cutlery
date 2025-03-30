#include<cutlery/dstring_diff.h>

#include<cutlery/cutlery_stds.h>
#include<cutlery/cutlery_math.h>
#include<cutlery/memory_allocator_interface.h>
#include<cutlery/multi_dim_array_util.h>

#define min3(a,b,c) min((a),(min((b),(c))))

cy_uint levenshtein_distance(const dstring* str0, const dstring* str1)
{
	const char* str0_data = get_byte_array_dstring(str0);
	cy_uint str0_len = get_char_count_dstring(str0);

	const char* str1_data = get_byte_array_dstring(str1);
	cy_uint str1_len = get_char_count_dstring(str1);

	cy_uint dp_dims[] = {str0_len + 1, str1_len + 1};
	cy_uint dp_bytes = sizeof(cy_uint) * dp_dims[0] * dp_dims[1];
	cy_uint* dp = aligned_allocate(DSTRING_mem_alloc, &dp_bytes, _Alignof(cy_uint));

	// on failure, return a CY_UINT_MAX
	if(dp == NULL)
		return CY_UINT_MAX;

	for(cy_uint i = 0; i < dp_dims[1]; i++)
	{
		for(cy_uint j = 0; j < dp_dims[0]; j++)
		{
			cy_uint curr = get_accessor_from_indices((cy_uint[]){j, i}, dp_dims, 2);

			if(i == 0)
				dp[curr] = j;
			else if(j == 0)
				dp[curr] = i;
			else
			{
				cy_uint top_left = get_accessor_from_indices((cy_uint[]){j-1, i-1}, dp_dims, 2);
				cy_uint top = get_accessor_from_indices((cy_uint[]){j, i-1}, dp_dims, 2);
				cy_uint left = get_accessor_from_indices((cy_uint[]){j-1, i}, dp_dims, 2);

				if(str1_data[i - 1] == str0_data[j - 1])
					dp[curr] = dp[top_left];
				else
					dp[curr] = min3(1 + dp[top_left], // replace
									1 + dp[top],      // insert in str1
									1 + dp[left] );   // insert in str0
			}
		}
	}

	// result at the last element
	cy_uint result = dp[(dp_dims[0] * dp_dims[1]) - 1];

	deallocate(DSTRING_mem_alloc, dp, dp_bytes);

	return result;
}

#define max3(a,b,c) max((a),(max((b),(c))))

cy_uint length_of_longest_common_subsequence(const dstring* str0, const dstring* str1)
{
	const char* str0_data = get_byte_array_dstring(str0);
	cy_uint str0_len = get_char_count_dstring(str0);

	const char* str1_data = get_byte_array_dstring(str1);
	cy_uint str1_len = get_char_count_dstring(str1);

	cy_uint dp_dims[] = {str0_len + 1, str1_len + 1};
	cy_uint dp_bytes = sizeof(cy_uint) * dp_dims[0] * dp_dims[1];
	cy_uint* dp = aligned_allocate(DSTRING_mem_alloc, &dp_bytes, _Alignof(cy_uint));

	// on failure, return a CY_UINT_MAX
	if(dp == NULL)
		return CY_UINT_MAX;

	for(cy_uint i = 0; i < dp_dims[1]; i++)
	{
		for(cy_uint j = 0; j < dp_dims[0]; j++)
		{
			cy_uint curr = get_accessor_from_indices((cy_uint[]){j, i}, dp_dims, 2);

			if(i == 0)
				dp[curr] = 0;
			else if(j == 0)
				dp[curr] = 0;
			else
			{
				cy_uint top_left = get_accessor_from_indices((cy_uint[]){j-1, i-1}, dp_dims, 2);
				cy_uint top = get_accessor_from_indices((cy_uint[]){j, i-1}, dp_dims, 2);
				cy_uint left = get_accessor_from_indices((cy_uint[]){j-1, i}, dp_dims, 2);

				if(str1_data[i - 1] == str0_data[j - 1])
					dp[curr] = 1 + dp[top_left];
				else
					dp[curr] = max3(dp[top_left],
									dp[top],
									dp[left] );
			}
		}
	}

	// result at the last element
	cy_uint result = dp[(dp_dims[0] * dp_dims[1]) - 1];

	deallocate(DSTRING_mem_alloc, dp, dp_bytes);

	return result;
}