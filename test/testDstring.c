#include<stdio.h>
#include<stdlib.h>
#include<dstring.h>

int main()
{
	dstring* str = get_dstring("", 1);
	display_dstring(str);printf("\n");

	append_to_dstring(str, "Hello ");
	display_dstring(str);printf("\n");

	dstring* tempstr = get_dstring("World, as said by Rohan\n", 2);
	concatenate_dstring(str, tempstr);
	delete_dstring(tempstr);
	display_dstring(str);printf("\n");

	display_dstring(str);printf("\n");
	char* temp = NULL;

	temp = "";
	printf("prefix %s %d\n", temp, is_prefix(str, temp));
	temp = "H";
	printf("prefix %s %d\n", temp, is_prefix(str, temp));
	temp = "Helt";
	printf("prefix %s %d\n", temp, is_prefix(str, temp));
	temp = "Hell";
	printf("prefix %s %d\n", temp, is_prefix(str, temp));
	temp = "Hello World";
	printf("prefix %s %d\n", temp, is_prefix(str, temp));
	temp = "Hello Worlds";
	printf("prefix %s %d\n", temp, is_prefix(str, temp));
	temp = "Hello World, as said by Rohan\n";
	printf("prefix %s %d\n", temp, is_prefix(str, temp));
	temp = "Hello World, as said by Rohan\n, Hello World";
	printf("prefix %s %d\n", temp, is_prefix(str, temp));

	delete_dstring(str);
}