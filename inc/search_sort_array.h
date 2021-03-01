#ifndef SEARCH_SORT_ARRAY_H
#define SEARCH_SORT_ARRAY_H

// NOTE NOTE NOTE
// the sorting and searching is based on the compare function provided, 
// which must be designed such that, int (*compare)(const void* data1, const void* data2)
// if data1 > data2
//    then compare > 0
// else if data1 < data2
//    then compare < 0
// else compare = 0 (i.e. data1 == data2)
// where both data1 and data2 are elements of the array

typedef struct array array;

//---------------------------------------------------------------------------
//                         SORT FUNCTIONS ON ARRAY
//---------------------------------------------------------------------------

// All functions sort elements in array from start_index to end_index, both inclusive,
// the sorting is based on the compare function provided


// sorting algorithm used => merge sort algorithm (iterative approach)
void sort_array(array* array_p, unsigned int start_index, unsigned int end_index, int (*compare)(const void* data1, const void* data2));






//------------------------------------------------------------------------------
//                          SEARCH FUNCTIONS ON ARRAY
//------------------------------------------------------------------------------

// These functions return an index to the array element, that compares equals to the data provided
// The data provided as parameter for equality checking may or may not be pointing to the element, already existing in array

// performs linear search in array from start_index to end_index, both inclusive,
// if no such element is found, then the function returns the total_size of array (which will not be indexable, INDEX_OUT_OF_BOUNDS)
unsigned int linear_search_in_array(const array* array_p, unsigned int start_index, unsigned int end_index, const void* data, int (*compare)(const void* data1, const void* data2));




// NOTE NOTE NOTE
// the binary_search_in_array function can be called on the array, only if
// your data in the array is sorted in ascending order in accordance to the compare function provided as parameter
// , (sorted) for all indices from start_index to end_index inclusive

// performs binary search in array from start_index to end_index, both inclusive,
// it returns the element that when compared to data equals to 0
// if no such element is found, we return the index which holds element, which is closest to data in the array
// if the indexes provided are inappropriate to begin with, the function returns the total_size of array (which is not indexable, INDEX_OUT_OF_BOUNDS)
unsigned int binary_search_in_array(const array* array_p, unsigned int start_index, unsigned int end_index, const void* data, int (*compare)(const void* data1, const void* data2));


#endif