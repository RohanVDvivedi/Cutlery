#include<dstring_index_accessed_interface.h>

index_accessed_interface get_index_accessed_interface_for_dstring(const dstring* str_p);

#include<dstring_char.h>

int compare_chars_ascending_at(const char* c1, const char* c2)
{
	return compare_chars_ascending(*c1, *c2);
}

int compare_chars_descending_at(const char* c1, const char* c2)
{
	return compare_chars_descending(*c1, *c2);
}