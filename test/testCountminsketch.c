#include<stdio.h>
#include<string.h>

#include<count_min_sketch.h>

#define COUNT_MIN_SKETCH_BUCKET_COUNT     (16)

unsigned int hash_f_1(const void* data, unsigned int ununsed)
{
	const char* str = data;
	return strlen(str);
}

unsigned int hash_f_2(const void* data, unsigned int ununsed)
{
	const char* str = data;
	unsigned int sum_chars = 0;
	while((*str) != '\0')
	{
		sum_chars += (((unsigned int)(*str)) - ((('a'<=((unsigned int)(*str)))&&(((unsigned int)(*str))<='z'))?'a':'A'));
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
	printf("%s -> %u\n", str, increment_frequency_in_count_min_sketch(cms_p, str, -1));
}

void get_and_print_frequency(const count_min_sketch* cms_p, char* str)
{
	printf("%s -> %u\n", str, get_frequency_from_count_min_sketch(cms_p, str, -1));
}

int main()
{
	count_min_sketch cms_temp;
	count_min_sketch* cms_p = &cms_temp;

	initialize_count_min_sketch(cms_p, COUNT_MIN_SKETCH_BUCKET_COUNT, 2, (const data_hash_func[]){hash_f_1, hash_f_2});

	increment_and_print_frequency(cms_p, "hello");

	print_countminsketch(cms_p);

	get_and_print_frequency(cms_p, "hello");

	get_and_print_frequency(cms_p, "eeloo");

	increment_and_print_frequency(cms_p, "hello");

	get_and_print_frequency(cms_p, "hello");

	get_and_print_frequency(cms_p, "eeloo");

	get_and_print_frequency(cms_p, "rohan");

	get_and_print_frequency(cms_p, "dvivedi");

	return 0;
}