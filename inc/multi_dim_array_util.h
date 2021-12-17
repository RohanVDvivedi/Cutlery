#ifndef MULTI_DIM_ARRAY_UTIL_H
#define MULTI_DIM_ARRAY_UTIL_H

/*
**
**	This file provides utility fundtions for accessing multi-dimensional array
**  that is laid out as a single flat 1-dimensional array
**
**	let array arr be a multi-dimentional integer array
**
**  so you may declare it as
**  int arr[arr_size];
**  
**  let its dimensions be dim = {dims[0], dims[1] ... , dims[dims_count-1]}
**  so here arr_size = dims[0] * dims[1] * ... * dims[dims_count-1]
**  
**  let us say we want to access the int 
**  at indices = {indices[0], indices[1] ... , indices[dims_count-1]}
**  since arr is inherently a 1-dimensional array we can not pass in all dims_count number of indices to it
**
**  so we define a variable called as accessor
**  such that
**      arr[accessor] == multi-dimensional arr[indices[dims_count-1]] ... [indices[1]] [indices[0]];
**
**  this source and header file provides you utility functions
**  to convert from accessor to indices and again from indices to accessor
**
*/

// total number of elements in multi dimension array
unsigned int get_element_count_in_multi_dim_array(const unsigned int* dims, unsigned int dims_count);

// accessor to array[index[dims_count-1]]...[index[1]][index[0]]
// is given by array[accessor]
// please make sure that all the indices are in range
// i.e. for any i form 0 to dims_count - 1 both inclusive ==>>> 0 <= index[i] < dims[i]
unsigned int get_accessor_from_indices(const unsigned int* indices, const unsigned int* dims, unsigned int dims_count);

// returns maximum index that is possible for a dimension at dim_no for given dimensions list
unsigned int get_max_index_in_multi_dim_array(unsigned int dim_no, const unsigned int* dims, unsigned int dims_count);

// accessor to array[index[dims_count-1]]...[index[1]][index[0]]
// is given by array[accessor]
// please make sure that the accessor in in range
// i.e. 0 <= accessor < ( dims[dims_count - 1] * ... * dims[1] * dims[0] )
void get_indices_from_accessor(unsigned int* indices, unsigned int accessor, const unsigned int* dims, unsigned int dims_count);

#endif