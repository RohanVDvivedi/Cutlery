#ifndef INDEX_ACCESSED_SEARCH_SORT_ARRAY_H
#define INDEX_ACCESSED_SEARCH_SORT_ARRAY_H

#include<cutlery_stds.h>
#include<memory_allocator_interface.h>

//---------------------------------------------------------------------------
//                         INDEX_ACCESSED_INTERFACE
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


//-----------------------------------------------------------------------------------------------------------
//                         CHECK IF SORTED FUNCTIONS ON INDEX_ACCESSED_INTERFACE
//-----------------------------------------------------------------------------------------------------------

// below function returns true, only if the given indexed interface is sorted
// it will return 0 (failure), if either it is unsorted, OR if the start_index and last_index are invalid
int is_sorted_iai(index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, int (*compare)(const void* data1, const void* data2));








//-----------------------------------------------------------------------------------------------------------
//                         SORT FUNCTIONS ON INDEX_ACCESSED_INTERFACE
//-----------------------------------------------------------------------------------------------------------

// All functions sort elements in array from start_index to last_index, both inclusive,
// the sorting is based on the compare function provided


// sorting algorithm used => merge sort algorithm (iterative approach)
// NOTE:: only use this function with array and arraylist - like containers that store pointers to the elements
int merge_sort_iai(index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, int (*compare)(const void* data1, const void* data2), memory_allocator mem_allocator);

// sorting algorithm used => heap sort algorithm
// NOTE:: only use this function with array and arraylist - like containers that store pointers to the elements
int heap_sort_iai(index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, int (*compare)(const void* data1, const void* data2), memory_allocator mem_allocator);

// sorting algorithm used => quick sort algorithm
int quick_sort_iai(index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, int (*compare)(const void* data1, const void* data2));

// sorting algorithm used => radix sort algorithm
// all the elements will be ordered in increasing order of their value for get_sort_attribute()
// NOTE:: only use this function with array and arraylist - like containers that store pointers to the elements
int radix_sort_iai(index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, unsigned long long int (*get_sort_attribute)(const void* data), memory_allocator mem_allocator);

// sorting algorithm used => bubble sort algorithm
int bubble_sort_iai(index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, int (*compare)(const void* data1, const void* data2));

// sorting algorithm used => insertion sort algorithm
int insertion_sort_iai(index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, int (*compare)(const void* data1, const void* data2));





//-----------------------------------------------------------------------------------------------------------
//                          SEARCH FUNCTIONS ON INDEX_ACCESSED_INTERFACE
//-----------------------------------------------------------------------------------------------------------

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


//-----------------------------------------------------------------------------------------------------------
//                          INSERTION FUNCTIONS ON SORTED INDEX_ACCESSED_INTERFACE
//-----------------------------------------------------------------------------------------------------------

// performs binary search in sorted index accessed datastructure, to find an index at which the given element data can be inserted
// it usually returns the index that is one more than the index of the largest element that is also lesser than or equal to the data (given as parameter)
cy_uint find_insertion_index_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, int (*compare)(const void* data1, const void* data2));

#endif