#include<bitmap.h>

#include<stdio.h>
#include<string.h>

int get_bit(const char* bitmap, unsigned int index)
{
	return (bitmap[index/8] >> (index % 8)) & 0x01;
}

void set_bit(char* bitmap, unsigned int index)
{
	bitmap[index/8] |= (1<<(index % 8));
}

void reset_bit(char* bitmap, unsigned int index)
{
	bitmap[index/8] &= ~(1<<(index % 8));
}

void set_all_bits(char* bitmap, unsigned int size)
{
	unsigned int bitmap_size = bitmap_size_in_bytes(size);
	for(unsigned int i = 0; i < bitmap_size; i++)
		bitmap[i] = 0xff;
}

void reset_all_bits(char* bitmap, unsigned int size)
{
	unsigned int bitmap_size = bitmap_size_in_bytes(size);
	for(unsigned int i = 0; i < bitmap_size; i++)
		bitmap[i] = 0;
}

void print_bitmap(const char* bitmap, unsigned int size)
{
	for(unsigned int i = 0; i < size; i++)
	{
		if(i)
			printf(" ");
		printf("%d", get_bit(bitmap, i));
	}
	printf("\n");
}

unsigned int bitmap_size_in_bytes(unsigned int size)
{
	return (size/8) + ((size%8)?1:0);
}

unsigned int find_first_set(const char* bitmap, unsigned int start_index, unsigned int size)
{
	unsigned int byte_index = start_index/8;
	unsigned int bytes_in_bitmap = bitmap_size_in_bytes(size);

	while(byte_index < bytes_in_bitmap)
	{
		if(bitmap[byte_index])
		{
			// ffs functions returns value between 1 <-> 2^n
			unsigned int bit_index = (byte_index*8) + (ffs(bitmap[byte_index]) - 1);

			// bit index valid only if it is between returnable bounds
			if(start_index <= bit_index && bit_index < size)
				return bit_index;
		}
		byte_index++;
	}

	// no bit between the range is set, return size (i.e. a value tht is out of returnable bounds)
	return size;
}