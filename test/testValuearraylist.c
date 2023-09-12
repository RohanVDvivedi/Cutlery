#include<value_arraylist.h>

declarations_value_arraylist(int_list, int)
#define EXPANSION_FACTOR 1.5
definitions_value_arraylist(int_list, int)

void sprint_int(dstring* append_str, const void* data_p, unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs);
	snprintf_dstring(append_str, "%d\n", *((const int*)data_p));
}

void print_int_list(const int_list* il_p)
{
	dstring append_str;
	init_empty_dstring(&append_str, 0);
	sprint_int_list(&append_str, il_p, sprint_int, 0);
	printf_dstring(&append_str);
	printf("\n");
}

int main()
{
	int_list il;
	int_list* il_p = &il;
	initialize_int_list(il_p, 10);

	int set_val = 0;

	set_val = 5;
	push_back_to_int_list(il_p, &set_val);

	set_val = 4;
	push_front_to_int_list(il_p, &set_val);

	set_val = 3;
	push_front_to_int_list(il_p, &set_val);

	set_val = 6;
	push_back_to_int_list(il_p, &set_val);

	set_val = 7;
	push_back_to_int_list(il_p, &set_val);

	print_int_list(il_p);

	deinitialize_int_list(il_p);
	return 0;
}