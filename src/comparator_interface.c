#include<comparator_interface.h>

#include<cutlery_stds.h>

int compare_with_comparator(const comparator_interface* comparator, const void* data1, const void* data2)
{
	if(comparator->context == NULL)
		return comparator->compare1(data1, data2);
	else
		return comparator->compare2(comparator->context, data1, data2);
}