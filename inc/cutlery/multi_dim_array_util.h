#ifndef MULTI_DIM_ARRAY_UTIL_H
#define MULTI_DIM_ARRAY_UTIL_H

#include<cutlery/cutlery_stds.h>

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
**  to convert from accessor to indices and from indices to accessor
**
** Note. :: the dimension-0 (the dimension refered to at index 0 in dims and indices array) is the lowest dimension
**       :: and dimension-(dims_count - 1) (i.e. the dimension referred to the last index in dims and indices array) is the highest/greatest/outermost dimension
**
** i.e. to compare usage for an array of dimensions (3,2,4) -> 3 being the outer most dimension and 4 being the inner most dimension, then
**
**	standard stack allocated static multi-dimensional array      |   array supported by multi_dim_array_util.h
**                                                               |
**  declaration:                                                 |   declaration:
**               int arr[3][2][4] = {};                          |                int arr[3 * 2 * 4] = {};   OR   int* arr = calloc(3 * 2 * 4, sizeof(int));
**                                                               |
**  element access: @ (2,0,3)                                    |   element access @ (2,0,3)
**               arr[2][0][3]                                    |                arr[ get_accessor_from_indices( &((cy_uint[]){3,0,2}), &((cy_uint[]){4,2,3}) , 3) ]
**
**	Notice the reverse order in supplying the dimensions and indices array ???
*/

// It is your responsibility to ensure that the product of all the dimensions does not overflow cy_uint

// total number of elements in multi dimension array
cy_uint get_element_count_in_multi_dim_array(const cy_uint* dims, cy_uint dims_count);

// accessor to array[index[dims_count-1]]...[index[1]][index[0]]
// is given by array[accessor]
// please make sure that all the indices are in range
// i.e. for any i form 0 to dims_count - 1 both inclusive ==>>> 0 <= index[i] < dims[i]
cy_uint get_accessor_from_indices(const cy_uint* indices, const cy_uint* dims, cy_uint dims_count);

// returns maximum index that is possible for a dimension at dim_no for given dimensions list
cy_uint get_max_index_in_multi_dim_array(cy_uint dim_no, const cy_uint* dims, cy_uint dims_count);

// accessor to array[index[dims_count-1]]...[index[1]][index[0]]
// is given by array[accessor]
// please make sure that the accessor in in range
// i.e. 0 <= accessor < ( dims[dims_count - 1] * ... * dims[1] * dims[0] )
void get_indices_from_accessor(cy_uint* indices, cy_uint accessor, const cy_uint* dims, cy_uint dims_count);

#endif
