#ifndef ARRAYLIST_SORT_H
#define ARRAYLIST_SORT_H

#include<arraylist.h>
#include<comparator_interface.h>

// inplace heapsort for arraylist
// sorting algorithm used => heap sort algorithm
int heap_sort_arraylist(arraylist* al, cy_uint start_index, cy_uint last_index, const comparator_interface* comparator);

#endif