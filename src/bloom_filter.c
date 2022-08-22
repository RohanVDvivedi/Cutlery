#include<bloom_filter.h>

#include<bitmap.h>
#include<multi_dim_array_util.h>

#include<cutlery_stds.h>
#include<memory_allocator_interface.h>

void initialize_bloom_filter(bloom_filter* bf_p, unsigned int bucket_count, unsigned int hash_functions_count, data_hash_func* data_hash_functions)
{
	bf_p->bucket_count = bucket_count;

	// initialize array (of data_hash_func-s) and populate it with data_hash_functions
	initialize_array(&(bf_p->data_hash_functions), hash_functions_count);
	for(unsigned int i = 0; i < hash_functions_count; i++)
		set_in_array(&(bf_p->data_hash_functions), data_hash_functions[i], i);

	// calculate the total number of bits required in bitmap
	// and then number of bytes required for storing the bitmap
	unsigned int bits_in_bitmap = bucket_count * hash_functions_count;
	unsigned int bytes_in_bitmap = bitmap_size_in_bytes(bits_in_bitmap);

	if(bytes_in_bitmap == 0)
		bf_p->bitmap = NULL;
	else
		bf_p->bitmap = zallocate(STD_C_mem_allocator, &bytes_in_bitmap);
}

void insert_in_bloom_filter(bloom_filter* bf_p, const void* data, unsigned int data_size)
{
	// TODO
}

bloom_filter_presence exists_in_bloom_filter(const bloom_filter* bf_p, const void* data, unsigned int data_size)
{
	// TODO
}

void deinitialize_bloom_filter(bloom_filter* bf_p)
{
	// calculate the total number of bits required in bitmap
	// and then number of bytes required for storing the bitmap
	unsigned int bits_in_bitmap = bf_p->bucket_count * get_capacity_array(&(bf_p->data_hash_functions));
	unsigned int bytes_in_bitmap = bitmap_size_in_bytes(bits_in_bitmap);

	deallocate(STD_C_mem_allocator, bf_p->bitmap, bytes_in_bitmap);

	deinitialize_array(&(bf_p->data_hash_functions));
}