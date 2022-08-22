#include<bloom_filter.h>

#include<bitmap.h>
#include<multi_dim_array_util.h>

#include<cutlery_stds.h>
#include<memory_allocator_interface.h>

static unsigned int get_bitmap_size_in_bytes_for_bloom_filter(const bloom_filter* bf_p)
{
	// calculate the total number of bits required in bitmap
	// and then number of bytes required for storing the bitmap
	unsigned int bits_in_bitmap = bf_p->bucket_count * get_capacity_array(&(bf_p->data_hash_functions));
	unsigned int bytes_in_bitmap = bitmap_size_in_bytes(bits_in_bitmap);
	return bytes_in_bitmap;
}

void initialize_bloom_filter(bloom_filter* bf_p, unsigned int bucket_count, unsigned int hash_functions_count, data_hash_func* data_hash_functions)
{
	bf_p->bucket_count = bucket_count;

	// initialize array (of data_hash_func-s) and populate it with data_hash_functions
	initialize_array(&(bf_p->data_hash_functions), hash_functions_count);
	for(unsigned int i = 0; i < hash_functions_count; i++)
		set_in_array(&(bf_p->data_hash_functions), data_hash_functions[i], i);

	// calculate  number of bytes required for storing the bitmap
	unsigned int bytes_in_bitmap = get_bitmap_size_in_bytes_for_bloom_filter(bf_p);

	if(bytes_in_bitmap == 0)
		bf_p->bitmap = NULL;
	else
		bf_p->bitmap = zallocate(STD_C_mem_allocator, &bytes_in_bitmap);
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

void deinitialize_bloom_filter(bloom_filter* bf_p)
{
	unsigned int bytes_in_bitmap = get_bitmap_size_in_bytes_for_bloom_filter(bf_p);
	deallocate(STD_C_mem_allocator, bf_p->bitmap, bytes_in_bitmap);

	deinitialize_array(&(bf_p->data_hash_functions));
}