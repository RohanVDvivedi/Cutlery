#include<cutlery_stds.h>

void memory_move(void* dest_start, const void* src_start, unsigned int size)
{
	// if they are the same memory locations, skip the copy operation
	if(src_start == dest_start || size == 0)
		return;

	// the end of the src buffer and last src byte address that needs to be copied
	const void* src_end = src_start + size;
	const void* src_last = src_end - 1;

	// make forward pass
	// if there is no overlap or if the overlap doesnt corrupt the copy operation
	if(dest_start < src_start  || src_last < dest_start)
	{
		char* dest = dest_start;
		const char* src = src_start;

		static const unsigned int int_size = sizeof(int); // we assume that int is always 2^n bytes in size, i.e. 1,2,4,8 etc
		static const unsigned int int_alignment_bit_mask = -((int)int_size);	// 0b 111...111 000...000 where 0s represent alignment

		// check if int copy is possible, and requires atleast 3 iterations (atleast 2 int copy iterations)
		if( (size >= 3 * int_size) &&
			(
				(((long int)src_start) & ~int_alignment_bit_mask) == (((long int)dest_start) & ~int_alignment_bit_mask)
			)
		)
		{
			// perform a byte by byte copy until the address is int aligned
			while( ((long int)src) & ~int_alignment_bit_mask )
				*(dest++) = *(src++);

			// perform an int transfer in this scope
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

		// finish up remaining with an old fashioned byte copy
		while(src < ((char*)(src_end)))
			*(dest++) = *(src++);
	}
	// else make backward pass
	// no optimization here since this is a minor and less frequent case
	else
	{
		// start from the last byte that needs to be copied
		char* dest = dest_start + size - 1;
		const char* src = src_last;

		while(src >= ((char*)(src_start)))
			*(dest--) = *(src--);
	}
}

#include<string.h>

void memory_set(void* dest_start, char byte_value, unsigned int size)
{
	// ##TODO
	// replace with custom implementation
	memset(dest_start, byte_value, size);
}