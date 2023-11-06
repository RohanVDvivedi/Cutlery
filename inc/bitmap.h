#ifndef BITMAP_H
#define BITMAP_H

#include<dstring.h>

// char* bitmap       => first pointer adddress of the bitmap
// cy_uint size  => size of the bitmap in number of bits
// cy_uint index => bit location in the bitmap (starting with 0)

int get_bit(const char* bitmap, cy_uint index);

void set_bit(char* bitmap, cy_uint index);

void reset_bit(char* bitmap, cy_uint index);

// gets bits from the bitmap as a single unsigned long long int
// number of bits in start_index to last_index must not exceed sizeof(unsigned long long int) * CHAR_BIT
unsigned long long int get_bits(const char* bitmap, cy_uint start_index, cy_uint last_index);

// sets bits from value in to bitmap from start_index to last_index
// number of bits in start_index to last_index must not exceed sizeof(unsigned long long int) * CHAR_BIT
int set_bits(const char* bitmap, cy_uint start_index, cy_uint last_index, unsigned long long int value);

void set_all_bits(char* bitmap, cy_uint size);

void reset_all_bits(char* bitmap, cy_uint size);

void sprint_bitmap(dstring* append_str, const char* bitmap, cy_uint size, unsigned int tabs);

cy_uint bitmap_size_in_bytes(cy_uint size);

// start_index => bit location in the bitmap to start checking from
// returns a least index of the bit set to 1, that is between start_index and size-1
// if no bit is set in the given range return size (size is out-of-bound index)
cy_uint find_first_set(const char* bitmap, cy_uint start_index, cy_uint size);

#endif