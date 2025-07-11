#include<cutlery/bitmap.h>

unsigned long long int get_bits(const char* bitmap, cy_uint start_index, cy_uint last_index)
{
	// fail if the indices are not valid
	if(start_index > last_index || (last_index - start_index + 1) > (sizeof(unsigned long long int) * CHAR_BIT))
		return 0;

	unsigned long long int res = 0;

	cy_uint i = start_index;
	cy_uint j = 0;

	// end index to copy from
	cy_uint end_index = last_index + 1;

	// copy first bits, bit by bit while the copy from index is not 8 bit aligned
	for(; (i < end_index) && ((i % CHAR_BIT) > 0); i++, j++)
		res = res | (((bitmap[i / CHAR_BIT] >> (i % CHAR_BIT)) & 1ULL) << j);

	// copy middle whole bytes, byte by byte
	for(; (i < UINT_ALIGN_DOWN(end_index, CHAR_BIT)); i+=CHAR_BIT, j+=CHAR_BIT)
		res = res | ((bitmap[i / CHAR_BIT] & 0xffULL) << j);

	// copy remaining trailing bits, bit by bit
	for(; i < end_index; i++, j++)
		res = res | (((bitmap[i / CHAR_BIT] >> (i % CHAR_BIT)) & 1ULL) << j);

	return res;
}

int set_bits(char* bitmap, cy_uint start_index, cy_uint last_index, unsigned long long int value)
{
	// fail if the indices are not valid
	if(start_index > last_index || (last_index - start_index + 1) > (sizeof(unsigned long long int) * CHAR_BIT))
		return 0;

	cy_uint i = start_index;
	cy_uint j = 0;

	// end index to copy to
	cy_uint end_index = last_index + 1;

	// copy first bits, bit by bit while the copy from index is not 8 bit aligned
	for(; (i < end_index) && ((i % CHAR_BIT) > 0); i++, j++)
	{
		// get bit value of jth bit in value
		int bit_value = (value >> j) & 1;

		// if it is 1, set the ith bit
		if(bit_value == 1)
			bitmap[i / CHAR_BIT] |= ('\x1' << (i % CHAR_BIT));
		else // else reset the ith bit
			bitmap[i / CHAR_BIT] &= ~('\x1' << (i % CHAR_BIT));
	}

	// copy middle whole bytes, byte by byte
	for(; (i < UINT_ALIGN_DOWN(end_index, CHAR_BIT)); i+=CHAR_BIT, j+=CHAR_BIT)
		bitmap[i / CHAR_BIT] = ((value >> j) & '\xff');

	// copy remaining trailing bits, bit by bit
	for(; i < end_index; i++, j++)
	{
		// get bit value of jth bit in value
		int bit_value = (value >> j) & 1;

		// if it is 1, set the ith bit
		if(bit_value == 1)
			bitmap[i / CHAR_BIT] |= ('\x1' << (i % CHAR_BIT));
		else // else reset the ith bit
			bitmap[i / CHAR_BIT] &= ~('\x1' << (i % CHAR_BIT));
	}

	return 1;
}

void set_all_bits(char* bitmap, cy_uint size)
{
	// number of whole bytes in the bitmap
	cy_uint bitmap_whole_byte_size = size / CHAR_BIT;
	
	// set all bits in the complete bytes
	for(cy_uint i = 0; i < bitmap_whole_byte_size; i++)
		bitmap[i] = (~('\x0'));

	// set necessary bits in the last partial byte
	for(cy_uint i = (bitmap_whole_byte_size * CHAR_BIT); i < size; i++)
		set_bit(bitmap, i);
}

void reset_all_bits(char* bitmap, cy_uint size)
{
	// number of whole bytes in the bitmap
	cy_uint bitmap_whole_byte_size = size / CHAR_BIT;
	
	// reset all bits in the complete bytes
	for(cy_uint i = 0; i < bitmap_whole_byte_size; i++)
		bitmap[i] = '\x0';

	// set necessary bits in the last partial byte
	for(cy_uint i = (bitmap_whole_byte_size * CHAR_BIT); i < size; i++)
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

// this function is a looping replica of std library's ffs, it has been implemented only to avoid reliance on std c library
// using ffs would have been a better choice performance wise
// returns position (1-indexed) position of the least significant set bit, just like the std library's ffs
// returns 0 if no bit is set
int ffs_cutlery(int v)
{
	if(v == 0)
		return 0;

	for(int b = 0; b < sizeof(int) * CHAR_BIT; b++)
	{
		if((v >> b) & 1)
			return (b + 1); // returns 1-indexed position of the bit
	}

	return 0;
}

cy_uint find_first_set(const char* bitmap, cy_uint start_index, cy_uint size)
{
	cy_uint byte_index = start_index / CHAR_BIT;
	cy_uint bytes_in_bitmap = bitmap_size_in_bytes(size);

	while(byte_index < bytes_in_bitmap)
	{
		if(bitmap[byte_index])
		{
			// ffs functions returns value between 1 <-> CHAR_BIT (both inclusive) and 0 if not bit is set
			cy_uint bit_index = (byte_index * CHAR_BIT) + (ffs_cutlery(bitmap[byte_index]) - 1);

			// bit index valid only if it is between returnable bounds
			if(start_index <= bit_index && bit_index < size)
				return bit_index;
		}
		byte_index++;
	}

	// no bit between the range is set, return size (i.e. a value tht is out of returnable bounds)
	return size;
}