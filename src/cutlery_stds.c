#include<cutlery_stds.h>

static const unsigned int int_size = sizeof(int); // we assume that int is always 2^n bytes in size, i.e. 1,2,4,8 etc
static const unsigned int int_bits_size = sizeof(int) * 8;
static const unsigned int int_alignment_bit_mask = -((int)sizeof(int));	// 0b 111...111 000...000 where 0s represent alignment

// on a 32 bit int system
// int_size = 4
// int_bits_size = 32
// int_alignment_bis_mask = 0xfffffffc (i.e. 2's complement of 4 OR binary representation of -4)

void memory_move(void* dest_start, const void* src_start, unsigned int size)
{
	// if they are the same memory locations, or if the copy size if 0, skip the copy operation
	if(src_start == dest_start || size == 0)
		return;

	// the end of the src buffer and last src byte address that needs to be copied
	const void* src_end = src_start + size;
	const void* src_last = src_end - 1;

	// make forward pass
	// if there is no overlap or if the overlap doesnt corrupt the copy operation
	if(dest_start < src_start  || src_last < dest_start)
	{
		// intialize our iterators for forward copy
		char* dest = dest_start;
		const char* src = src_start;

		// check if int copy is possible, and requires atleast 3 iterations (atleast 2 int copy iterations)
		if( (size >= 3 * int_size) &&
			(
				(((long int)src_start) & ~int_alignment_bit_mask) == (((long int)dest_start) & ~int_alignment_bit_mask)
			)
		)
		{
			// perform a byte-by-byte copy until the address is int aligned
			while( ((long int)src) & ~int_alignment_bit_mask )
				*(dest++) = *(src++);

			// perform an int-by-int transfer in this scope
			// dest_int and src_int must not leave this scope
			// they must equal the dest and src, right before and right after this scope
			{
				int* dest_int = (int*)dest;
				const int* src_int = (int*)src;

				// additonal bytes that you might have to copy after completing the int copy
				unsigned long int additional_bytes = ((unsigned long int)src_end) & ~int_alignment_bit_mask;

				// if you hit this address, stop copying ints
				const int* src_end_int = src_end - additional_bytes;

				while( src_int < src_end_int )
					*(dest_int++) = *(src_int++);

				dest = (char*)dest_int;
				src = (char*)src_int;
			}
		}

		// finish up remaining with an old fashioned byte-by-byte copy
		while( src < ((char*)(src_end)) )
			*(dest++) = *(src++);
	}
	// else make backward pass
	// no optimization here since this is a minor and less frequent case
	else
	{
		// intialize our iterators for backward copy
		// start from the last byte that needs to be copied
		char* dest = dest_start + size - 1;
		const char* src = src_last;

		while( src >= ((char*)(src_start)) )
			*(dest--) = *(src--);
	}
}

void memory_set(void* dest_start, char byte_value, unsigned int size)
{
	// if the copy size is zero, skip the copy operation
	if(size == 0)
		return;

	// the end of the dest buffer and last dest byte address that needs to be copied
	const void* dest_end = dest_start + size;
	//const void* dest_last = dest_end - 1;

	// intialize our iterators for the copy operation
	char* dest = dest_start;

	// check if int copy is possible, and requires atleast 3 iterations (atleast 2 int copy iterations)
	if(size >= 3 * int_size)
	{
		// perform a byte-by-byte copy until the address is int aligned
		while( ((long int)dest) & ~int_alignment_bit_mask )
			*(dest++) = byte_value;

		// perform an int-by-int transfer in this scope
		// dest_int must not leave this scope
		// it must equal the dest, right before and right after this scope
		{
			int* dest_int = (int*)dest;

			// generate the int_value that we could copy to the int locations int-by-int
			int int_value = 0;
			for(unsigned int i = 0; i < int_bits_size; i += 8)
				int_value |= ( (((int)byte_value) & 0xff) << i );

			// additonal bytes that you might have to copy after completing the int copy
			unsigned long int additional_bytes = ((unsigned long int)dest_end) & ~int_alignment_bit_mask;

			// if you hit this address, stop copying ints
			const int* dest_end_int = dest_end - additional_bytes;

			while( dest_int < dest_end_int )
				*(dest_int++) = int_value;

			dest = (char*)dest_int;
		}
	}

	// finish up remaining with an old fashioned byte-by-byte copy
	while( dest < ((char*)(dest_end)) )
		*(dest++) = byte_value;
}