#include<count_min_sketch.h>

#include<multi_dim_array_util.h>

#include<dstring.h>
#include<cutlery_stds.h>
#include<cutlery_math.h>
#include<memory_allocator_interface.h>

int initialize_count_min_sketch(count_min_sketch* cms_p, cy_uint bucket_count, cy_uint hash_functions_count, const data_hash_func data_hash_functions[])
{
	// no allocators provided initialize with STC_C_mem_allocator
	return initialize_count_min_sketch_with_allocator(cms_p, bucket_count, hash_functions_count, data_hash_functions, STD_C_mem_allocator, STD_C_mem_allocator);
}

int initialize_count_min_sketch_with_allocator(count_min_sketch* cms_p, cy_uint bucket_count, cy_uint hash_functions_count, const data_hash_func data_hash_functions[], memory_allocator data_hash_functions_array_allocator, memory_allocator frequencies_allocator)
{
	// the total_buckets must not be 0, and the total_buckets must not overflow CY_UINT_MAX, and it must be lesser than the max bucket count possible on your machine
	cy_uint total_buckets = bucket_count * hash_functions_count;
	if(total_buckets == 0 || total_buckets < bucket_count || total_buckets < hash_functions_count || total_buckets > MAX_COUNT_MIN_SKETCH_TOTAL_BUCKET_COUNT)
		return 0;

	// initialize array (of data_hash_func-s) and populate it with data_hash_functions
	if(!initialize_array_with_allocator(&(cms_p->data_hash_functions), hash_functions_count, data_hash_functions_array_allocator))
		return 0;
	for(cy_uint i = 0; i < hash_functions_count; i++)
		set_in_array(&(cms_p->data_hash_functions), data_hash_functions[i], i);

	cms_p->frequencies_allocator = frequencies_allocator;
	cms_p->capacity_in_bytes = total_buckets * sizeof(cy_uint);

	cms_p->frequencies = zallocate(cms_p->frequencies_allocator, &(cms_p->capacity_in_bytes));
	if(cms_p->frequencies == NULL)
	{
		cms_p->capacity_in_bytes = 0;
		deinitialize_array(&(cms_p->data_hash_functions));
		return 0;
	}

	return 1;
}

int initialize_count_min_sketch_with_memory(count_min_sketch* cms_p, cy_uint bucket_count, cy_uint hash_functions_count, const data_hash_func data_hash_functions[], cy_uint frequencies[])
{
	// the total_buckets must not be 0, and the total_buckets must not overflow CY_UINT_MAX, and it must be lesser than the max bucket count possible on your machine
	cy_uint total_buckets = bucket_count * hash_functions_count;
	if(total_buckets == 0 || total_buckets < bucket_count || total_buckets < hash_functions_count || total_buckets > MAX_COUNT_MIN_SKETCH_TOTAL_BUCKET_COUNT)
		return 0;

	// initialize array with memory
	initialize_array_with_memory(&(cms_p->data_hash_functions), hash_functions_count, (const void**)data_hash_functions);

	cms_p->frequencies_allocator = NULL;
	cms_p->capacity_in_bytes = total_buckets * sizeof(cy_uint);

	if(frequencies != NULL)
		cms_p->frequencies = frequencies;
	else
	{
		cms_p->frequencies_allocator = STD_C_mem_allocator;
		cms_p->frequencies = zallocate(cms_p->frequencies_allocator, &(cms_p->capacity_in_bytes));
		if(cms_p->frequencies == NULL)
		{
			deinitialize_array(&(cms_p->data_hash_functions));
			return 0;
		}
	}

	return 1;
}

cy_uint get_hash_function_count_count_min_sketch(const count_min_sketch* cms_p)
{
	return get_capacity_array(&(cms_p->data_hash_functions));
}

// this is the number of buckets per hash function in the count min sketch
// equivalent to (cms_p->capacity_in_bytes / sizeof(cy_uint)) / get_hash_function_count_bloom_filter(bf_p)
cy_uint get_bucket_count_per_hash_function_count_min_sketch(const count_min_sketch* cms_p)
{
	return (cms_p->capacity_in_bytes / sizeof(cy_uint)) / get_hash_function_count_count_min_sketch(cms_p);
}

// incrementing frequency by 1, while avoiding overflow
static cy_uint increment_frequency_by_1(cy_uint frequency)
{
	if(frequency == CY_UINT_MAX)
		return frequency;
	return frequency + 1;
}


// returns frequency of a certain data element, and the bucket numbers concerned for each of the hash_functions
// the bucket_indices must point to hash_functions_count number of cy_uints
// where bucket_indices[h] is the index of the bucket number concerned for the given data for the h-th hash_function
static cy_uint get_frequency_and_concerned_bucket_indices_from_count_min_sketch(const count_min_sketch* cms_p, const void* data, cy_uint data_size, cy_uint* bucket_indices)
{
	cy_uint hash_functions_count = get_hash_function_count_count_min_sketch(cms_p);
	cy_uint bucket_count = get_bucket_count_per_hash_function_count_min_sketch(cms_p);

	// result frequency
	cy_uint result = CY_UINT_MAX;

	// we iterate over all the hash functions
	for(cy_uint h = 0; (h < hash_functions_count); h++)
	{
		data_hash_func hash_function = get_from_array(&(cms_p->data_hash_functions), h);

		cy_uint bucket_no = hash_function(data, data_size) % bucket_count;

		bucket_indices[h] = get_accessor_from_indices((const cy_uint []){bucket_no, h}, (const cy_uint []){bucket_count, hash_functions_count}, 2);

		result = min(result, cms_p->frequencies[bucket_indices[h]]);
	}

	return result;
}

cy_uint increment_frequency_in_count_min_sketch(count_min_sketch* cms_p, const void* data, cy_uint data_size)
{
	cy_uint hash_functions_count = get_hash_function_count_count_min_sketch(cms_p);

	// we calculate the current max_frequency (of data)
	// and additionally we cache all the buckets we touched in bucket_indices array
	// this above caching allows us to avoid recalculation of all the hash functions and their corresponding accessible 2-d -> 1d coversion indices

	cy_uint bucket_indices_size = sizeof(cy_uint) * ((cy_uint)hash_functions_count);
	cy_uint* bucket_indices = allocate(STD_C_mem_allocator, &bucket_indices_size);
	
	// get the frequency and the concerned bucket_nos
	cy_uint frequency = get_frequency_and_concerned_bucket_indices_from_count_min_sketch(cms_p, data, data_size, bucket_indices);

	// we iterate over all the hash functions, and increment frequency if it matches the max frequency
	for(cy_uint h = 0; (h < hash_functions_count); h++)
	{
		if(cms_p->frequencies[bucket_indices[h]] == frequency)
			cms_p->frequencies[bucket_indices[h]] = increment_frequency_by_1(cms_p->frequencies[bucket_indices[h]]);
	}

	// deallocate bucket_indices
	deallocate(STD_C_mem_allocator, bucket_indices, bucket_indices_size);

	return increment_frequency_by_1(frequency);
}

cy_uint get_frequency_from_count_min_sketch(const count_min_sketch* cms_p, const void* data, cy_uint data_size)
{
	cy_uint hash_functions_count = get_hash_function_count_count_min_sketch(cms_p);
	cy_uint bucket_count = get_bucket_count_per_hash_function_count_min_sketch(cms_p);

	// result frequency
	cy_uint result = CY_UINT_MAX;

	// in the below for loop, the minimum value result can have is 0
	// hence we save some cycles by exiting the loop early if result == 0
	for(cy_uint h = 0; (h < hash_functions_count) && (result > 0); h++)
	{
		data_hash_func hash_function = get_from_array(&(cms_p->data_hash_functions), h);

		cy_uint bucket_no = hash_function(data, data_size) % bucket_count;

		cy_uint index = get_accessor_from_indices((const cy_uint []){bucket_no, h}, (const cy_uint []){bucket_count, hash_functions_count}, 2);

		result = min(result, cms_p->frequencies[index]);
	}

	return result;
}

void reset_frequencies_in_count_min_sketch(count_min_sketch* cms_p)
{
	memory_set(cms_p->frequencies, 0, cms_p->capacity_in_bytes);
}

void sprint_count_min_sketch(dstring* append_str, const count_min_sketch* cms_p, unsigned int tabs)
{
	cy_uint hash_functions_count = get_hash_function_count_count_min_sketch(cms_p);
	cy_uint bucket_count = get_bucket_count_per_hash_function_count_min_sketch(cms_p);

	for(cy_uint h = 0; h < hash_functions_count; h++)
	{
		sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "h(%" PRIu_cy_uint ") -> ", h);

		for(cy_uint b = 0; b < bucket_count; b++)
		{
			cy_uint index = get_accessor_from_indices((const cy_uint []){b, h}, (const cy_uint []){bucket_count, hash_functions_count}, 2);

			snprintf_dstring(append_str, "%" PRIu_cy_uint " ", cms_p->frequencies[index]);
		}
		snprintf_dstring(append_str, "\n");
	}
}

void deinitialize_count_min_sketch(count_min_sketch* cms_p)
{
	if(cms_p->frequencies_allocator != NULL && cms_p->capacity_in_bytes > 0)
		deallocate(cms_p->frequencies_allocator, cms_p->frequencies, cms_p->capacity_in_bytes);
	cms_p->frequencies = NULL;
	cms_p->capacity_in_bytes = 0;
	cms_p->frequencies_allocator = NULL;
	deinitialize_array(&(cms_p->data_hash_functions));
}