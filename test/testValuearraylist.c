#include<value_arraylist.h>

#define MAX_int_list_CAPACITY (CY_UINT_MAX / sizeof(int))
declarations_value_arraylist(int_list, int)
definitions_value_arraylist(int_list, int)

int main()
{
	int_list il;
	int_list* il_p = &il;
	initialize_int_list(il_p, 10);

	deinitialize_int_list(il_p);
	return 0;
}