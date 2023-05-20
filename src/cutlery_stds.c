#include<cutlery_stds.h>

#include<stdio.h>

static const cy_uint int_size = (sizeof(int)); // we assume that int is always 2^n bytes in size, i.e. 1,2,4,8 etc
static const cy_uint int_bits_size = (sizeof(int) * CHAR_BIT); // CHAR_BIT must be 8
static const cy_uint int_alignment_bit_mask = (-(sizeof(int)));	// 0b 111...111 000...000 where 0s represent alignment

// on a 32 bit int system
// int_size = 4
// int_bits_size = 32
// int_alignment_bis_mask = 0xfffffffc (i.e. 2's complement of 4 OR binary representation of -4)

void memory_move(void* dest_start, const void* src_start, cy_uint size)
{
	// if they are the same memory locations, or if the copy size if 0, skip the copy operation
	if(src_start == dest_start || size == 0)
		return;

	// compute the last src and dest byte address that needs to be copied
	const void* src_end = src_start + size;
	void* dest_end = dest_start + size;

	// this conditions may arise if we happen to be at edge of the memory
	if(src_start >= src_end || dest_start >= dest_end)
		return;

	// decide to make a forward pass or backward pass 
	// based on whether the pass could corrupt the src memory address before we could copy it

	// make forward pass
	if( dest_start < src_start )
	{
		// initalize our iterators for forward copy
		const char* src = src_start;
		char* dest = dest_start;

		// check if int copy is possible, and requires atleast 3 iterations (atleast 2 int copy iterations)
		if( (size >= 3 * int_size) &&
			(
				(((cy_uint)src_start) & ~int_alignment_bit_mask) == (((cy_uint)dest_start) & ~int_alignment_bit_mask)
			)
		)
		{
			// perform a byte-by-byte copy until the addresses are int aligned
			while( ( ((cy_uint)src) & ~int_alignment_bit_mask ) )
				*(dest++) = *(src++);

			// perform an int-by-int transfer in this scope
			// dest_int and src_int must not leave this scope
			// they must equal the dest and src, right before and right after this scope
			{
				const int* src_int = (int*)src;
				int* dest_int = (int*)dest;

				// additonal bytes that you might have to copy after completing the int copy
				cy_uint additional_bytes = ((cy_uint)(src_end)) & ~int_alignment_bit_mask;

				// this is the address of the last byte that must be copied under the int-by-int copy loop
				const int* src_end_int = src_end - additional_bytes;

				// int-by-int copy loop
				while( src_int != src_end_int )
					*(dest_int++) = *(src_int++);

				src = (char*)src_int;
				dest = (char*)dest_int;
			}
		}

		// finish up remaining with an old fashioned byte-by-byte copy loop
		while( src != ((char*)(src_end)) )
			*(dest++) = *(src++);
	}
	// else make backward pass
	else
	{
		// initalize our iterators for backward copy
		const char* src = src_end;
		char* dest = dest_end;

		// check if int copy is possible, and requires atleast 3 iterations (atleast 2 int copy iterations)
		if( (size >= 3 * int_size) &&
			(
				(((cy_uint)src_start) & ~int_alignment_bit_mask) == (((cy_uint)dest_start) & ~int_alignment_bit_mask)
			)
		)
		{
			// perform a byte-by-byte copy until the addresses are int - 1 aligned
			while( ( ((cy_uint)src) & ~int_alignment_bit_mask ) )
				*(--dest) = *(--src);

			// perform an int-by-int transfer in this scope
			// dest_int and src_int must not leave this scope
			// they must equal the dest and src, right before and right after this scope
			{
				const int* src_int = (int*)src;
				int* dest_int = (int*)dest;

				// additonal bytes that you might have to copy after completing the int copy
				cy_uint additional_bytes = (int_size - (((cy_uint)(src_start)) & ~int_alignment_bit_mask)) & ~int_alignment_bit_mask;

				// this is the address of the last byte that must be copied under the int-by-int copy loop
				const int* src_start_int = src_start + additional_bytes;

				// int-by-int copy loop
				while( src_int != src_start_int )
					*(--dest_int) = *(--src_int);

				src = (char*)src_int;
				dest = (char*)dest_int;
			}
		}

		// finish up remaining with an old fashioned byte-by-byte copy loop
		while( src != ((char*)(src_start)) )
			*(--dest) = *(--src);
	}
}

void memory_set(void* dest_start, char byte_value, cy_uint size)
{
	// if the copy size is zero, skip the copy operation
	if(size == 0)
		return;

	// compute the last dest byte address that needs to be copied
	void* dest_end = dest_start + size;

	// this conditions may arise if we happen to be at edge of the memory
	if(dest_start >= dest_end)
		return;

	// initalize our iterators for the copy operation
	char* dest = dest_start;

	// check if int copy is possible, and requires atleast 3 iterations (atleast 2 int copy iterations)
	if(size >= 3 * int_size)
	{
		// perform a byte-by-byte copy until the address is int aligned
		while( ( ((cy_uint)dest) & ~int_alignment_bit_mask ) )
			*(dest++) = byte_value;

		// perform an int-by-int transfer in this scope
		// dest_int must not leave this scope
		// it must equal the dest, right before and right after this scope
		{
			int* dest_int = (int*)dest;

			// generate the int_value that we could copy to the int locations int-by-int
			int int_value = 0;
			for(unsigned int i = 0; i < int_bits_size; i += CHAR_BIT)
				int_value |= ( (((int)byte_value) & 0xff) << i );

			// additonal bytes that you might have to copy after completing the int copy
			cy_uint additional_bytes = ((cy_uint)(dest_end)) & ~int_alignment_bit_mask;

			// this is the address of the last byte that must be copied under the int-by-int copy loop
			const int* dest_end_int = dest_end - additional_bytes;

			// int-by-int copy loop
			while( dest_int != dest_end_int )
				*(dest_int++) = int_value;

			dest = (char*)dest_int;
		}
	}

	// finish up remaining with an old fashioned byte-by-byte copy
	while( dest != ((char*)(dest_end)) )
		*(dest++) = byte_value;
}

int memory_compare(const void* data1_start, const void* data2_start, cy_uint size)
{
	// if they are the same memory locations, or if the size if 0, then they are equal
	if(data1_start == data2_start || size == 0)
		return 0;

	// compute the end of data1 and data2 byte
	const void* data1_end = data1_start + size;
	const void* data2_end = data2_start + size;

	// this conditions may arise if we happen to be at edge of the memory
	if(data1_start >= data1_end || data2_start >= data2_end)
		return 0;

	// initalize our iterators for forward compare
	const char* data1 = data1_start;
	const char* data2 = data2_start;

	// check if int compare is possible, and requires atleast 3 iterations (atleast 2 int copy iterations)
	if( (size >= 3 * int_size) &&
		(
			(((cy_uint)data1_start) & ~int_alignment_bit_mask) == (((cy_uint)data2_start) & ~int_alignment_bit_mask)
		)
	)
	{
		// perform a byte-by-byte compare until the addresses are int aligned
		while( ( ((cy_uint)data1) & ~int_alignment_bit_mask ) )
		{
			if((*data1) > (*data2))
				return 1;
			else if((*data1) < (*data2))
				return -1;
			else
			{
				data1++;
				data2++;
			}
		}

		// perform an int-by-int compare in this scope
		// data1_int and data2_int must not leave this scope
		// they must equal the data1 and data2, right before and right after this scope
		{
			const int* data1_int = (int*)data1;
			const int* data2_int = (int*)data2;

			// additonal bytes that you might have to compare after completing the int copy
			cy_uint additional_bytes = ((cy_uint)(data1_end)) & ~int_alignment_bit_mask;

			// this is the address of the last byte that must be compared under the int-by-int compare loop
			const int* data1_end_int = data1_end - additional_bytes;

			// int-by-int compare loop
			while( data1_int != data1_end_int )
			{
				if( *(data2_int) == *(data1_int) )
				{
					data1_int++;
					data2_int++;
				}
				else
					break;
			}

			data1 = (char*)data1_int;
			data2 = (char*)data2_int;
		}
	}

	// finish up remaining with an old fashioned byte-by-byte compare loop
	while( data1 != ((char*)(data1_end)) )
	{
		if((*data1) > (*data2))
			return 1;
		else if((*data1) < (*data2))
			return -1;
		else
		{
			data1++;
			data2++;
		}
	}

	return 0;
}

int memory_swap(void* data1_start, void* data2_start, cy_uint size)
{
	// if they are the same memory locations, or if the size if 0, then swap can not be performed
	if(data1_start == data2_start || size == 0)
		return 0;

	// compute the end of data1 and data2 byte address
	void* data1_end = data1_start + size;
	void* data2_end = data2_start + size;

	// this conditions may arise if we happen to be at edge of the memory
	if(data1_start >= data1_end || data2_start >= data2_end)
		return 0;

	// make sure there is no overlap in data1 and data2
	if((data1_start <= data2_start && data2_start < data1_end) || (data2_start <= data1_start && data1_start < data2_end))
		return 0;

	// initalize our iterators for forward swap
	char* data1 = data1_start;
	char* data2 = data2_start;

	// check if int compare is possible, and requires atleast 3 iterations (atleast 2 int copy iterations)
	if( (size >= 3 * int_size) &&
		(
			(((cy_uint)data1_start) & ~int_alignment_bit_mask) == (((cy_uint)data2_start) & ~int_alignment_bit_mask)
		)
	)
	{
		// perform a byte-by-byte swap until the addresses are int aligned
		while( ( ((cy_uint)data1) & ~int_alignment_bit_mask ) )
		{
			(*data1) ^= (*data2);
			(*data2) ^= (*data1);
			(*data1) ^= (*data2);
			data1++;
			data2++;
		}

		// perform an int-by-int compare in this scope
		// data1_int and data2_int must not leave this scope
		// they must equal the data1 and data2, right before and right after this scope
		{
			int* data1_int = (int*)data1;
			int* data2_int = (int*)data2;

			// additonal bytes that you might have to compare after completing the int copy
			cy_uint additional_bytes = ((cy_uint)(data1_end)) & ~int_alignment_bit_mask;

			// this is the address of the last byte that must be compared under the int-by-int compare loop
			const int* data1_end_int = data1_end - additional_bytes;

			// int-by-int compare loop
			while( data1_int != data1_end_int )
			{
				(*data1_int) ^= (*data2_int);
				(*data2_int) ^= (*data1_int);
				(*data1_int) ^= (*data2_int);
				data1_int++;
				data2_int++;
			}

			data1 = (char*)data1_int;
			data2 = (char*)data2_int;
		}
	}

	// finish up remaining with an old fashioned byte-by-byte compare loop
	while( data1 != ((char*)(data1_end)) )
	{
		(*data1) ^= (*data2);
		(*data2) ^= (*data1);
		(*data1) ^= (*data2);
		data1++;
		data2++;
	}

	return 1;
}