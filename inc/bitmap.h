#ifndef BITMAP_H
#define BITMAP_H

// char* bitmap      => first pointer adddress of the bitmap
// unsigned int size => size of the bitmap in number of bits
// unsigned int index => bit location in the bitmap (starting with 0)

int get_bit(const char* bitmap, unsigned int index);

void set_bit(char* bitmap, unsigned int index);

void reset_bit(char* bitmap, unsigned int index);

void set_all_bits(char* bitmap, unsigned int size);

void reset_all_bits(char* bitmap, unsigned int size);

void print_bitmap(const char* bitmap, unsigned int size);

unsigned int bitmap_size_in_bytes(unsigned int size);

// unsigned int start_index => bit location in the bitmap to start checking from
// returns a least index of the bit set to 1, that is between start_index and size-1
// if no bit is set in the given range return size (size is out-of-bound index)
unsigned int find_first_set(const char* bitmap, unsigned int start_index, unsigned int size);

#endif