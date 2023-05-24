#include<stdio.h>
#include<string.h>

#include<count_min_sketch.h>

#define COUNT_MIN_SKETCH_BUCKET_COUNT     (16)

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

void print_countminsketch(const count_min_sketch* cms_p)
{
	dstring str;
	init_dstring(&str, "", 0);
	sprint_count_min_sketch(&str, cms_p, 0);
	printf_dstring(&str);
	deinit_dstring(&str);
	printf("\n");
}

void increment_and_print_frequency(count_min_sketch* cms_p, char* str)
{
	printf("%s -> %" PRIu_cy_uint "\n", str, increment_frequency_in_count_min_sketch(cms_p, str, -1));
}

void get_and_print_frequency(const count_min_sketch* cms_p, char* str)
{
	printf("%s -> %" PRIu_cy_uint "\n", str, get_frequency_from_count_min_sketch(cms_p, str, -1));
}

int main()
{
	count_min_sketch cms_temp;
	count_min_sketch* cms_p = &cms_temp;


	//initialize_count_min_sketch(cms_p, COUNT_MIN_SKETCH_BUCKET_COUNT, 2, (const data_hash_func[]){hash_f_1, hash_f_2});

	// OR to avoid a new array allocation for hash_functions inside the count_min_sketch, use below initialization

	// observe how we can get away with passing NULL for frequencies (the last parameter)
	initialize_count_min_sketch_with_memory(cms_p, COUNT_MIN_SKETCH_BUCKET_COUNT, 2, (const data_hash_func[]){hash_f_1, hash_f_2}, NULL);

	increment_and_print_frequency(cms_p, "hello");

	increment_and_print_frequency(cms_p, "hello");

	increment_and_print_frequency(cms_p, "rohan");

	increment_and_print_frequency(cms_p, "hello");

	increment_and_print_frequency(cms_p, "rohan");

	increment_and_print_frequency(cms_p, "dvivedi");

	increment_and_print_frequency(cms_p, "rohan dvivedi");

	increment_and_print_frequency(cms_p, "dvivedi");

	increment_and_print_frequency(cms_p, "rupa");

	increment_and_print_frequency(cms_p, "vipul");

	increment_and_print_frequency(cms_p, "devashree");

	increment_and_print_frequency(cms_p, "manan");

	print_countminsketch(cms_p);

	get_and_print_frequency(cms_p, "hello");

	get_and_print_frequency(cms_p, "eeloo");

	get_and_print_frequency(cms_p, "rohan");

	get_and_print_frequency(cms_p, "dvivedi");

	get_and_print_frequency(cms_p, "rohan dvivedi");

	get_and_print_frequency(cms_p, "rupa");

	get_and_print_frequency(cms_p, "vipul");

	get_and_print_frequency(cms_p, "devashree");

	get_and_print_frequency(cms_p, "manan");

	get_and_print_frequency(cms_p, "Rohan V");

	get_and_print_frequency(cms_p, "Rohan Xvivedi");

	printf("reseting frequencies\n");
	reset_frequencies_in_count_min_sketch(cms_p);

	print_countminsketch(cms_p);

	get_and_print_frequency(cms_p, "hello");

	get_and_print_frequency(cms_p, "eeloo");

	get_and_print_frequency(cms_p, "rohan");

	get_and_print_frequency(cms_p, "dvivedi");

	get_and_print_frequency(cms_p, "rohan dvivedi");

	get_and_print_frequency(cms_p, "rupa");

	get_and_print_frequency(cms_p, "vipul");

	get_and_print_frequency(cms_p, "devashree");

	get_and_print_frequency(cms_p, "manan");

	get_and_print_frequency(cms_p, "Rohan V");

	get_and_print_frequency(cms_p, "Rohan Xvivedi");

	deinitialize_count_min_sketch(cms_p);

	return 0;
}