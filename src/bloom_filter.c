#include<bloom_filter.h>

#include<bitmap.h>
#include<multi_dim_array_util.h>

#include<dstring.h>
#include<cutlery_math.h>
#include<memory_allocator_interface.h>

int initialize_bloom_filter(bloom_filter* bf_p, cy_uint bucket_count, cy_uint hash_functions_count, const data_hash_func data_hash_functions[])
{
	// no allocators provided initialize with STC_C_mem_allocator
	return initialize_bloom_filter_with_allocator(bf_p, bucket_count, hash_functions_count, data_hash_functions, STD_C_mem_allocator, STD_C_mem_allocator);
}

int initialize_bloom_filter_with_allocator(bloom_filter* bf_p, cy_uint bucket_count, cy_uint hash_functions_count, const data_hash_func data_hash_functions[], memory_allocator data_hash_functions_array_allocator, memory_allocator bitmap_allocator)
{
	// the bits_in_bitmap must not overflow the cy_uint datatype
	if(will_unsigned_mul_overflow(cy_uint, bucket_count, hash_functions_count))
		return 0;

	cy_uint bits_in_bitmap = bucket_count * hash_functions_count;

	// number of bits in bitmap must not be 0
	if(bits_in_bitmap == 0)
		return 0;

	// initialize array (of data_hash_func-s) and populate it with data_hash_functions
	if(!initialize_array_with_allocator(&(bf_p->data_hash_functions), hash_functions_count, data_hash_functions_array_allocator))
		return 0;
	for(cy_uint i = 0; i < hash_functions_count; i++)
		set_in_array(&(bf_p->data_hash_functions), data_hash_functions[i], i);

	bf_p->bitmap_allocator = bitmap_allocator;
	bf_p->capacity_in_bytes = bitmap_size_in_bytes(bits_in_bitmap);

	bf_p->bitmap = zallocate(bf_p->bitmap_allocator, &(bf_p->capacity_in_bytes));
	if(bf_p->bitmap == NULL)
	{
		bf_p->capacity_in_bytes = 0;
		deinitialize_array(&(bf_p->data_hash_functions));
		return 0;
	}

	return 1;
}

int initialize_bloom_filter_with_memory(bloom_filter* bf_p, cy_uint bucket_count, cy_uint hash_functions_count, const data_hash_func data_hash_functions[], char bitmap[])
{
	// the bits_in_bitmap must not overflow the cy_uint datatype
	if(will_unsigned_mul_overflow(cy_uint, bucket_count, hash_functions_count))
		return 0;

	cy_uint bits_in_bitmap = bucket_count * hash_functions_count;

	// number of bits in bitmap must not be 0
	if(bits_in_bitmap == 0)
		return 0;

	// initialize array with memory
	if(!initialize_array_with_memory(&(bf_p->data_hash_functions), hash_functions_count, (const void**)data_hash_functions))
		return 0;

	bf_p->bitmap_allocator = NULL;
	bf_p->capacity_in_bytes = bitmap_size_in_bytes(bits_in_bitmap);

	if(bitmap != NULL)
		bf_p->bitmap = bitmap;
	else
	{
		bf_p->bitmap_allocator = STD_C_mem_allocator;
		bf_p->bitmap = zallocate(bf_p->bitmap_allocator, &(bf_p->capacity_in_bytes));
		if(bf_p->bitmap == NULL)
		{
			bf_p->capacity_in_bytes = 0;
			deinitialize_array(&(bf_p->data_hash_functions));
			return 0;
		}
	}

	return 1;
}

cy_uint get_hash_function_count_bloom_filter(const bloom_filter* bf_p)
{
	return get_capacity_array(&(bf_p->data_hash_functions));
}

cy_uint get_bucket_count_per_hash_function_bloom_filter(const bloom_filter* bf_p)
{
	return (bf_p->capacity_in_bytes * CHAR_BIT) / get_hash_function_count_bloom_filter(bf_p);
}

void insert_in_bloom_filter(bloom_filter* bf_p, const void* data, cy_uint data_size)
{
	cy_uint hash_functions_count = get_hash_function_count_bloom_filter(bf_p);
	cy_uint bucket_count = get_bucket_count_per_hash_function_bloom_filter(bf_p);

	for(cy_uint h = 0; h < hash_functions_count; h++)
	{
		data_hash_func hash_function = get_from_array(&(bf_p->data_hash_functions), h);

		cy_uint bucket_no = hash_function(data, data_size) % bucket_count;

		cy_uint bit_index = get_accessor_from_indices((const cy_uint []){bucket_no, h}, (const cy_uint []){bucket_count, hash_functions_count}, 2);

		set_bit(bf_p->bitmap, bit_index);
	}
}

bloom_filter_presence exists_in_bloom_filter(const bloom_filter* bf_p, const void* data, cy_uint data_size)
{
	cy_uint hash_functions_count = get_hash_function_count_bloom_filter(bf_p);
	cy_uint bucket_count = get_bucket_count_per_hash_function_bloom_filter(bf_p);

	bloom_filter_presence result = MAY_BE_PRESENT;

	for(cy_uint h = 0; (h < hash_functions_count) && (result == MAY_BE_PRESENT); h++)
	{
		data_hash_func hash_function = get_from_array(&(bf_p->data_hash_functions), h);

		cy_uint bucket_no = hash_function(data, data_size) % bucket_count;

		cy_uint bit_index = get_accessor_from_indices((const cy_uint []){bucket_no, h}, (const cy_uint []){bucket_count, hash_functions_count}, 2);

		result = result && get_bit(bf_p->bitmap, bit_index);
	}

	return result;
}

void reset_bits_in_bloom_filter(bloom_filter* bf_p)
{
	// set all of the memory pointed to by bf_p->bitmap to 0
	memory_set(bf_p->bitmap, 0, bf_p->capacity_in_bytes);
}

double get_fraction_of_bloom_filter_bits_set(const bloom_filter* bf_p)
{
	cy_uint hash_functions_count = get_hash_function_count_bloom_filter(bf_p);
	cy_uint bucket_count = get_bucket_count_per_hash_function_bloom_filter(bf_p);

	cy_uint total_bits = hash_functions_count * bucket_count;

	// count the number of bits that are set
	cy_uint bits_set = 0;
	for(cy_uint b = 0; b < total_bits; b++)
		bits_set += get_bit(bf_p->bitmap, b);

	return ((double)(bits_set)) / ((double)(total_bits));
}

void sprint_bloom_filter_bitmap(dstring* append_str, const bloom_filter* bf_p, unsigned int tabs)
{
	cy_uint hash_functions_count = get_hash_function_count_bloom_filter(bf_p);
	cy_uint bucket_count = get_bucket_count_per_hash_function_bloom_filter(bf_p);

	for(cy_uint h = 0; h < hash_functions_count; h++)
	{
		sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "h(%" PRIu_cy_uint ") -> ", h);

		for(cy_uint b = 0; b < bucket_count; b++)
		{
			cy_uint bit_index = get_accessor_from_indices((const cy_uint []){b, h}, (const cy_uint []){bucket_count, hash_functions_count}, 2);

			snprintf_dstring(append_str, "%d ", get_bit(bf_p->bitmap, bit_index));
		}
		snprintf_dstring(append_str, "\n");
	}
}

void deinitialize_bloom_filter(bloom_filter* bf_p)
{
	if(bf_p->bitmap_allocator != NULL && bf_p->capacity_in_bytes > 0)
		deallocate(bf_p->bitmap_allocator, bf_p->bitmap, bf_p->capacity_in_bytes);
	bf_p->bitmap_allocator = NULL;
	bf_p->bitmap = NULL;
	bf_p->capacity_in_bytes = 0;
	deinitialize_array(&(bf_p->data_hash_functions));
}