#include<array.h>

#include<cutlery_stds.h>
#include<memory_allocator_interface.h>

#include<stdlib.h>

void sort_array(array* array_p, unsigned int start_index, unsigned int end_index, int (*compare)(const void* data_p1, const void* data_p2))
{
	if(start_index > end_index || end_index >= array_p->total_size || end_index - start_index == 1)
		return;

	unsigned int total_elements = end_index - start_index + 1;

	void** src  = (void**) array_p->data_p_p + start_index;
	void** dest = malloc(sizeof(void*) * total_elements);

	// sort element in src and write them to dest
	// sort algo

	unsigned int sort_chunk_size = 1;
	while(sort_chunk_size <= total_elements)
	{
		unsigned int dest_index = 0;
		while(dest_index < total_elements)
		{
			unsigned int a_start = dest_index;
			unsigned int a_end = a_start + sort_chunk_size - 1;
			unsigned int b_start = a_end + 1;
			unsigned int b_end = b_start + sort_chunk_size - 1;

			if(b_start > total_elements - 1)
			{
				if(a_end > total_elements - 1)
					a_end = total_elements - 1;

				memory_move(dest + dest_index, src + a_start, (a_end - a_start + 1) * sizeof(void*));
				break;
			}
			else
			{
				if(b_end > total_elements - 1)
					b_end = total_elements - 1;

				while(dest_index <= b_end)
				{
					if((b_start > b_end) || (a_start <= a_end && compare(src[a_start], src[b_start]) < 0))
						dest[dest_index++] = src[a_start++];
					else
						dest[dest_index++] = src[b_start++];
				}
			}
		}

		void** temp = src;
		src = dest;
		dest = temp;

		sort_chunk_size = sort_chunk_size << 1;
	}

	// sort algo

	if( ((void**)(array_p->data_p_p + start_index)) == src)
		free(dest);
	else
	{
		memory_move(array_p->data_p_p + start_index, src, total_elements * sizeof(void*));
		free(src);
	}
}

unsigned int linear_search_in_array(const array* array_p, unsigned int start_index, unsigned int end_index, const void* data_p, int (*compare)(const void* arr_data, const void* data_p))
{
	if(start_index > end_index || end_index >= array_p->total_size)
		return array_p->total_size;

	for(unsigned int i = start_index; i <= end_index; i++)
	{
		if(compare(get_element(array_p, i), data_p) == 0)
			return i;
	}

	// we return answer or return an element out of bounds
	return array_p->total_size;
}

unsigned int binary_search_in_array(const array* array_p, unsigned int start_index, unsigned int end_index, void* data_p, int (*compare)(const void* arr_data, const void* data_p))
{
	if(start_index > end_index || end_index >= array_p->total_size)
		return array_p->total_size;

	if(compare(get_element(array_p, start_index), data_p) >= 0)
		return start_index;

	if(compare(get_element(array_p, end_index), data_p) <= 0)
		return end_index;

	unsigned int mid = (start_index + end_index) / 2;
	while(start_index < end_index)
	{
		int cmp = compare(get_element(array_p, mid), data_p);
		if(cmp > 0)
			end_index = mid;
		else if(cmp < 0)
			start_index = mid;
		else
			return mid;

		mid = (start_index + end_index) / 2;

		if(mid == start_index || mid == end_index)
			break;
	}

	// we return answer else return some other element closer to it
	return mid;
}