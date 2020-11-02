#include<cutlery_stds.h>

void memory_move(void* dest_start, const void* src_start, unsigned int size)
{
	// if they are the same memory locations, skip the copy operation
	if(src_start == dest_start)
		return;

	if(dest_start < src_start) // make forward pass
	{
		char* dest = dest_start;
		const char* src = src_start;

		while(src <= ((char*)(src_start + size - 1)))
			*(dest++) = *(src++);
	}
	else // else make backward pass
	{
		char* dest = dest_start + size - 1;
		const char* src = src_start + size - 1;

		while(src >= ((char*)(src_start)))
			*(dest--) = *(src--);
	}
}