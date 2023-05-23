#include<bitmap.h>

int get_bit(const char* bitmap, cy_uint index)
{
	return (bitmap[X_div_8(index)] >> (X_mod_8(index))) & 0x01;
}

void set_bit(char* bitmap, cy_uint index)
{
	bitmap[X_div_8(index)] |= (1<<(X_mod_8(index)));
}

void reset_bit(char* bitmap, cy_uint index)
{
	bitmap[X_div_8(index)] &= ~(1<<(X_mod_8(index)));
}

void set_all_bits(char* bitmap, cy_uint size)
{
	// number of whole bytes in the bitmap
	cy_uint bitmap_whole_byte_size = X_div_8(size);
	
	// set all bits in the complete bytes
	for(cy_uint i = 0; i < bitmap_whole_byte_size; i++)
		bitmap[i] = (~('\x0'));

	// set necessary bits in the last partial byte
	for(cy_uint i = X_mul_8(bitmap_whole_byte_size); i < size; i++)
		set_bit(bitmap, i);
}

void reset_all_bits(char* bitmap, cy_uint size)
{
	// number of whole bytes in the bitmap
	cy_uint bitmap_whole_byte_size = X_div_8(size);
	
	// reset all bits in the complete bytes
	for(cy_uint i = 0; i < bitmap_whole_byte_size; i++)
		bitmap[i] = '\x0';

	// set necessary bits in the last partial byte
	for(cy_uint i = X_mul_8(bitmap_whole_byte_size); i < size; i++)
		reset_bit(bitmap, i);
}

void sprint_bitmap(dstring* append_str, const char* bitmap, cy_uint size, unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs);
	for(cy_uint i = 0; i < size; i++)
	{
		if(i)
			snprintf_dstring(append_str, " ");
		else
			snprintf_dstring(append_str, "Bitmap: ");
		snprintf_dstring(append_str, "%d", get_bit(bitmap, i));
	}
	snprintf_dstring(append_str, "\n");
}

cy_uint bitmap_size_in_bytes(cy_uint size)
{
	// size/8 => gives us the number of complete bytes of data
	// note: size/8 => floor(((double)size)/8)
	// (size%8)?1:0 => gives us 1 or 0 based on whether there is any partial byte with leser than 8 bits
	// return (size/8) + ((size%8)?1:0);          // -- version 1
	// return X_div_8(size) + (X_mod_8(size)>0);  // -- version 2
	return X_div_8(size + 7);					  // -- version 3
}

#include<string.h>

cy_uint find_first_set(const char* bitmap, cy_uint start_index, cy_uint size)
{
	cy_uint byte_index = X_div_8(start_index);
	cy_uint bytes_in_bitmap = bitmap_size_in_bytes(size);

	while(byte_index < bytes_in_bitmap)
	{
		if(bitmap[byte_index])
		{
			// ffs functions returns value between 1 <-> 8
			cy_uint bit_index = X_mul_8(byte_index) + (ffs(bitmap[byte_index]) - 1);

			// bit index valid only if it is between returnable bounds
			if(start_index <= bit_index && bit_index < size)
				return bit_index;
		}
		byte_index++;
	}

	// no bit between the range is set, return size (i.e. a value tht is out of returnable bounds)
	return size;
}