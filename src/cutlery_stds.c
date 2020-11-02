#include<cutlery_stds.h>

void memory_move(void* dest_start, const void* src_start, unsigned int size)
{
	// if they are the same memory locations, skip the copy operation
	if(src == dest)
		return;

	// calcualte the last src and dest addresses
	void* dest_end = dest_start + size - 1;
	const void* src_end = src_start + size - 1;

	if(dest_start < src_start) // make forward pass
	{
		void* dest = dest_start;
		const void* src = src_start;

		while(src <= src_end)
			*(dest++) = *(src++);
	}
	else // else make backward pass
	{
		void* dest = dest_end;
		const void* src = src_end;

		while(src >= src_start)
			*(dest--) = *(src--);
	}
}