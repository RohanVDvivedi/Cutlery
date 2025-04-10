#include<stdio.h>
#include<string.h>

#include<cutlery/bloom_filter.h>

#define BLOOM_FILTER_BUCKET_COUNT     (16)

cy_uint hash_f_1(const void* data, cy_uint ununsed)
{
	const char* str = data;
	return strlen(str);
}

cy_uint hash_f_2(const void* data, cy_uint ununsed)
{
	const char* str = data;
	cy_uint sum_chars = 0;
	while((*str) != '\0')
	{
		sum_chars += (((cy_uint)(*str)) - ((('a'<=((cy_uint)(*str)))&&(((cy_uint)(*str))<='z'))?'a':'A'));
		str++;
	}
	return sum_chars;
}

char* result_values[] = {"ABSENT", "MAY_BE_PRESENT"};

void print_bloomfilter(const bloom_filter* bf_p)
{
	dstring str;
	init_dstring(&str, "", 0);
	sprint_bloom_filter_bitmap(&str, bf_p, 0);
	printf_dstring(&str);
	deinit_dstring(&str);
	printf("\n");
}

void print_exists_in_bloom_filter(const bloom_filter* bf_p, const char* str)
{
	printf("%s -> %s\n", str, result_values[exists_in_bloom_filter(bf_p, str, -1)]);
	printf("\n");
}

int main()
{
	bloom_filter bf_temp;
	bloom_filter* bf_p = &bf_temp;

	//initialize_bloom_filter(bf_p, BLOOM_FILTER_BUCKET_COUNT, 2, (const data_hash_func[]){hash_f_1, hash_f_2});

	// OR to avoid a new array allocation for hash_functions inside the bloom_filter, use below initialization

	// observe how we can get away with passing NULL for bitmap (the last parameter)
	initialize_bloom_filter_with_memory(bf_p, BLOOM_FILTER_BUCKET_COUNT, 2, (const data_hash_func[]){hash_f_1, hash_f_2}, NULL);

	insert_in_bloom_filter(bf_p, "hello", -1);

	insert_in_bloom_filter(bf_p, "rohan", -1);

	insert_in_bloom_filter(bf_p, "dvivedi", -1);

	insert_in_bloom_filter(bf_p, "rohan dvivedi", -1);

	insert_in_bloom_filter(bf_p, "vipul", -1);

	insert_in_bloom_filter(bf_p, "rupa", -1);

	insert_in_bloom_filter(bf_p, "devashree", -1);

	print_bloomfilter(bf_p);

	print_exists_in_bloom_filter(bf_p, "hello");

	print_exists_in_bloom_filter(bf_p, "eeloo");

	print_exists_in_bloom_filter(bf_p, "rohan");

	print_exists_in_bloom_filter(bf_p, "dvivedi");

	print_exists_in_bloom_filter(bf_p, "rohan dvivedi");

	print_exists_in_bloom_filter(bf_p, "rupa");

	print_exists_in_bloom_filter(bf_p, "vipul");

	print_exists_in_bloom_filter(bf_p, "vipulkumar");

	print_exists_in_bloom_filter(bf_p, "devashree");

	print_exists_in_bloom_filter(bf_p, "manan");

	print_exists_in_bloom_filter(bf_p, "Rohan V");

	print_exists_in_bloom_filter(bf_p, "Rohan Xvivedi");

	print_bloomfilter(bf_p);

	reset_bits_in_bloom_filter(bf_p);

	print_bloomfilter(bf_p);

	printf("percentage 1s in bloom_filter = %lf %%\n", 100.0 * get_fraction_of_bloom_filter_bits_set(bf_p));

	deinitialize_bloom_filter(bf_p);

	return 0;
}