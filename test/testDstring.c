#include<stdio.h>
#include<stdlib.h>
#include<dstring.h>

int main()
{
	dstring* str = get_dstring("", 1);
	display(str);

	str = append(str, "Hello ");
	display(str);

	str = concatenate(str, get_dstring("World, as said by Rohan\n", 2));
	display(str);

	delete_dstring(str);
}