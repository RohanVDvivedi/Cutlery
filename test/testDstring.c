#include<stdio.h>
#include<stdlib.h>
#include<dstring.h>

int main()
{
	dstring str = get_dstring(NULL, 0);
	printf("str : ");printf_dstring(&str);printf("\n");

	concatenate_dstring(&str, &get_literal_cstring("Hello "));
	printf("str : ");printf_dstring(&str);printf("\n");

	{
		const dstring* tempstr = &get_literal_cstring("World, as said by Rohan AAACAAAAAC BBCBBD BBCBBD");
		printf("concatenate str with tempstr : ");printf_dstring(tempstr);printf("\n");
		concatenate_dstring(&str, tempstr);
		printf("str : ");printf_dstring(&str);printf("\n\n");
	}

	{
		char* temp = "";
		printf("prefix %s %d\n", temp, is_prefix(&str, &get_literal_cstring(temp)));

		temp = "H";
		printf("prefix %s %d\n", temp, is_prefix(&str, &get_literal_cstring(temp)));
		
		temp = "Helt";
		printf("prefix %s %d\n", temp, is_prefix(&str, &get_literal_cstring(temp)));
		
		temp = "Hell";
		printf("prefix %s %d\n", temp, is_prefix(&str, &get_literal_cstring(temp)));
		
		temp = "Hello World";
		printf("prefix %s %d\n", temp, is_prefix(&str, &get_literal_cstring(temp)));
		
		temp = "Hello Worlds";
		printf("prefix %s %d\n", temp, is_prefix(&str, &get_literal_cstring(temp)));
		
		temp = "Hello World, as said by Rohan";
		printf("prefix %s %d\n", temp, is_prefix(&str, &get_literal_cstring(temp)));
		
		temp = "Hello World, as said by Rohan, Hello World";
		printf("prefix %s %d\n", temp, is_prefix(&str, &get_literal_cstring(temp)));
	}

	snprintf_dstring(&str, "iNt no : %d, strIng : %s, floAT : %f", 336, "Heello Worlds!! =+", 35.49);
	printf_dstring(&str);
	printf("\n\n");

	printf("Created a literal dstring : ");
	printf_dstring(&get_literal_dstring(get_byte_array_dstring(&str) + 4, 10));
	printf("\n\n");

	printf("Created a slize : ");
	const dstring* slize = &get_literal_dstring(get_byte_array_dstring(&str), get_char_count_dstring(&str));
	printf_dstring(slize);
	printf("\non compare to str = case_compare(str,slize) = %d\n\n", case_compare_dstring(&str, slize));

	printf("Created a slize : ");
	slize = &get_literal_dstring(get_byte_array_dstring(&str), get_char_count_dstring(&str));
	printf_dstring(slize);
	printf("\non compare to str = compare(str,slize) = %d\n\n", compare_dstring(&str, slize));

	printf("Created a slize : ");
	slize = &get_literal_dstring(get_byte_array_dstring(&str), 20);
	printf_dstring(slize);
	printf("\non compare to str = case_compare(str,slize) = %d\n\n", case_compare_dstring(&str, slize));

	printf("Created a slize : ");
	slize = &get_literal_dstring(get_byte_array_dstring(&str), 20);
	printf_dstring(slize);
	printf("\non compare to str = compare(str,slize) = %d\n\n", compare_dstring(&str, slize));

	printf("Created a slize : ");
	slize = &get_literal_dstring(get_byte_array_dstring(&str), get_char_count_dstring(&str));
	printf_dstring(slize);
	printf("\n\n");

	printf("\non compare to str = case_compare(str,slize) = %d\n\n", case_compare_dstring(&str, slize));
	printf("\non compare to str = compare(str,slize) = %d\n\n", compare_dstring(&str, slize));

	printf("Created a slize : ");
	dstring* slize2 = &((dstring){});init_dstring(slize2, get_byte_array_dstring(&str), get_char_count_dstring(&str));
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
		printf("Testing contains function for \"%s\" in str => %p\n\n", temp, contains_dstring(&str, &get_literal_cstring(temp), NULL));

		temp = "Rohan";
		printf("Testing contains function for \"%s\" in str => %p\n\n", temp, contains_dstring(&str, &get_literal_cstring(temp), NULL));
		
		temp = "said by";
		printf("Testing contains function for \"%s\" in str => %p\n\n", temp, contains_dstring(&str, &get_literal_cstring(temp), NULL));
		
		temp = "said byt";
		printf("Testing contains function for \"%s\" in str => %p\n\n", temp, contains_dstring(&str, &get_literal_cstring(temp), NULL));
		
		temp = "rHello World";
		printf("Testing contains function for \"%s\" in str => %p\n\n", temp, contains_dstring(&str, &get_literal_cstring(temp), NULL));
		
		temp = "sRohan";
		printf("Testing contains function for \"%s\" in str => %p\n\n", temp, contains_dstring(&str, &get_literal_cstring(temp), NULL));
		
		temp = "Hello Worldp";
		printf("Testing contains function for \"%s\" in str => %p\n\n", temp, contains_dstring(&str, &get_literal_cstring(temp), NULL));
		
		temp = "Rohano";
		printf("Testing contains function for \"%s\" in str => %p\n\n", temp, contains_dstring(&str, &get_literal_cstring(temp), NULL));
		
		temp = "Hellyo World";
		printf("Testing contains function for \"%s\" in str => %p\n\n", temp, contains_dstring(&str, &get_literal_cstring(temp), NULL));
		
		temp = "Rohhan";
		printf("Testing contains function for \"%s\" in str => %p\n\n", temp, contains_dstring(&str, &get_literal_cstring(temp), NULL));
	}

	printf("\n\nsubstring using std and KMP algorithm\n");
	printf_dstring(&str);
	printf("\n\n");

	unsigned int* cache = NULL;

	slize = &get_literal_cstring("AFFA");
	cache = alloca(sizeof(unsigned int) * (get_char_count_dstring(slize)+1));get_prefix_suffix_match_lengths(slize, cache);
	for(unsigned int i = 0; i <= get_char_count_dstring(slize); i++){printf("%u -> %u\n", i, cache[i]);}
	printf("\nslize : \"");printf_dstring(slize);printf("\"  O(m*n): %p, O(m+n): %p\n\n", contains_dstring(&str, slize, NULL), contains_dstring(&str, slize, cache));

	slize = &get_literal_cstring("BBCBBD");
	cache = alloca(sizeof(unsigned int) * (get_char_count_dstring(slize)+1));get_prefix_suffix_match_lengths(slize, cache);
	for(unsigned int i = 0; i <= get_char_count_dstring(slize); i++){printf("%u -> %u\n", i, cache[i]);}
	printf("\nslize : \"");printf_dstring(slize);printf("\"  O(m*n): %p, O(m+n): %p\n\n", contains_dstring(&str, slize, NULL), contains_dstring(&str, slize, cache));

	slize = &get_literal_cstring("AABAACAABAA");
	cache = alloca(sizeof(unsigned int) * (get_char_count_dstring(slize)+1));get_prefix_suffix_match_lengths(slize, cache);
	for(unsigned int i = 0; i <= get_char_count_dstring(slize); i++){printf("%u -> %u\n", i, cache[i]);}
	printf("\nslize : \"");printf_dstring(slize);printf("\"  O(m*n): %p, O(m+n): %p\n\n", contains_dstring(&str, slize, NULL), contains_dstring(&str, slize, cache));

	slize = &get_literal_cstring("AAACAAAAAC");
	cache = alloca(sizeof(unsigned int) * (get_char_count_dstring(slize)+1));get_prefix_suffix_match_lengths(slize, cache);
	for(unsigned int i = 0; i <= get_char_count_dstring(slize); i++){printf("%u -> %u\n", i, cache[i]);}
	printf("\nslize : \"");printf_dstring(slize);printf("\"  O(m*n): %p, O(m+n): %p\n\n", contains_dstring(&str, slize, NULL), contains_dstring(&str, slize, cache));

	toLowercase(&str);
	printf("To lowercase str : ");
	printf_dstring(&str);
	printf("\n\n");

	toUppercase(&str);
	printf("To uppercase str : ");
	printf_dstring(&str);
	printf("\n\n");

	deinit_dstring(&str);


	// levenshtein distance formula and length of longest common sub sequence
	{
		const dstring* s0 = &get_literal_cstring("TRACE");
		const dstring* s1 = &get_literal_cstring("CRATE");
		printf("leven-dist(\"%s\", \"%s\") = %u\n\n", s0->cstring, s1->cstring, levenshtein_distance(s0, s1));
		printf("len-of-lcs(\"%s\", \"%s\") = %u\n\n", s0->cstring, s1->cstring, length_of_longest_common_subsequence(s0, s1));
	}
}