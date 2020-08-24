#ifndef SEARCH_SORT_ARRAY_H
#define SEARCH_SORT_ARRAY_H

// NOTE NOTe NOTE
// the sorting and searching is based on the compare function provided, 
// which must be designed such that, int (*compare)(const void* data_p1, const void* data_p2)
// compare > 0, if data_p1 > data_p2 else compare < 0
// where both data_p1 and data_p2 are elements of the array

typedef struct array array;

// SORT FUNCTIONS ON ARRAY

// sorts elements in array from start_index to end_index, both inclusive,
// the sorting is based on the compare function provided
void sort_array(array* array_p, unsigned int start_index, unsigned int end_index, int (*compare)(const void* data_p1, const void* data_p2));

// SEARCH FUNCTIONS ON ARRAY
// These functions return an index to the array element, that compares equals to the data provided
// The data provided may or may not be pointing to the element, already existing in array

// COMPARE FUNCTION FOR SORT ARRAY
// The compare function for sort_array, is little bit different from the one to be used for search array
// int (*compare)(const void* arr_data, const void* data_p)
// here the first parameter arr_data, will always be an element that already exists in array
// the second element data_p will be the pointer that you provide for comparison
// all other rules for comparison function remain the same, as stated above

// The above information is not so important, since the same compare function that worked for you in sort_array,
// will most likely work for you for search functions aswell 
// but if you have the above info, you may be able to use it wisely to provide only comparison key for data
// and not need to construct whole object on your stack just for the sake of comparison


// performs linear search in array from start_index to end_index, both inclusive,
// the sorting is based on the compare function provided
// it returns the first element that when compared to data_p equals to 0
// if no such element is found, then the function returns the total_size of array (which must not be indexed, as our logic suggests)
unsigned int linear_search_in_array(const array* array_p, unsigned int start_index, unsigned int end_index, const void* data_p, int (*compare)(const void* arr_data, const void* data_p));


// NOTE NOTE NOTE
// the binary_search_in_array function can be called on the array, on if
// your data in array is sorted in ascending order in accordance to the compare function provided
// in indices from start_index to end index inclusive

// performs binary search in array from start_index to end_index, both inclusive,
// the sorting is based on the compare function provided
// it returns the element that when compared to data_p equals to 0
// if no such element is found, we return the index which holds element, which is immediately greater or lesser than the data_p provided
// if the indexes provided are inappropriate to begin with, the function returns the total_size of array (which must not be indexed, as our logic suggests)
unsigned int binary_search_in_array(const array* array_p, unsigned int start_index, unsigned int end_index, void* data_p, int (*compare)(const void* arr_data, const void* data_p));


#endif