#include<bitmap.h>

#include<string.h>

// UTILITY - OPTIMIZED - start

// X_mul_8(x) ==> x * 8
#define X_mul_8(x) ((x)<<3)

// X_div_8(x) ==> x / 8
#define X_div_8(x) ((x)>>3)

// X_mod_8(x) ==> x % 8
#define X_mod_8(x) ((x)&7)

// UTILITY - end



int get_bit(const char* bitmap, unsigned int index)
{
	return (bitmap[X_div_8(index)] >> (X_mod_8(index))) & 0x01;
}

void set_bit(char* bitmap, unsigned int index)
{
	bitmap[X_div_8(index)] |= (1<<(X_mod_8(index)));
}

void reset_bit(char* bitmap, unsigned int index)
{
	bitmap[X_div_8(index)] &= ~(1<<(X_mod_8(index)));
}

void set_all_bits(char* bitmap, unsigned int size)
{
	// number of whole bytes in the bitmap
	unsigned int bitmap_whole_byte_size = X_div_8(size);
	
	// set all bits in the complete bytes
	for(unsigned int i = 0; i < bitmap_whole_byte_size; i++)
		bitmap[i] = 0xff;

	// set necessary bits in the last partial byte
	for(unsigned int i = X_mul_8(bitmap_whole_byte_size); i < size; i++)
		set_bit(bitmap, i);
}

void reset_all_bits(char* bitmap, unsigned int size)
{
	// number of whole bytes in the bitmap
	unsigned int bitmap_whole_byte_size = X_div_8(size);
	
	// reset all bits in the complete bytes
	for(unsigned int i = 0; i < bitmap_whole_byte_size; i++)
		bitmap[i] = 0x00;

	// set necessary bits in the last partial byte
	for(unsigned int i = X_mul_8(bitmap_whole_byte_size); i < size; i++)
		reset_bit(bitmap, i);
}

void sprint_bitmap(dstring* append_str, const char* bitmap, unsigned int size, unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs);
	for(unsigned int i = 0; i < size; i++)
	{
		if(i)
			snprintf_dstring(append_str, " ");
		else
			snprintf_dstring(append_str, "Bitmap: ");
		snprintf_dstring(append_str, "%d", get_bit(bitmap, i));
	}
	snprintf_dstring(append_str, "\n");
}

unsigned int bitmap_size_in_bytes(unsigned int size)
{
	// size/8 => gives us the number of complete bytes of data
	// note: size/8 => floor(((double)size)/8)
	// (size%8)?1:0 => gives us 1 or 0 based on whether there is any partial byte with leser than 8 bits
	// return (size/8) + ((size%8)?1:0);          // -- version 1
	// return X_div_8(size) + (X_mod_8(size)>0);  // -- version 2
	return X_div_8(size + 7);					  // -- version 3
}

unsigned int find_first_set(const char* bitmap, unsigned int start_index, unsigned int size)
{
	unsigned int byte_index = X_div_8(start_index);
	unsigned int bytes_in_bitmap = bitmap_size_in_bytes(size);

	while(byte_index < bytes_in_bitmap)
	{
		if(bitmap[byte_index])
		{
			// ffs functions returns value between 1 <-> 8
			unsigned int bit_index = X_mul_8(byte_index) + (ffs(bitmap[byte_index]) - 1);

			// bit index valid only if it is between returnable bounds
			if(start_index <= bit_index && bit_index < size)
				return bit_index;
		}
		byte_index++;
	}

	// no bit between the range is set, return size (i.e. a value tht is out of returnable bounds)
	return size;
}