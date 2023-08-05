#ifndef INDEXED_SEARCH_SORT_ARRAY_H
#define INDEXED_SEARCH_SORT_ARRAY_H

#include<cutlery_stds.h>
#include<memory_allocator_interface.h>

//---------------------------------------------------------------------------
//                         INTERFACE
//---------------------------------------------------------------------------

#include<index_accessed_interface.h>

// NOTE NOTE NOTE
// the sorting and searching is based on the compare function provided, 
// which must be designed such that, int (*compare)(const void* data1, const void* data2)
// if data1 > data2
//    then compare > 0
// else if data1 < data2
//    then compare < 0
// else compare = 0 (i.e. data1 == data2)
// where both data1 and data2 are elements of the array




//---------------------------------------------------------------------------
//                         SORT FUNCTIONS ON ARRAY
//---------------------------------------------------------------------------

// All functions sort elements in array from start_index to last_index, both inclusive,
// the sorting is based on the compare function provided


// sorting algorithm used => merge sort algorithm (iterative approach)
int merge_sort_iai(index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, int (*compare)(const void* data1, const void* data2), memory_allocator mem_allocator);

// sorting algorithm used => heap sort algorithm
//int heap_sort_array(array* array_p, cy_uint start_index, cy_uint last_index, int (*compare)(const void* data1, const void* data2));

// sorting algorithm used => quick sort algorithm
int quick_sort_array(index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, int (*compare)(const void* data1, const void* data2));

// sorting algorithm used => radix sort algorithm
// all the elements will be ordered in increasing order of their value for get_sort_attribute()
int radix_sort_iai(index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, unsigned long long int (*get_sort_attribute)(const void* data), memory_allocator mem_allocator);






//------------------------------------------------------------------------------
//                          SEARCH FUNCTIONS ON ARRAY
//------------------------------------------------------------------------------

// These functions return an index to the element in the index_accessed datastructure,
// that compares `comparable` (This means differently for different functions) to the data provided
// The data provided as parameter for equality checking may or may not be pointing to the element, already existing in the data structure
// The check is only performed for all indices from start_index to last_index both inclusive
// the search_occurence parameter suggests whether we are looking for first or the last occurence (parameter occurence_type) that compares equal
// This functions return INVALID_INDEX, if the such a `comparable` element is not found, or the indexes provided are invalid


// performs linear search in the index accessed datastructure
cy_uint linear_search_in_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, int (*compare)(const void* data1, const void* data2), search_occurence occurence_type);

// performs binary search in the sorted index accessed datastructure
cy_uint binary_search_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, int (*compare)(const void* data1, const void* data2), search_occurence occurence_type);

// performs binary search in the sorted index accessed datastructure
cy_uint find_preceding_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, int (*compare)(const void* data1, const void* data2));
cy_uint find_preceding_or_equals_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, int (*compare)(const void* data1, const void* data2));
cy_uint find_succeeding_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, int (*compare)(const void* data1, const void* data2));
cy_uint find_succeeding_or_equals_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, int (*compare)(const void* data1, const void* data2));


//------------------------------------------------------------------------------
//                          INSERTION FUNCTIONS ON SORTED ARRAY
//------------------------------------------------------------------------------

// performs binary search in sorted index accessed datastructure, to find an index at which the given element data can be inserted
// it usually returns the index that is one more than the index of the largest element that is also lesser than or equal to the data (given as parameter)
cy_uint find_insertion_index_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, int (*compare)(const void* data1, const void* data2));

#endif