#ifndef BITMAP_H
#define BITMAP_H

#include<cutlery/dstring.h>

// char* bitmap       => first pointer adddress of the bitmap
// cy_uint size  => size of the bitmap in number of bits
// cy_uint index => bit location in the bitmap (starting with 0)

static inline int get_bit(const char* bitmap, cy_uint index)
{
	return (bitmap[index / CHAR_BIT] >> (index % CHAR_BIT)) & '\x1';
}

static inline void set_bit(char* bitmap, cy_uint index)
{
	bitmap[index / CHAR_BIT] |= ('\x1' << (index % CHAR_BIT));
}

static inline void reset_bit(char* bitmap, cy_uint index)
{
	bitmap[index / CHAR_BIT] &= ~('\x1' << (index % CHAR_BIT));
}

// gets bits from the bitmap between indices, start_index to last_index both inclusive, as a single unsigned long long int
// number of bits in start_index to last_index must not exceed sizeof(unsigned long long int) * CHAR_BIT
unsigned long long int get_bits(const char* bitmap, cy_uint start_index, cy_uint last_index);

// sets bits from value in to bitmap from start_index to last_index both inclusive
// this set is not like the set/reset bit api, if the ith bit of valu is x, then it will make start_index + ith bit of bimap as x, it is literal set function
// number of bits in start_index to last_index must not exceed sizeof(unsigned long long int) * CHAR_BIT
int set_bits(char* bitmap, cy_uint start_index, cy_uint last_index, unsigned long long int value);

void set_all_bits(char* bitmap, cy_uint size);

void reset_all_bits(char* bitmap, cy_uint size);

void sprint_bitmap(dstring* append_str, const char* bitmap, cy_uint size, unsigned int tabs);

static inline cy_uint bitmap_size_in_bytes(cy_uint size)
{
	// (size / CHAR_BIT)     => gives us the number of complete bytes of data
	// (size % CHAR_BIT) > 0 => gives us 1 or 0 based on whether there is any partial byte with leser than CHAR_BIT bits
	return (size / CHAR_BIT) + ((size % CHAR_BIT) > 0);				// -- version 1
	// return (size + CHAR_BIT - 1) / CHAR_BIT;						// -- version 2 -- not overflow safe
}

// start_index => bit location in the bitmap to start checking from
// returns a least index of the bit set to 1, that is between start_index and size-1
// if no bit is set in the given range return size (size is out-of-bound index)
cy_uint find_first_set(const char* bitmap, cy_uint start_index, cy_uint size);

#endif