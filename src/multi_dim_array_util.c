#include<cutlery/multi_dim_array_util.h>

cy_uint get_element_count_in_multi_dim_array(const cy_uint* dims, cy_uint dims_count)
{
	cy_uint element_count = 0;
	for(cy_uint dim = 0; dim < dims_count; dim++)
		element_count *= dims[dim];
	return element_count;
}

cy_uint get_accessor_from_indices(const cy_uint* indices, const cy_uint* dims, cy_uint dims_count)
{
	cy_uint accessor = 0;
	cy_uint elements_under_ith_dimension = 1;
	// elements_under_the_ith_dimension is the number of elements that you jump when ith dimension's index (0 <= indices[i] < dims[i]) changes by 1
	// 0 -> 1
	// 1 -> 1 * dims[0]
	// 2 -> 1 * dims[0] * dims[1]
	// 3 -> ... and so on,
	for(cy_uint dim = 0; dim < dims_count; dim++)
	{
		accessor += (elements_under_ith_dimension * indices[dim]);
		elements_under_ith_dimension *= dims[dim];
	}
	return accessor;
}

cy_uint get_max_index_in_multi_dim_array(cy_uint dim_no, const cy_uint* dims, cy_uint dims_count)
{
	return dims[dim_no] - 1;
}

void get_indices_from_accessor(cy_uint* indices, cy_uint accessor, const cy_uint* dims, cy_uint dims_count)
{
	cy_uint elements_under_ith_dimension = 1;
	// elements_under_the_ith_dimension is the number of elements that you jump when ith index (0 <= indices[i] < dims[i]) changes by 1
	// 0 -> 1
	// 1 -> 1 * dims[0]
	// 2 -> 1 * dims[0] * dims[1]
	// 3 -> ... and so on,
	for(cy_uint dim = 0; dim < dims_count; dim++)
	{
		indices[dim] = (accessor / elements_under_ith_dimension) % dims[dim];
		elements_under_ith_dimension *= dims[dim];
	}
}
