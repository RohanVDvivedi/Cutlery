#include<count_min_sketch.h>

#include<multi_dim_array_util.h>

#include<dstring.h>
#include<cutlery_stds.h>
#include<memory_allocator_interface.h>

void initialize_count_min_sketch(count_min_sketch* cms_p, unsigned int bucket_count, unsigned int hash_functions_count, const data_hash_func* data_hash_functions)
{
	// TODO
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
	// TODO
}

void sprint_count_min_sketch(dstring* append_str, const count_min_sketch* cms_p, unsigned int tabs)
{
	// TODO
}

void deinitialize_count_min_sketch(count_min_sketch* cms_p)
{
	// TODO
}