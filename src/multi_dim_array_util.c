#include<multi_dim_array_util.h>

unsigned int get_element_count_in_multi_dim_array(const unsigned int* dims, unsigned int dims_count);

int get_accessor_from_indices(unsigned int* accessor, const unsigned int* indices, const unsigned int* dims, unsigned int dims_count);

unsigned int get_max_index_in_multi_dim_array(unsigned int dim_no, const unsigned int* dims, unsigned int dims_count);

int get_indices_from_accessor(unsigned int* indices, unsigned int accessor, const unsigned int* dims, unsigned int dims_count);