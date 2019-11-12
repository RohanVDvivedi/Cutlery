#include<dstring.h>

dstring* get_dstring(const char* cstr_p, unsigned long long int additional_allocation)
{
	dstring* str_p = (dstring*)malloc(sizeof(dstring));
	init_dstring(str_p, cstr_p, additional_allocation);
	return str_p;
}

void init_dstring(dstring* str_p, const char* cstr_p, unsigned long long int additional_allocation)
{
	str_p->bytes_occupied = cstr_p == NULL ? 1 : (strlen(cstr_p) + 1);
	str_p->bytes_allocated = (2 * (str_p->bytes_occupied + 1)) + 2 + additional_allocation;
	str_p->cstring = (char*)malloc(str_p->bytes_allocated);
	str_p->cstring[0] = '\0';
	if(cstr_p != NULL)
	{
		memcpy(str_p->cstring, cstr_p, str_p->bytes_occupied);
	}
}

void make_dstring_empty(dstring* str_p)
{
	str_p->cstring[0] = '\0';
	str_p->bytes_occupied = 1;
}

int compare_dstring(const dstring* str_p1, const dstring* str_p2)
{
	return strcmp(str_p1->cstring, str_p2->cstring);
}

void expand_dstring(dstring* str_p, unsigned long long int additional_allocation)
{
	dstring expanded_dstring;
	init_dstring(&expanded_dstring, str_p->cstring, additional_allocation);
	if(str_p->cstring != NULL)
	{
		free(str_p->cstring);
	}
	(*str_p) = expanded_dstring;
}

void appendn_to_dstring(dstring* str_p, char* cstr_p, unsigned long long int occ)
{
	// we consider that the client has not considered counting '\0' in string
	if( occ + str_p->bytes_occupied > str_p->bytes_allocated)
	{
		expand_dstring(str_p, (2 * occ) + 2);
	}
	if(cstr_p != NULL)
	{
		memcpy(str_p->cstring + str_p->bytes_occupied - 1, cstr_p, occ);
		str_p->bytes_occupied += occ;
		str_p->cstring[str_p->bytes_occupied - 1] = '\0';
	}
}

void append_to_dstring(dstring* str_p, char* cstr_p)
{
	appendn_to_dstring(str_p, cstr_p, ((cstr_p == NULL) ? 0 : strlen(cstr_p)) );
}

void concatenate_dstring(dstring* str_p1, dstring* str_p2)
{
	append_to_dstring(str_p1, str_p2->cstring);
}

void toLowercase(dstring* str_p)
{
	char* stemp = str_p->cstring;
	while(*stemp != '\0')
	{
		*stemp = tolower(*stemp);
		stemp++;
	}
}

void toUppercase(dstring* str_p)
{
	char* stemp = str_p->cstring;
	while(*stemp != '\0')
	{
		*stemp = toupper(*stemp);
		stemp++;
	}
}

void display_dstring(dstring* str_p)
{
	printf("%s", str_p->cstring);
}

void delete_dstring(dstring* str_p)
{
	if(str_p->cstring != NULL)
	{
		free(str_p->cstring);
	}
	free(str_p);
}