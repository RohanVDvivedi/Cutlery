#include<stdio.h>
#include<stdlib.h>
#include<dstring.h>

int main()
{
	dstring str;
	init_dstring(&str, "");
	printf("str : ");display_dstring(&str);printf("\n");

	append_to_dstring(&str, "Hello ");
	printf("str : ");display_dstring(&str);printf("\n");

	dstring tempstr;
	init_dstring(&tempstr, "World, as said by Rohan\n");
	printf("tempstr : ");display_dstring(&tempstr);printf("\n");
	concatenate_dstring(&str, &tempstr);
	deinit_dstring(&tempstr);
	printf("str : ");display_dstring(&str);printf("\n");

	char* temp = NULL;

	temp = "";
	printf("prefix %s %d\n", temp, is_prefix(&str, temp));
	temp = "H";
	printf("prefix %s %d\n", temp, is_prefix(&str, temp));
	temp = "Helt";
	printf("prefix %s %d\n", temp, is_prefix(&str, temp));
	temp = "Hell";
	printf("prefix %s %d\n", temp, is_prefix(&str, temp));
	temp = "Hello World";
	printf("prefix %s %d\n", temp, is_prefix(&str, temp));
	temp = "Hello Worlds";
	printf("prefix %s %d\n", temp, is_prefix(&str, temp));
	temp = "Hello World, as said by Rohan\n";
	printf("prefix %s %d\n", temp, is_prefix(&str, temp));
	temp = "Hello World, as said by Rohan\n, Hello World";
	printf("prefix %s %d\n", temp, is_prefix(&str, temp));

	append_to_dstring_formatted(&str, "iNt no : %d, strIng : %s, floAT : %f\n", 336, "Heello Worlds!! =+", 35.49);
	display_dstring(&str);

	toLowercase(&str);
	display_dstring(&str);

	toUppercase(&str);
	display_dstring(&str);

	dstring slize;
	init_dstring_slize(&slize, str.cstring + 4, 10);
	display_dstring(&slize);
	deinit_dstring(&slize);

	deinit_dstring(&str);
}