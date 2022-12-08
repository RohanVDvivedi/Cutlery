#ifndef SEARCH_SORT_ARRAY_H
#define SEARCH_SORT_ARRAY_H

#include<cutlery_stds.h>

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

// All functions sort elements in array from start_index to last_index, both inclusive,
// the sorting is based on the compare function provided


// sorting algorithm used => merge sort algorithm (iterative approach)
void merge_sort_array(array* array_p, unsigned int start_index, unsigned int last_index, int (*compare)(const void* data1, const void* data2));

// sorting algorithm used => heap sort algorithm
void heap_sort_array(array* array_p, unsigned int start_index, unsigned int last_index, int (*compare)(const void* data1, const void* data2));

// sorting algorithm used => quick sort algorithm
void quick_sort_array(array* array_p, unsigned int start_index, unsigned int last_index, int (*compare)(const void* data1, const void* data2));

// sorting algorithm used => radix sort algorithm
// all the elements will be ordered in increasing order of their value for get_sort_attribute()
void radix_sort_array(array* array_p, unsigned int start_index, unsigned int last_index, unsigned int (*get_sort_attribute)(const void* data));






//------------------------------------------------------------------------------
//                          SEARCH FUNCTIONS ON ARRAY
//------------------------------------------------------------------------------

// These functions return an index to the array element, that compares equals to the data provided
// The data provided as parameter for equality checking may or may not be pointing to the element, already existing in array
// the search_occurence parameter suggests whether we are looking for first or the last occurence (parameter occurence_type) that compares equal

// performs linear search in array from start_index to last_index, both inclusive,
// returns INVALID_INDEX, if the element is not found, or the indexes provided are invalid
unsigned int linear_search_in_array(const array* array_p, unsigned int start_index, unsigned int last_index, const void* data_p, int (*compare)(const void* data1, const void* data2), search_occurence occurence_type);




// NOTE :::
// the binary_search_in_sorted_array function can be called on the array, only if
// your data in the array is sorted in ascending order in accordance to the compare function provided as parameter
// , (sorted) for all indices from start_index to last_index, both inclusive

// performs binary search in array from start_index to last_index, both inclusive,
// returns INVALID_INDEX, if the element is not found, or the indexes provided are invalid
unsigned int binary_search_in_sorted_array(const array* array_p, unsigned int start_index, unsigned int last_index, const void* data_p, int (*compare)(const void* data1, const void* data2), search_occurence occurence_type);

// NOTE :::
// the find_*_in_sorted_array function can be called on the array, only if
// your data in the array is sorted in ascending order in accordance to the compare function provided as parameter
// , (sorted) for all indices from start_index to last_index, both inclusive

// These functions performs binary search in array from start_index to last_index, both inclusive,
// returns INVALID_INDEX, if no result is not found, or the indexes provided are invalid
unsigned int find_preceding_in_sorted_array(const array* array_p, unsigned int start_index, unsigned int last_index, const void* data_p, int (*compare)(const void* data1, const void* data2));
unsigned int find_preceding_or_equals_in_sorted_array(const array* array_p, unsigned int start_index, unsigned int last_index, const void* data_p, int (*compare)(const void* data1, const void* data2));
unsigned int find_succeeding_in_sorted_array(const array* array_p, unsigned int start_index, unsigned int last_index, const void* data_p, int (*compare)(const void* data1, const void* data2));
unsigned int find_succeeding_or_equals_in_sorted_array(const array* array_p, unsigned int start_index, unsigned int last_index, const void* data_p, int (*compare)(const void* data1, const void* data2));


//------------------------------------------------------------------------------
//                          INSERTION FUNCTIONS ON SORTED ARRAY
//------------------------------------------------------------------------------

// NOTE :::
// the find_insertion_index_in_array function can be called on the array, only if
// your data in the array is sorted in ascending order in accordance to the compare function provided as parameter
// , (sorted) for all indices from start_index to last_index, both inclusive

// performs binary search in array from start_index to last_index, both inclusive, to find an dex at which the given element data can be inserted
// it usually returns the index that is one more than the index of the largest element that is also lesser than or equal to the data (given as parameter)
// returns INVALID_INDEX, if the element is not found, or the indexes provided are invalid
unsigned int find_insertion_index_in_sorted_array(const array* array_p, unsigned int start_index, unsigned int last_index, const void* data_p, int (*compare)(const void* data1, const void* data2));

#endif