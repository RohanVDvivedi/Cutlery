#ifndef INDEX_ACCESSED_SEARCH_SORT_ARRAY_H
#define INDEX_ACCESSED_SEARCH_SORT_ARRAY_H

#include<cutlery/cutlery_stds.h>
#include<cutlery/memory_allocator_interface.h>

//---------------------------------------------------------------------------
//                         INDEX_ACCESSED_INTERFACE
//---------------------------------------------------------------------------

#include<cutlery/comparator_interface.h>
#include<cutlery/index_accessed_interface.h>

// NOTE NOTE NOTE
// the sorting and searching is based on the comparator provided, 
// which must be designed such that, comparator returns following
// if data1 > data2
//    then return a value > 0
// else if data1 < data2
//    then return a value < 0
// else return 0 (i.e. data1 == data2)
// where both data1 and data2 are elements of the array


//-----------------------------------------------------------------------------------------------------------
//                         CHECK IF SORTED FUNCTIONS ON INDEX_ACCESSED_INTERFACE
//-----------------------------------------------------------------------------------------------------------

// below function returns true, only if the given indexed interface is sorted
// it will return 0 (failure), if either it is unsorted, OR if the start_index and last_index are invalid
int is_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const comparator_interface* comparator);








//-----------------------------------------------------------------------------------------------------------
//                         SORT FUNCTIONS ON INDEX_ACCESSED_INTERFACE
//-----------------------------------------------------------------------------------------------------------

// All functions sort elements in array from start_index to last_index, both inclusive,
// the sorting is based on the compare function provided


// sorting algorithm used => merge sort algorithm (iterative approach)
// NOTE:: only use this function with array and arraylist - like containers that store pointers to the elements
// uses set_element -> the set_element is expected to succeed with indices valid and within bounds
int merge_sort_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const comparator_interface* comparator, memory_allocator mem_allocator);

// sorting algorithm used => heap sort algorithm, degree specifie the degree of the heap being used, for most cases 2 is fine
// uses swap_elements -> fails with 0, if the swap_elements returns 0
int heap_sort_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const comparator_interface* comparator, cy_uint degree);

// sorting algorithm used => quick sort algorithm
// uses swap_elements -> fails with 0, if the swap_elements returns 0
int quick_sort_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const comparator_interface* comparator);

// sorting algorithm used => radix sort algorithm
// all the elements will be ordered in increasing order of their value for get_sort_attribute()
// NOTE:: only use this function with array and arraylist - like containers that store pointers to the elements
// uses set_element -> the set_element is expected to succeed with indices valid and within bounds
int radix_sort_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, unsigned long long int (*get_sort_attribute)(const void* data), memory_allocator mem_allocator);

// sorting algorithm used => bubble sort algorithm
// uses swap_elements -> fails with 0, if the swap_elements returns 0
int bubble_sort_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const comparator_interface* comparator);

// sorting algorithm used => insertion sort algorithm
// uses swap_elements -> fails with 0, if the swap_elements returns 0
int insertion_sort_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const comparator_interface* comparator);





//-----------------------------------------------------------------------------------------------------------
//                          SEARCH FUNCTIONS ON INDEX_ACCESSED_INTERFACE
//-----------------------------------------------------------------------------------------------------------

// These functions return an index to the element in the index_accessed datastructure,
// that compares `comparable` (This means differently for different functions) to the data provided
// The "data_p" provided as parameter for equality checking may or may not be pointing to the element, already existing in the data structure
// The check is only performed for all indices from start_index to last_index both inclusive
// the search_occurence parameter suggests whether we are looking for first or the last occurence (parameter occurence_type) that compares equal
// This functions return INVALID_INDEX, if the such a `comparable` element is not found, or the indexes provided are invalid

// Be sure that in all the function following this comment that
// your "data_p", always becomes the second parameter of the comparator function that you provide
// This allows you greater flexibility while comparing different types


// performs linear search in the index accessed datastructure
cy_uint linear_search_in_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, const comparator_interface* comparator, search_occurence occurence_type);

// performs binary search in the sorted index accessed datastructure
cy_uint binary_search_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, const comparator_interface* comparator, search_occurence occurence_type);

// performs binary search in the sorted index accessed datastructure
cy_uint find_preceding_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, const comparator_interface* comparator);
cy_uint find_preceding_or_equals_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, const comparator_interface* comparator);
cy_uint find_succeeding_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, const comparator_interface* comparator);
cy_uint find_succeeding_or_equals_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, const comparator_interface* comparator);


//-----------------------------------------------------------------------------------------------------------
//                          INSERTION FUNCTIONS ON SORTED INDEX_ACCESSED_INTERFACE
//-----------------------------------------------------------------------------------------------------------

// performs binary search in sorted index accessed datastructure, to find an index at which the given element data can be inserted
// it usually returns the index that is one more than the index of the largest element that is also lesser than or equal to the data (given as parameter)
cy_uint find_insertion_index_in_sorted_iai(const index_accessed_interface* iai_p, cy_uint start_index, cy_uint last_index, const void* data_p, const comparator_interface* comparator);

#endif