#include<stdio.h>
#include<stdlib.h>
#include<dstring.h>

int main()
{
	dstring str;init_dstring_from_cstring(&str, "");
	printf("str : ");printf_dstring(&str);printf("\n");

	concatenate_dstring(&str, dstring_DUMMY_CSTRING("Hello "));
	printf("str : ");printf_dstring(&str);printf("\n");

	{
		const dstring* tempstr = dstring_DUMMY_CSTRING("World, as said by Rohan AAACAAAAAC BBCBBD BBCBBD");
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

	snprintf_dstring(&str, "iNt no : %d, strIng : %s, floAT : %f", 336, "Heello Worlds!! =+", 35.49);
	printf_dstring(&str);
	printf("\n\n");

	printf("Created a dummy dstring : ");
	printf_dstring(dstring_DUMMY_DATA(str.cstring + 4, 10));
	printf("\n\n");

	printf("Created a slize : ");
	const dstring* slize = dstring_DUMMY_DATA(str.cstring, str.bytes_occupied);
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

	printf("Created a slize : ");
	slize = dstring_DUMMY_DATA(str.cstring, str.bytes_occupied);
	printf_dstring(slize);
	printf("\n\n");

	printf("\non compare to str = case_compare(str,slize) = %d\n\n", case_compare_dstring(&str, slize));
	printf("\non compare to str = compare(str,slize) = %d\n\n", compare_dstring(&str, slize));

	printf("Created a slize : ");
	dstring* slize2 = &((dstring){});init_dstring(slize2, str.cstring, str.bytes_occupied);
	toUppercase(slize2);
	printf_dstring(slize2);
	printf("\n\n");
	printf("\non compare to str = case_compare(str,slize2) = %d\n\n", case_compare_dstring(&str, slize2));
	printf("\non compare to str = compare(str,slize2) = %d\n\n", compare_dstring(&str, slize2));
	deinit_dstring(slize2);

	printf_dstring(&str);
	printf("\n\n");

	{
		char* temp = "Hello World";
		printf("Testing contains function for \"%s\" in str => %d\n\n", temp, contains_dstring(&str, dstring_DUMMY_CSTRING(temp), NULL));

		temp = "Rohan";
		printf("Testing contains function for \"%s\" in str => %d\n\n", temp, contains_dstring(&str, dstring_DUMMY_CSTRING(temp), NULL));
		
		temp = "said by";
		printf("Testing contains function for \"%s\" in str => %d\n\n", temp, contains_dstring(&str, dstring_DUMMY_CSTRING(temp), NULL));
		
		temp = "said byt";
		printf("Testing contains function for \"%s\" in str => %d\n\n", temp, contains_dstring(&str, dstring_DUMMY_CSTRING(temp), NULL));
		
		temp = "rHello World";
		printf("Testing contains function for \"%s\" in str => %d\n\n", temp, contains_dstring(&str, dstring_DUMMY_CSTRING(temp), NULL));
		
		temp = "sRohan";
		printf("Testing contains function for \"%s\" in str => %d\n\n", temp, contains_dstring(&str, dstring_DUMMY_CSTRING(temp), NULL));
		
		temp = "Hello Worldp";
		printf("Testing contains function for \"%s\" in str => %d\n\n", temp, contains_dstring(&str, dstring_DUMMY_CSTRING(temp), NULL));
		
		temp = "Rohano";
		printf("Testing contains function for \"%s\" in str => %d\n\n", temp, contains_dstring(&str, dstring_DUMMY_CSTRING(temp), NULL));
		
		temp = "Hellyo World";
		printf("Testing contains function for \"%s\" in str => %d\n\n", temp, contains_dstring(&str, dstring_DUMMY_CSTRING(temp), NULL));
		
		temp = "Rohhan";
		printf("Testing contains function for \"%s\" in str => %d\n\n", temp, contains_dstring(&str, dstring_DUMMY_CSTRING(temp), NULL));
	}

	printf("\n\nsubstring using std and KMP algorithm\n");
	printf_dstring(&str);
	printf("\n\n");

	unsigned int* cache = NULL;

	slize = dstring_DUMMY_CSTRING("AFFA");
	cache = alloca(sizeof(unsigned int) * (slize->bytes_occupied+1));get_prefix_suffix_match_lengths(slize, cache);
	for(unsigned int i = 0; i <= slize->bytes_occupied; i++){printf("%u -> %u\n", i, cache[i]);}
	printf("\nslize : \"");printf_dstring(slize);printf("\"  O(m*n): %d, O(m+n): %d\n\n", contains_dstring(&str, slize, NULL), contains_dstring(&str, slize, cache));

	slize = dstring_DUMMY_CSTRING("BBCBBD");
	cache = alloca(sizeof(unsigned int) * (slize->bytes_occupied+1));get_prefix_suffix_match_lengths(slize, cache);
	for(unsigned int i = 0; i <= slize->bytes_occupied; i++){printf("%u -> %u\n", i, cache[i]);}
	printf("\nslize : \"");printf_dstring(slize);printf("\"  O(m*n): %d, O(m+n): %d\n\n", contains_dstring(&str, slize, NULL), contains_dstring(&str, slize, cache));

	slize = dstring_DUMMY_CSTRING("AABAACAABAA");
	cache = alloca(sizeof(unsigned int) * (slize->bytes_occupied+1));get_prefix_suffix_match_lengths(slize, cache);
	for(unsigned int i = 0; i <= slize->bytes_occupied; i++){printf("%u -> %u\n", i, cache[i]);}
	printf("\nslize : \"");printf_dstring(slize);printf("\"  O(m*n): %d, O(m+n): %d\n\n", contains_dstring(&str, slize, NULL), contains_dstring(&str, slize, cache));

	slize = dstring_DUMMY_CSTRING("AAACAAAAAC");
	cache = alloca(sizeof(unsigned int) * (slize->bytes_occupied+1));get_prefix_suffix_match_lengths(slize, cache);
	for(unsigned int i = 0; i <= slize->bytes_occupied; i++){printf("%u -> %u\n", i, cache[i]);}
	printf("\nslize : \"");printf_dstring(slize);printf("\"  O(m*n): %d, O(m+n): %d\n\n", contains_dstring(&str, slize, NULL), contains_dstring(&str, slize, cache));

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