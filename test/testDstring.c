#include<stdio.h>
#include<stdlib.h>
#include<dstring.h>

int main()
{
	dstring* str = get_dstring("", 1);
	display_dstring(str);printf("\n");

	append_to_dstring(str, "Hello ");
	display_dstring(str);printf("\n");

	concatenate_dstring(str, get_dstring("World, as said by Rohan\n", 2));
	display_dstring(str);printf("\n");

	delete_dstring(str);
}