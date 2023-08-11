#ifndef ARRAYLIST_SORT_H
#define ARRAYLIST_SORT_H

#include<arraylist.h>

// inplace heapsort for arraylist
// sorting algorithm used => heap sort algorithm
int heap_sort_arraylist(arraylist* al, cy_uint start_index, cy_uint last_index, int (*compare)(const void* data1, const void* data2));

#endif