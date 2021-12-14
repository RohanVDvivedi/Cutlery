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
void merge_sort_array(array* array_p, unsigned int start_index, unsigned int end_index, int (*compare)(const void* data1, const void* data2));

// sorting algorithm used => heap sort algorithm
void heap_sort_array(array* array_p, unsigned int start_index, unsigned int end_index, int (*compare)(const void* data1, const void* data2));

// sorting algorithm used => quick sort algorithm
void quick_sort_array(array* array_p, unsigned int start_index, unsigned int end_index, int (*compare)(const void* data1, const void* data2));

// sorting algorithm used => radix sort algorithm
// all the elements will be ordered in increasing order of their value for get_sort_attribute()
void radix_sort_array(array* array_p, unsigned int start_index, unsigned int end_index, unsigned int (*get_sort_attribute)(const void* data));






//------------------------------------------------------------------------------
//                          SEARCH FUNCTIONS ON ARRAY
//------------------------------------------------------------------------------

// These functions return an index to the array element, that compares equals to the data provided
// The data provided as parameter for equality checking may or may not be pointing to the element, already existing in array
// if the element is not found an INVALID_INDEX is returned
// the search_occurence parameter suggests whether we are looking for first or the last occurence that compares equal


enum search_occurence
{
	FIRST_OCCURENCE = 0,
	LAST_OCCURENCE
};
typedef enum search_occurence search_occurence;

// performs linear search in array from start_index to end_index, both inclusive,
// returns INVALID_INDEX, if the element is not found
unsigned int linear_search_in_array(const array* array_p, unsigned int start_index, unsigned int end_index, const void* data, int (*compare)(const void* data1, const void* data2), search_occurence occurence_type);




// NOTE NOTE NOTE
// the binary_search_in_array function can be called on the array, only if
// your data in the array is sorted in ascending order in accordance to the compare function provided as parameter
// , (sorted) for all indices from start_index to end_index inclusive

// performs binary search in array from start_index to end_index, both inclusive,
// it returns the element that when compared to data equals to 0
// if no such element is found, we return the index which holds element, which is closest to data in the array
// if the indexes provided are inappropriate to begin with, the function returns the total_size of array (which is not indexable, INDEX_OUT_OF_BOUNDS)
unsigned int binary_search_in_array(const array* array_p, unsigned int start_index, unsigned int end_index, const void* data, int (*compare)(const void* data1, const void* data2), search_occurence occurence_type);


#endif