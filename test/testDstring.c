#include<stdio.h>
#include<stdlib.h>
#include<dstring.h>

int main()
{
	dstring str;
	init_dstring(&str, "", 1);
	display_dstring(&str);printf("\n");

	append_to_dstring(&str, "Hello ");
	display_dstring(&str);printf("\n");

	dstring tempstr;
	init_dstring(&tempstr, "World, as said by Rohan\n", 2);
	concatenate_dstring(&str, &tempstr);
	deinit_dstring(&tempstr);
	display_dstring(&str);printf("\n");

	display_dstring(&str);printf("\n");
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

	append_to_dstring_formatted(&str, "int no : %d, string : %s, float : %f\n", 336, "Heello Worlds!! =+", 35.49);

	display_dstring(&str);

	deinit_dstring(&str);
}