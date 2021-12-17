#include<multi_dim_array_util.h>

unsigned int get_element_count_in_multi_dim_array(const unsigned int* dims, unsigned int dims_count)
{
	unsigned int element_count = 0;
	for(unsigned int dim = 0; dim < dims_count; dim++)
		element_count *= dims[dim];
	return element_count;
}

unsigned int get_accessor_from_indices(const unsigned int* indices, const unsigned int* dims, unsigned int dims_count)
{
	unsigned int accessor = 0;
	unsigned int elements_under_ith_dimension = 1;
	for(unsigned int dim = 0; dim < dims_count; dim++)
	{
		accessor += (elements_under_ith_dimension * indices[dim]);
		elements_under_ith_dimension *= dims[dim];
	}
	return accessor;
}

unsigned int get_max_index_in_multi_dim_array(unsigned int dim_no, const unsigned int* dims, unsigned int dims_count)
{
	return dims[dim_no] - 1;
}

void get_indices_from_accessor(unsigned int* indices, unsigned int accessor, const unsigned int* dims, unsigned int dims_count)
{
	unsigned int elements_under_ith_dimension = 1;
	for(unsigned int dim = 0; dim < dims_count; dim++)
	{
		indices[dim] = (accessor / elements_under_ith_dimension) % dims[dim];
		elements_under_ith_dimension *= dims[dim];
	}
}