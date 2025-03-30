#include<stdio.h>

#include<cutlery/bitmap.h>

void print_bitmap(const char* bitmap, unsigned int bitmap_complete_size)
{
	dstring str;
	init_dstring(&str, "", 0);
	sprint_bitmap(&str, bitmap, bitmap_complete_size, 0);
	printf_dstring(&str);
	deinit_dstring(&str);
	printf("\n");
}

int main()
{
	char bitmap[5];
	unsigned int bitmap_complete_size = 40;
	reset_all_bits(bitmap, 40);

	printf("SET 28\n");
	set_all_bits(bitmap, 28);
	print_bitmap(bitmap, bitmap_complete_size);
	set_all_bits(bitmap, 40);

	printf("RESET 29\n");
	reset_all_bits(bitmap, 29);
	print_bitmap(bitmap, bitmap_complete_size);
	reset_all_bits(bitmap, 40);

	printf("SET 30\n");
	set_all_bits(bitmap, 30);
	print_bitmap(bitmap, bitmap_complete_size);
	set_all_bits(bitmap, 40);

	printf("RESET 31\n");
	reset_all_bits(bitmap, 31);
	print_bitmap(bitmap, bitmap_complete_size);
	reset_all_bits(bitmap, 40);

	printf("SET 32\n");
	set_all_bits(bitmap, 32);
	print_bitmap(bitmap, bitmap_complete_size);
	set_all_bits(bitmap, 40);

	printf("RESET 33\n");
	reset_all_bits(bitmap, 33);
	print_bitmap(bitmap, bitmap_complete_size);
	reset_all_bits(bitmap, 40);

	printf("SET 34\n");
	set_all_bits(bitmap, 34);
	print_bitmap(bitmap, bitmap_complete_size);
	set_all_bits(bitmap, 40);

	printf("RESET 35\n");
	reset_all_bits(bitmap, 35);
	print_bitmap(bitmap, bitmap_complete_size);
	reset_all_bits(bitmap, 40);

	printf("SET 36\n");
	set_all_bits(bitmap, 36);
	print_bitmap(bitmap, bitmap_complete_size);
	set_all_bits(bitmap, 40);

	printf("RESET 37\n");
	reset_all_bits(bitmap, 37);
	print_bitmap(bitmap, bitmap_complete_size);
	reset_all_bits(bitmap, 40);

	printf("SET 38\n");
	set_all_bits(bitmap, 38);
	print_bitmap(bitmap, bitmap_complete_size);
	set_all_bits(bitmap, 40);

	// set pattern
	{
		printf("setting pattern \"Rohn\" in ascii in bits 4 to 35\n");
		const char* str = "Rohn";
		set_bits(bitmap, 3, 34, *((unsigned long long int*)str));
	}

	// get pattern
	{
		unsigned long long int value = get_bits(bitmap, 3, 34);
		printf("pattern read \"%.*s\"\n", 4, (const char*)(&value));
	}

	return 0;
}