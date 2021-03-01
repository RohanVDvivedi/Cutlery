#include<stdio.h>
#include<string.h>

#include<bitmap.h>

#define BLOOM_FILTER_SIZE_IN_BITS     (16)
#define BLOOM_FILTER_SIZE_IN_BYTES    ((BLOOM_FILTER_SIZE_IN_BITS / 8) + ((BLOOM_FILTER_SIZE_IN_BITS % 8) > 0 ? 1 : 0))

char bloomfilter_bitmap[2][BLOOM_FILTER_SIZE_IN_BYTES];

unsigned int hash_f_1(char* str)
{
	return strlen(str) % BLOOM_FILTER_SIZE_IN_BITS;
}

unsigned int hash_f_2(char* str)
{
	unsigned int sum_chars = 0;
	while((*str) != '\0')
	{
		sum_chars += (((unsigned int)(*str)) - ((('a'<=((unsigned int)(*str)))&&(((unsigned int)(*str))<='z'))?'a':'A'));
		str++;
	}
	return sum_chars % BLOOM_FILTER_SIZE_IN_BITS;
}

enum find_result
{
	NOT_PRESENT     = 0,
	MAY_BE_PRESENT  = 1
};
typedef enum find_result find_result;

void insert_in_bloomfilter(const char* str)
{
	unsigned int hash1 = hash_f_1(str);
	unsigned int hash2 = hash_f_2(str);
	printf("Bloom filter insert : %s\n\thash1 : %u\n\thash2 : %u\n\n", str, hash1, hash2);
	set_bit(bloomfilter_bitmap[0], hash1);
	set_bit(bloomfilter_bitmap[1], hash2);
}

find_result check_if_inserted_in_bloomfilter(const char* str)
{
	find_result fresult = get_bit(bloomfilter_bitmap[0], hash_f_1(str)) 
						& get_bit(bloomfilter_bitmap[1], hash_f_2(str));
	printf("Bloom filter check  : %s : %s\n\n", str, (fresult?"MAY_BE_PRESENT":"NOT_PRESENT"));
	return fresult ? MAY_BE_PRESENT : NOT_PRESENT;
}

void print_bloomfilter()
{
	printf("bitmap 0 : ");
	print_bitmap(bloomfilter_bitmap[0], BLOOM_FILTER_SIZE_IN_BITS);
	printf("bitmap 1 : ");
	print_bitmap(bloomfilter_bitmap[1], BLOOM_FILTER_SIZE_IN_BITS);
	printf("\n\n");
}

int main()
{
	insert_in_bloomfilter("hello");

	print_bloomfilter();

	check_if_inserted_in_bloomfilter("hello");

	check_if_inserted_in_bloomfilter("eeloo");

	check_if_inserted_in_bloomfilter("rohan");

	check_if_inserted_in_bloomfilter("dvivedi");

	return 0;
}