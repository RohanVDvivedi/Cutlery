#include<cutlery/dstring_index_accessed_interface.h>

static const char* get_char_at_from_dstring(const dstring* str_p, cy_uint index)
{
	// make sure that the index is valid
	if(index >= get_char_count_dstring(str_p))
		return NULL;

	// return pointer to the char
	return get_byte_array_dstring(str_p) + index;
}

static int set_char_at_in_dstring(dstring* str_p, const char* c, cy_uint index)
{
	// make sure that the index is valid
	if(index >= get_char_count_dstring(str_p))
		return 0;

	// update the char at index with the value at pointer c
	get_byte_array_dstring(str_p)[index] = (*c);
	return 1;
}

static int swap_chars_at_in_dstring(dstring* str_p, cy_uint i1, cy_uint i2)
{
	// make sure that the indices are valid
	if(i1 >= get_char_count_dstring(str_p) || i2 >= get_char_count_dstring(str_p))
		return 0;

	// swap the character at i1 and i2
	char temp = get_byte_array_dstring(str_p)[i1];
	get_byte_array_dstring(str_p)[i1] = get_byte_array_dstring(str_p)[i2];
	get_byte_array_dstring(str_p)[i2] = temp;
	return 1;
}

index_accessed_interface get_index_accessed_interface_for_dstring(dstring* str_p)
{
	return (index_accessed_interface){
		.ds_p = str_p,
		.get_element = ((const void* (*)(const void*, cy_uint))get_char_at_from_dstring),
		.set_element = ((int (*)(void*, const void*, cy_uint))set_char_at_in_dstring),
		.swap_elements = ((int (*)(void*, cy_uint, cy_uint))swap_chars_at_in_dstring),
		.get_element_count = ((cy_uint (*)(const void*))get_char_count_dstring),
	};
}

#include<cutlery/dstring_char.h>

int compare_chars_ascending_at(const void* c1, const void* c2)
{
	return compare_chars_ascending(*((const char*)c1), *((const char*)c2));
}

int compare_chars_descending_at(const void* c1, const void* c2)
{
	return compare_chars_descending(*((const char*)c1), *((const char*)c2));
}