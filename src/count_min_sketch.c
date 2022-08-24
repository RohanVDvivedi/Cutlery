#include<count_min_sketch.h>

#include<multi_dim_array_util.h>

#include<dstring.h>
#include<cutlery_stds.h>
#include<cutlery_math.h>
#include<memory_allocator_interface.h>

static unsigned int get_total_bucket_count_for_count_min_sketch(const count_min_sketch* cms_p)
{
	return cms_p->bucket_count * get_capacity_array(&(cms_p->data_hash_functions));
}

void initialize_count_min_sketch(count_min_sketch* cms_p, unsigned int bucket_count, unsigned int hash_functions_count, const data_hash_func* data_hash_functions)
{
	cms_p->bucket_count = bucket_count;

	// initialize array (of data_hash_func-s) and populate it with data_hash_functions
	initialize_array(&(cms_p->data_hash_functions), hash_functions_count);
	for(unsigned int i = 0; i < hash_functions_count; i++)
		set_in_array(&(cms_p->data_hash_functions), data_hash_functions[i], i);

	// calculate total number of buckets required
	unsigned int total_bucket_count = get_total_bucket_count_for_count_min_sketch(cms_p);
	unsigned int total_bytes_for_all_buckets = total_bucket_count * sizeof(unsigned int);

	if(total_bytes_for_all_buckets == 0)
		cms_p->frequencies = NULL;
	else
		cms_p->frequencies = zallocate(STD_C_mem_allocator, &total_bytes_for_all_buckets);
}


// incrementing frequency by 1, while avoiding overflow
static unsigned int increment_frequency_by_1(unsigned int frequency)
{
	if(frequency == UINT_MAX)
		return frequency;
	return frequency + 1;
}


// returns frequency of a certain data element, and the bucket numbers concerned for each of the hash_functions
// the bucket_nos must point to hash_functions_count number of unsigned ints
static unsigned int get_frequency_and_concerned_bucket_nos_from_count_min_sketch(const count_min_sketch* cms_p, const void* data, unsigned int data_size, unsigned int* bucket_nos)
{
	unsigned int hash_functions_count = get_capacity_array(&(cms_p->data_hash_functions));
	unsigned int bucket_count = cms_p->bucket_count;

	// result frequency
	unsigned int result = UINT_MAX;

	// we iterate over all the hash functions
	for(unsigned int h = 0; (h < hash_functions_count); h++)
	{
		data_hash_func hash_function = get_from_array(&(cms_p->data_hash_functions), h);

		bucket_nos[h] = hash_function(data, data_size) % bucket_count;

		unsigned int index = get_accessor_from_indices((const unsigned int []){bucket_nos[h], h}, (const unsigned int []){bucket_count, hash_functions_count}, 2);

		result = min(result, cms_p->frequencies[index]);
	}

	return result;
}

unsigned int increment_frequency_in_count_min_sketch(count_min_sketch* cms_p, const void* data, unsigned int data_size)
{
	unsigned int hash_functions_count = get_capacity_array(&(cms_p->data_hash_functions));
	unsigned int bucket_count = cms_p->bucket_count;

	unsigned int* bucket_nos = malloc(sizeof(unsigned int) * hash_functions_count);
	
	// get the maximum frequency and the concerned bucket_nos
	unsigned int max_frequency = get_frequency_and_concerned_bucket_nos_from_count_min_sketch(cms_p, data, data_size, bucket_nos);

	// we iterate over all the hash functions, and increment frequency if it matches the max frequency
	for(unsigned int h = 0; (h < hash_functions_count); h++)
	{
		if(cms_p->frequencies[h][bucket_nos[h]] == max_frequency)
			cms_p->frequencies[h][bucket_nos[h]] = increment_frequency_by_1(cms_p->frequencies[h][bucket_nos[h]]);
	}

	return max_frequency + 1;
}

unsigned int get_frequency_from_count_min_sketch(const count_min_sketch* cms_p, const void* data, unsigned int data_size)
{
	unsigned int hash_functions_count = get_capacity_array(&(cms_p->data_hash_functions));
	unsigned int bucket_count = cms_p->bucket_count;

	// result frequency
	unsigned int result = UINT_MAX;

	// in the below for loop, the minimum value result can have is 0
	// hence we save some cycles by exiting the loop early if result == 0
	for(unsigned int h = 0; (h < hash_functions_count) && (result > 0); h++)
	{
		data_hash_func hash_function = get_from_array(&(cms_p->data_hash_functions), h);

		unsigned int bucket_no = hash_function(data, data_size) % bucket_count;

		unsigned int index = get_accessor_from_indices((const unsigned int []){bucket_no, h}, (const unsigned int []){bucket_count, hash_functions_count}, 2);

		result = min(result, cms_p->frequencies[index]);
	}

	return result;
}

void reset_frequencies_in_count_min_sketch(count_min_sketch* cms_p)
{
	// calculate total number of buckets required
	unsigned int total_bucket_count = get_total_bucket_count_for_count_min_sketch(cms_p);
	unsigned int total_bytes_for_all_buckets = total_bucket_count * sizeof(unsigned int);

	memory_set(cms_p->frequencies, 0, total_bytes_for_all_buckets);
}

void sprint_count_min_sketch(dstring* append_str, const count_min_sketch* cms_p, unsigned int tabs)
{
	unsigned int hash_functions_count = get_capacity_array(&(cms_p->data_hash_functions));
	unsigned int bucket_count = cms_p->bucket_count;

	for(unsigned int h = 0; h < hash_functions_count; h++)
	{
		sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "h(%u) -> ", h);

		for(unsigned int b = 0; b < bucket_count; b++)
		{
			unsigned int index = get_accessor_from_indices((const unsigned int []){b, h}, (const unsigned int []){bucket_count, hash_functions_count}, 2);

			snprintf_dstring(append_str, "%u ", cms_p->frequencies[index]);
		}
		snprintf_dstring(append_str, "\n");
	}
}

void deinitialize_count_min_sketch(count_min_sketch* cms_p)
{
	// calculate total number of buckets required
	unsigned int total_bucket_count = get_total_bucket_count_for_count_min_sketch(cms_p);
	unsigned int total_bytes_for_all_buckets = total_bucket_count * sizeof(unsigned int);
	deallocate(STD_C_mem_allocator, cms_p->frequencies, total_bytes_for_all_buckets);

	deinitialize_array(&(cms_p->data_hash_functions));
}