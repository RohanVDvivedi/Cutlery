#include<stdio.h>
#include<stdlib.h>
#include<dstring.h>

int main()
{
	dstring str;init_dstring_from_cstring(&str, "");
	printf("str : ");printf_dstring(&str);printf("\n");

	append_to_dstring(&str, "Hello ");
	printf("str : ");printf_dstring(&str);printf("\n");

	{
		dstring* tempstr = dstring_DUMMY_CSTRING("World, as said by Rohan AAACAAAAAC BBCBBD BBCBBD");
		printf("concatenate str with tempstr : ");printf_dstring(tempstr);printf("\n");
		concatenate_dstring(&str, tempstr);
		printf("str : ");printf_dstring(&str);printf("\n\n");
	}

	{
		char* temp = "";
		printf("prefix %s %d\n", temp, is_prefix(&str, dstring_DUMMY_CSTRING(temp)));

		temp = "H";
		printf("prefix %s %d\n", temp, is_prefix(&str, dstring_DUMMY_CSTRING(temp)));
		
		temp = "Helt";
		printf("prefix %s %d\n", temp, is_prefix(&str, dstring_DUMMY_CSTRING(temp)));
		
		temp = "Hell";
		printf("prefix %s %d\n", temp, is_prefix(&str, dstring_DUMMY_CSTRING(temp)));
		
		temp = "Hello World";
		printf("prefix %s %d\n", temp, is_prefix(&str, dstring_DUMMY_CSTRING(temp)));
		
		temp = "Hello Worlds";
		printf("prefix %s %d\n", temp, is_prefix(&str, dstring_DUMMY_CSTRING(temp)));
		
		temp = "Hello World, as said by Rohan";
		printf("prefix %s %d\n", temp, is_prefix(&str, dstring_DUMMY_CSTRING(temp)));
		
		temp = "Hello World, as said by Rohan, Hello World";
		printf("prefix %s %d\n", temp, is_prefix(&str, dstring_DUMMY_CSTRING(temp)));
	}

	append_to_dstring_formatted(&str, "iNt no : %d, strIng : %s, floAT : %f", 336, "Heello Worlds!! =+", 35.49);
	printf_dstring(&str);
	printf("\n\n");

	printf("Created a dummy dstring : ");
	printf_dstring(dstring_DUMMY_DATA(str.cstring + 4, 10));
	printf("\n\n");

	printf("Created a slize : ");
	dstring* slize = dstring_DUMMY_DATA(str.cstring, str.bytes_occupied);
	printf_dstring(slize);
	printf("\non compare to str = case_compare(str,slize) = %d\n\n", case_compare_dstring(&str, slize));

	printf("Created a slize : ");
	slize = dstring_DUMMY_DATA(str.cstring, str.bytes_occupied);
	printf_dstring(slize);
	printf("\non compare to str = compare(str,slize) = %d\n\n", compare_dstring(&str, slize));

	printf("Created a slize : ");
	slize = dstring_DUMMY_DATA(str.cstring, 20);
	printf_dstring(slize);
	printf("\non compare to str = case_compare(str,slize) = %d\n\n", case_compare_dstring(&str, slize));

	printf("Created a slize : ");
	slize = dstring_DUMMY_DATA(str.cstring, 20);
	printf_dstring(slize);
	printf("\non compare to str = compare(str,slize) = %d\n\n", compare_dstring(&str, slize));

	init_dstring_slize(&slize, str.cstring, str.bytes_occupied);
	convert_slize_to_dstring(&slize);
	dstring capitalize_slize;
	init_dstring_slize(&capitalize_slize, slize.cstring + 4, 10);
	toUppercase(&capitalize_slize);
	deinit_dstring(&capitalize_slize);
	printf("Created a slize : ");
	printf_dstring(&slize);
	printf("\n\n");

	printf("\non compare to str = case_compare(str,slize) = %d\n\n", case_compare_dstring(&str, &slize));

	printf("\non compare to str = compare(str,slize) = %d\n\n", compare_dstring(&str, &slize));

	deinit_dstring(&slize);

	printf_dstring(&str);
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

	printf("\n\nsubstring using std and KMP algorithm\n");
	printf_dstring(&str);
	printf("\n\n");

	dstring substring1;init_dstring(&substring1, "AFFA");unsigned int* cache1 = alloca(sizeof(unsigned int) * (substring1.bytes_occupied+1));get_prefix_suffix_match_lengths(&substring1, cache1);
	for(unsigned int i = 0; i <= substring1.bytes_occupied; i++)
		printf("%u -> %u\n", i, cache1[i]);
	printf("\nsubstring1 : \"");printf_dstring(&substring1);printf("\"  O(m*n): %d, O(m+n): %d\n\n", contains_dstring(&str, &substring1, NULL), contains_dstring(&str, &substring1, cache1));
	deinit_dstring(&substring1);

	dstring substring2;init_dstring(&substring2, "BBCBBD");unsigned int* cache2 = alloca(sizeof(unsigned int) * (substring2.bytes_occupied+1));get_prefix_suffix_match_lengths(&substring2, cache2);
	for(unsigned int i = 0; i <= substring2.bytes_occupied; i++)
		printf("%u -> %u\n", i, cache2[i]);
	printf("\nsubstring2 : \"");printf_dstring(&substring2);printf("\"  O(m*n): %d, O(m+n): %d\n\n", contains_dstring(&str, &substring2, NULL), contains_dstring(&str, &substring2, cache2));
	deinit_dstring(&substring2);

	dstring substring3;init_dstring(&substring3, "AABAACAABAA");unsigned int* cache3 = alloca(sizeof(unsigned int) * (substring3.bytes_occupied+1));get_prefix_suffix_match_lengths(&substring3, cache3);
	for(unsigned int i = 0; i <= substring3.bytes_occupied; i++)
		printf("%u -> %u\n", i, cache3[i]);
	printf("\nsubstring3 : \"");printf_dstring(&substring3);printf("\"  O(m*n): %d, O(m+n): %d\n\n", contains_dstring(&str, &substring3, NULL), contains_dstring(&str, &substring3, cache3));
	deinit_dstring(&substring3);

	dstring substring4;init_dstring(&substring4, "AAACAAAAAC");unsigned int* cache4 = alloca(sizeof(unsigned int) * (substring4.bytes_occupied+1));get_prefix_suffix_match_lengths(&substring4, cache4);
	for(unsigned int i = 0; i <= substring4.bytes_occupied; i++)
		printf("%u -> %u\n", i, cache4[i]);
	printf("\nsubstring4 : \"");printf_dstring(&substring4);printf("\"  O(m*n): %d, O(m+n): %d\n\n", contains_dstring(&str, &substring4, NULL), contains_dstring(&str, &substring4, cache4));
	deinit_dstring(&substring4);

	toLowercase(&str);
	printf("To lowercase str : ");
	printf_dstring(&str);
	printf("\n\n");

	toUppercase(&str);
	printf("To uppercase str : ");
	printf_dstring(&str);
	printf("\n\n");

	deinit_dstring(&str);
}