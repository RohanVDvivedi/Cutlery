#include<array.h>

#include<string.h>

void sort_array(array* array_p, unsigned int start_index, unsigned int end_index, int (*compare)(const void* data_p1, const void* data_p2))
{
	if(start_index > end_index || end_index >= array_p->total_size || end_index - start_index == 1)
		return;

	unsigned int total_elements = end_index - start_index + 1;

	array test_arr_1;
	initialize_array(&test_arr_1, total_elements);
	array test_arr_2;
	initialize_array(&test_arr_2, total_elements);

	array* src = &test_arr_1;
	array* dest = &test_arr_2;

	memcpy(src->data_p_p, array_p->data_p_p + start_index, total_elements * sizeof(void*));

	// sort element in src and write them to dest

	memcpy(array_p->data_p_p + start_index, dest->data_p_p, total_elements * sizeof(void*));

	deinitialize_array(&test_arr_1);
	deinitialize_array(&test_arr_2);
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

	while(start_index <= end_index)
	{
		unsigned int mid = (start_index + end_index) / 2;

		if(compare(get_element(array_p, mid), data_p) > 0)
			end_index = mid;
		else if(compare(get_element(array_p, mid), data_p) < 0)
			start_index = mid;
		else
			return mid;
	}

	// we return answer or return an element out of bounds
	return array_p->total_size;
}