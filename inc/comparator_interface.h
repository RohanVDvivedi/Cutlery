#ifndef COMPARATOR_INTERFACE_H
#define COMPARATOR_INTERFACE_H

/* 
** A comparator interface is an interface allowing you to compare 2 data (pointed to by data1 and data2 pointers)
** It allows you to compare with OR without a context
** The compare function must returns 0 if they are same, >0 if data1 is greater than data2 else it must return <0 value
*/

typedef struct comparator_interface comparator_interface;
struct comparator_interface
{
	// context for the comparator
	const void* context;

	// the compare functions of the comparator
	// The compare function must returns 0 if they are same, >0 if data1 is greater than data2 else it must return <0 value
	union
	{
		// compare function to use when context == NULL
		int (*compare1)(const void* data1, const void* data2);

		// compare function to use when context != NULL
		int (*compare2)(const void* context, const void* data1, const void* data2);
	};
};

// you must use only this function, with the comparator, to avail the complete functionality of the comparator
// it returns the result of the comparison
int compare_with_comparator(const comparator_interface* comparator, const void* data1, const void* data2);

// You may use the macros below to initialize your custom comparator_interface
#define simple_comparator(compare_function_p)               ((const comparator_interface){.context = NULL,        .compare1 = (compare_function_p)})
#define contexted_comparator(context_p, compare_function_p) ((const comparator_interface){.context = (context_p), .compare2 = (compare_function_p)})

#endif