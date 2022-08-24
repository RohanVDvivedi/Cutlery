#include<count_min_sketch.h>

#include<multi_dim_array_util.h>

#include<dstring.h>
#include<cutlery_stds.h>
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

void increment_frequency_in_count_min_sketch(count_min_sketch* cms_p, const void* data, unsigned int data_size)
{
	// TODO
}

unsigned int get_frequency_from_count_min_sketch(const count_min_sketch* cms_p, const void* data, unsigned int data_size)
{
	// TODO
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
	// TODO
}

void deinitialize_count_min_sketch(count_min_sketch* cms_p)
{
	// calculate total number of buckets required
	unsigned int total_bucket_count = get_total_bucket_count_for_count_min_sketch(cms_p);
	unsigned int total_bytes_for_all_buckets = total_bucket_count * sizeof(unsigned int);
	deallocate(STD_C_mem_allocator, cms_p->frequencies, total_bytes_for_all_buckets);

	deinitialize_array(&(cms_p->data_hash_functions));
}