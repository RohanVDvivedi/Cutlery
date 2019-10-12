#include<dstring.h>

dstring* get_dstring(const char* cstr_p, unsigned long long int additional_allocation)
{
	dstring* str_p = (dstring*)malloc(sizeof(dstring));
	str_p->bytes_occupied = cstr_p == NULL ? 1 : (strlen(cstr_p) + 1);
	str_p->bytes_allocated = (2 * (str_p->bytes_occupied + 1)) + 2 + additional_allocation;
	str_p->cstring = (char*)malloc(str_p->bytes_allocated);
	str_p->cstring[0] = '\0';
	if(cstr_p != NULL)
	{
		memcpy(str_p->cstring, cstr_p, str_p->bytes_occupied);
	}
	return str_p;
}

dstring* make_dstring_empty(dstring* str_p)
{
	str_p->cstring[0] = '\0';
	str_p->bytes_occupied = 1;
	return str_p;
}

int compare_dstring(const dstring* str_p1, const dstring* str_p2)
{
	return strcmp(str_p1->cstring, str_p2->cstring);
}

dstring* expand_dstring(dstring* str_p, unsigned long long int additional_allocation)
{
	dstring* expanded_dstring = get_dstring(str_p->cstring, additional_allocation);
	delete_dstring(str_p);
	return expanded_dstring;
}

dstring* append_to_dstring(dstring* str_p, char* cstr_p)
{
	int occ = cstr_p == NULL ? 1 : (strlen(cstr_p) + 1);
	if( occ + str_p->bytes_occupied - 1 > str_p->bytes_allocated)
	{
		str_p = expand_dstring(str_p, (2 * occ) + 2);
	}
	if(cstr_p != NULL)
	{
		memcpy(str_p->cstring + str_p->bytes_occupied - 1, cstr_p, occ);
		str_p->bytes_occupied += (occ - 1);
	}
	return str_p;
}

dstring* concatenate_dstring(dstring* str_p1, dstring* str_p2)
{
	str_p1 = append_to_dstring(str_p1, str_p2->cstring);
	return str_p1;
}

void display_dstring(dstring* str_p)
{
	printf("%s", str_p->cstring);
}

void delete_dstring(dstring* str_p)
{
	free(str_p->cstring);
	free(str_p);
}