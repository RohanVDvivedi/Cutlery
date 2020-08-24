#include<array.h>

void sort_array(array* array_p, unsigned int start_index, unsigned int end_index, int (*compare)(const void* data_p1, const void* data_p2));

unsigned int linear_search_in_array(const array* array_p, unsigned int start_index, unsigned int end_index, const void* data_p, int (*compare)(const void* arr_data, const void* data_p));

unsigned int find_first_in_array(const array* array_p, void* data_p, int (*compare)(const void* data_p1, const void* data_p2))
{
	for(unsigned int i = 0; i < array_p->total_size; i++)
	{
		if(compare(get_element(array_p, i), data_p) == 0)
			return i;
	}
	return array_p->total_size;
}

unsigned int binary_search_in_array(const array* array_p, unsigned int start_index, unsigned int end_index, void* data_p, int (*compare)(const void* arr_data, const void* data_p));