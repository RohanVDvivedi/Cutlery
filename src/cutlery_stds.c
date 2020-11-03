#include<cutlery_stds.h>

void memory_move(void* dest_start, const void* src_start, unsigned int size)
{
	// if they are the same memory locations, skip the copy operation
	if(src_start == dest_start || size == 0)
		return;

	// the last byte address that needs to be copied
	const void* src_end = src_start + size - 1;

	// make forward pass
	// if there is no overlap or if the overlap doesnt corrupt the copy operation
	if(dest_start < src_start || src_end < dest_start)
	{
		unsigned int int_size = sizeof(int);
		unsigned int int_alignment_bit_mask = -((int)int_size);

		// check if int copy is possible
		if( (size >= 16) && !(size & ~int_alignment_bit_mask)
			&& !(((unsigned long int)src_start) & ~int_alignment_bit_mask)
			&& !(((unsigned long int)dest_start) & ~int_alignment_bit_mask) )
		{
			int* dest = dest_start;
			const int* src = src_start;

			while(src < ((int*)(src_start + size)))
				*(dest++) = *(src++);
		}
		// else just do an old fashioned byte copy
		else
		{
			char* dest = dest_start;
			const char* src = src_start;

			while(src < ((char*)(src_start + size)))
				*(dest++) = *(src++);
		}
	}
	// else make backward pass
	else
	{
		char* dest = dest_start + size - 1;
		const char* src = src_start + size - 1;

		while(src >= ((char*)(src_start)))
			*(dest--) = *(src--);
	}
}