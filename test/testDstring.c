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
	init_dstring(&tempstr, "World, as said by Rohan");
	printf("tempstr : ");display_dstring(&tempstr);printf("\n");
	concatenate_dstring(&str, &tempstr);
	deinit_dstring(&tempstr);
	printf("str : ");display_dstring(&str);printf("\n\n");

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
	temp = "Hello World, as said by Rohan";
	printf("prefix %s %d\n", temp, is_prefix(&str, temp));
	temp = "Hello World, as said by Rohan, Hello World";
	printf("prefix %s %d\n", temp, is_prefix(&str, temp));

	append_to_dstring_formatted(&str, "iNt no : %d, strIng : %s, floAT : %f", 336, "Heello Worlds!! =+", 35.49);
	display_dstring(&str);
	printf("\n\n");

	dstring slize;
	printf("Created a slize : ");
	init_dstring_slize(&slize, str.cstring + 4, 10);
	display_dstring(&slize);
	printf("\n\n");

	printf("Created a slize : ");
	init_dstring_slize(&slize, str.cstring, str.bytes_occupied);
	display_dstring(&slize);
	printf("\non compare to str = case_compare(str,slize) = %d\n\n", case_compare_dstring(&str, &slize));

	printf("Created a slize : ");
	init_dstring_slize(&slize, str.cstring, str.bytes_occupied);
	display_dstring(&slize);
	printf("\non compare to str = compare(str,slize) = %d\n\n", compare_dstring(&str, &slize));

	printf("Created a slize : ");
	init_dstring_slize(&slize, str.cstring, 20);
	display_dstring(&slize);
	printf("\non compare to str = case_compare(str,slize) = %d\n\n", case_compare_dstring(&str, &slize));

	printf("Created a slize : ");
	init_dstring_slize(&slize, str.cstring, 20);
	display_dstring(&slize);
	printf("\non compare to str = compare(str,slize) = %d\n\n", compare_dstring(&str, &slize));

	init_dstring_slize(&slize, str.cstring, str.bytes_occupied);
	convert_slize_to_dstring(&slize);
	dstring capitalize_slize;
	init_dstring_slize(&capitalize_slize, slize.cstring + 4, 10);
	toUppercase(&capitalize_slize);
	deinit_dstring(&capitalize_slize);
	printf("Created a slize : ");
	display_dstring(&slize);
	printf("\n\n");

	printf("\non compare to str = case_compare(str,slize) = %d\n\n", case_compare_dstring(&str, &slize));

	printf("\non compare to str = compare(str,slize) = %d\n\n", compare_dstring(&str, &slize));

	deinit_dstring(&slize);

	display_dstring(&str);
	printf("\n\n");

	printf("Testing contains function for \"%s\" in str => %d\n\n", "Hello World", contains_cstring(&str, "Hello World"));
	printf("Testing contains function for \"%s\" in str => %d\n\n", "Rohan", contains_cstring(&str, "Rohan"));
	printf("Testing contains function for \"%s\" in str => %d\n\n", "said by", contains_cstring(&str, "said by"));
	printf("Testing contains function for \"%s\" in str => %d\n\n", "said byt", contains_cstring(&str, "said byt"));
	printf("Testing contains function for \"%s\" in str => %d\n\n", "rHello World", contains_cstring(&str, "rHello World"));
	printf("Testing contains function for \"%s\" in str => %d\n\n", "sRohan", contains_cstring(&str, "sRohan"));
	printf("Testing contains function for \"%s\" in str => %d\n\n", "Hello Worldp", contains_cstring(&str, "Hello Worldp"));
	printf("Testing contains function for \"%s\" in str => %d\n\n", "Rohano", contains_cstring(&str, "Rohano"));
	printf("Testing contains function for \"%s\" in str => %d\n\n", "Hellyo World", contains_cstring(&str, "Hellyo World"));
	printf("Testing contains function for \"%s\" in str => %d\n\n", "Rohhan", contains_cstring(&str, "Rohhan"));

	toLowercase(&str);
	printf("To lowercase str : ");
	display_dstring(&str);
	printf("\n\n");

	toUppercase(&str);
	printf("To uppercase str : ");
	display_dstring(&str);
	printf("\n\n");

	deinit_dstring(&str);
}