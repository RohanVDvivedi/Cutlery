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
		sum_chars += ((unsigned int)(*str));
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
	set_bit(bloomfilter_bitmap[0], hash_f_1(str));
	set_bit(bloomfilter_bitmap[1], hash_f_2(str));
}

find_result check_if_inserted_in_bloomfilter(const char* str)
{
	return ( get_bit(bloomfilter_bitmap[0], hash_f_1(str)) 
		& get_bit(bloomfilter_bitmap[1], hash_f_2(str)) )  ? MAY_BE_PRESENT : NOT_PRESENT;
}

int main()
{

}