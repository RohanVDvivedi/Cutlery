#ifndef MULTI_DIM_ARRAY_UTIL_H
#define MULTI_DIM_ARRAY_UTIL_H

// total number of elements in multi dimension array
unsigned int get_element_count_in_multi_dim_array(const unsigned int* dims, unsigned int dims_count);

// accessor to array[index[dims_count-1]]...[index[1]][index[0]]
// is given by array[accessor]
// please make sure that all the indices are in range
// i.e. for any i form 0 to dims_count - 1 both inclusive ==>>> 0 <= index[i] < dims[i]
unsigned int get_accessor_from_indices(const unsigned int* indices, const unsigned int* dims, unsigned int dims_count);

unsigned int get_max_index_in_multi_dim_array(unsigned int dim_no, const unsigned int* dims, unsigned int dims_count);

// accessor to array[index[dims_count-1]]...[index[1]][index[0]]
// is given by array[accessor]
// please make sure that the accessor in in range
// i.e. 0 <= accessor < ( dims[dims_count - 1] * ... * dims[1] * dims[0] )
void get_indices_from_accessor(unsigned int* indices, unsigned int accessor, const unsigned int* dims, unsigned int dims_count);

#endif