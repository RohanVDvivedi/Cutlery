#include<value_arraylist.h>

declarations_value_arraylist(int_list, int)
#define EXPANSION_FACTOR 1.5
definitions_value_arraylist(int_list, int)

int main()
{
	int_list il;
	int_list* il_p = &il;
	initialize_int_list(il_p, 10);

	deinitialize_int_list(il_p);
	return 0;
}