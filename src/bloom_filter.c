#include<bloom_filter.h>

#include<bitmap.h>
#include<multi_dim_array_util.h>

#include<dstring.h>
#include<cutlery_stds.h>
#include<memory_allocator_interface.h>

static unsigned int get_bitmap_size_in_bits_for_bloom_filter(const bloom_filter* bf_p)
{
	// calculate the total number of bits required in bitmap
	return bf_p->bucket_count * get_capacity_array(&(bf_p->data_hash_functions));
}

static unsigned int get_bitmap_size_in_bytes_for_bloom_filter(const bloom_filter* bf_p)
{
	unsigned int bits_in_bitmap = get_bitmap_size_in_bits_for_bloom_filter(bf_p);

	// and then number of bytes required for storing the bitmap
	return bitmap_size_in_bytes(bits_in_bitmap);
}

int initialize_bloom_filter(bloom_filter* bf_p, unsigned int bucket_count, unsigned int hash_functions_count, const data_hash_func data_hash_functions[])
{
	// no allocators provided initialize with STC_C_mem_allocator
	return initialize_bloom_filter_with_allocator(bf_p, bucket_count, hash_functions_count, data_hash_functions, STD_C_mem_allocator, STD_C_mem_allocator);
}

int initialize_bloom_filter_with_allocator(bloom_filter* bf_p, unsigned int bucket_count, unsigned int hash_functions_count, const data_hash_func data_hash_functions[], memory_allocator data_hash_functions_array_allocator, memory_allocator bitmap_allocator)
{
	// initialize array (of data_hash_func-s) and populate it with data_hash_functions
	if(!initialize_array_with_allocator(&(bf_p->data_hash_functions), hash_functions_count, data_hash_functions_array_allocator))
		return 0;
	for(unsigned int i = 0; i < hash_functions_count; i++)
		set_in_array(&(bf_p->data_hash_functions), data_hash_functions[i], i);

	bf_p->bucket_count = bucket_count;
	bf_p->bitmap_allocator = bitmap_allocator;

	// calculate  number of bytes required for storing the bitmap
	cy_uint bytes_in_bitmap = get_bitmap_size_in_bytes_for_bloom_filter(bf_p);

	if(bytes_in_bitmap == 0)
		bf_p->bitmap = NULL;
	else
	{
		bf_p->bitmap = zallocate(bf_p->bitmap_allocator, &bytes_in_bitmap);
		if(bf_p->bitmap == NULL)
		{
			deinitialize_array(&(bf_p->data_hash_functions));
			return 0;
		}
	}

	return 1;
}

void initialize_bloom_filter_with_memory(bloom_filter* bf_p, unsigned int bucket_count, unsigned int hash_functions_count, const data_hash_func data_hash_functions[], char bitmap[])
{
	// initialize array with memory
	initialize_array_with_memory(&(bf_p->data_hash_functions), hash_functions_count, (const void**)data_hash_functions);

	bf_p->bitmap_allocator = NULL;
	bf_p->bucket_count = bucket_count;

	// calculate  number of bytes required for storing the bitmap
	cy_uint bytes_in_bitmap = get_bitmap_size_in_bytes_for_bloom_filter(bf_p);

	if(bytes_in_bitmap == 0)
		bf_p->bitmap = NULL;
	else if(bitmap != NULL)
		bf_p->bitmap = bitmap;
	else
	{
		bf_p->bitmap_allocator = STD_C_mem_allocator;
		bf_p->bitmap = zallocate(bf_p->bitmap_allocator, &bytes_in_bitmap);
	}
}

void insert_in_bloom_filter(bloom_filter* bf_p, const void* data, unsigned int data_size)
{
	unsigned int hash_functions_count = get_capacity_array(&(bf_p->data_hash_functions));
	unsigned int bucket_count = bf_p->bucket_count;

	for(unsigned int h = 0; h < hash_functions_count; h++)
	{
		data_hash_func hash_function = get_from_array(&(bf_p->data_hash_functions), h);

		unsigned int bucket_no = hash_function(data, data_size) % bucket_count;

		unsigned int bit_index = get_accessor_from_indices((const unsigned int []){bucket_no, h}, (const unsigned int []){bucket_count, hash_functions_count}, 2);

		set_bit(bf_p->bitmap, bit_index);
	}
}

bloom_filter_presence exists_in_bloom_filter(const bloom_filter* bf_p, const void* data, unsigned int data_size)
{
	unsigned int hash_functions_count = get_capacity_array(&(bf_p->data_hash_functions));
	unsigned int bucket_count = bf_p->bucket_count;

	bloom_filter_presence result = MAY_BE_PRESENT;

	for(unsigned int h = 0; (h < hash_functions_count) && (result == MAY_BE_PRESENT); h++)
	{
		data_hash_func hash_function = get_from_array(&(bf_p->data_hash_functions), h);

		unsigned int bucket_no = hash_function(data, data_size) % bucket_count;

		unsigned int bit_index = get_accessor_from_indices((const unsigned int []){bucket_no, h}, (const unsigned int []){bucket_count, hash_functions_count}, 2);

		result = result && get_bit(bf_p->bitmap, bit_index);
	}

	return result;
}

double get_fraction_of_bloom_filter_bits_set(const bloom_filter* bf_p)
{
	unsigned int total_bits = get_bitmap_size_in_bits_for_bloom_filter(bf_p);

	// count the number of bits that are set
	unsigned int bits_set = 0;
	for(unsigned int b = 0; b < total_bits; b++)
		bits_set += get_bit(bf_p->bitmap, b);

	return ((double)(bits_set)) / ((double)(total_bits));
}

void sprint_bloom_filter_bitmap(dstring* append_str, const bloom_filter* bf_p, unsigned int tabs)
{
	unsigned int hash_functions_count = get_capacity_array(&(bf_p->data_hash_functions));
	unsigned int bucket_count = bf_p->bucket_count;

	for(unsigned int h = 0; h < hash_functions_count; h++)
	{
		sprint_chars(append_str, '\t', tabs); snprintf_dstring(append_str, "h(%u) -> ", h);

		for(unsigned int b = 0; b < bucket_count; b++)
		{
			unsigned int bit_index = get_accessor_from_indices((const unsigned int []){b, h}, (const unsigned int []){bucket_count, hash_functions_count}, 2);

			snprintf_dstring(append_str, "%d ", get_bit(bf_p->bitmap, bit_index));
		}
		snprintf_dstring(append_str, "\n");
	}
}

void deinitialize_bloom_filter(bloom_filter* bf_p)
{
	cy_uint bytes_in_bitmap = get_bitmap_size_in_bytes_for_bloom_filter(bf_p);

	if(bf_p->bitmap_allocator != NULL && bytes_in_bitmap > 0)
		deallocate(bf_p->bitmap_allocator, bf_p->bitmap, bytes_in_bitmap);

	deinitialize_array(&(bf_p->data_hash_functions));
}