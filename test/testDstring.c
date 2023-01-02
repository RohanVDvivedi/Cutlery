#include<stdio.h>
#include<stdlib.h>
#include<dstring.h>

int main()
{
	dstring str = new_dstring(NULL, 0);
	printf("str : \"" printf_dstring_format "\"\n\n", printf_dstring_params(&str));

	concatenate_dstring(&str, &get_literal_cstring("Hello "));
	printf("str : \"" printf_dstring_format "\"\n\n", printf_dstring_params(&str));

	{
		const dstring* tempstr = &get_literal_cstring("World, as said by Rohan AAACAAAAAC BBCBBD BBCBBD");
		printf("concatenate str with tempstr : \"" printf_dstring_format "\"\n", printf_dstring_params(tempstr));
		concatenate_dstring(&str, tempstr);
		printf("str : \"" printf_dstring_format "\"\n\n", printf_dstring_params(&str));
	}

	{
		char* temp[] = {
			"",
			"H",
			"Helt",
			"Hell",
			"Hello World",
			"Hello Worlds",
			"Hello World, as said by Rohan",
			"Hello World, as said by Rohan, Hello World",
		};

		int temp_size = sizeof(temp) / sizeof(char*);

		for(int i = 0; i < temp_size; i++)
		{
			printf("prefix \"%s\" => %d\n", temp[i], is_prefix_of_dstring(&str, &get_literal_cstring(temp[i])));
			printf("\n\n");
		}

		printf("\n\n");
	}

	snprintf_dstring(&str, "iNt no : %d, strIng : %s, floAT : %f", 336, "Heello Worlds!! =+", 35.49);
	printf("str : \"" printf_dstring_format "\"\n\n\n", printf_dstring_params(&str));

	printf("Created a literal dstring : " printf_dstring_format "\n\n", printf_dstring_params(&get_literal_dstring(get_byte_array_dstring(&str) + 4, 10)));

	const dstring* slize = &get_literal_dstring(get_byte_array_dstring(&str), get_char_count_dstring(&str));
	printf("Created a literal dstring (slize) : \"" printf_dstring_format "\"\n", printf_dstring_params(slize));
	printf("on compare to str = case_compare(str,slize) = %d\n\n", case_compare_dstring(&str, slize));

	slize = &get_literal_dstring(get_byte_array_dstring(&str), get_char_count_dstring(&str));
	printf("Created a literal dstring (slize) : \"" printf_dstring_format "\"\n", printf_dstring_params(slize));
	printf("on compare to str = compare(str,slize) = %d\n\n", compare_dstring(&str, slize));

	slize = &get_literal_dstring(get_byte_array_dstring(&str), 20);
	printf("Created a literal dstring (slize) : \"" printf_dstring_format "\"\n", printf_dstring_params(slize));
	printf("on compare to str = case_compare(str,slize) = %d\n\n", case_compare_dstring(&str, slize));

	slize = &get_literal_dstring(get_byte_array_dstring(&str), 20);
	printf("Created a literal dstring (slize) : \"" printf_dstring_format "\"\n", printf_dstring_params(slize));
	printf("on compare to str = compare(str,slize) = %d\n\n", compare_dstring(&str, slize));

	slize = &get_literal_dstring(get_byte_array_dstring(&str), get_char_count_dstring(&str));
	printf("Created a literal dstring (slize) : \"" printf_dstring_format "\"\n", printf_dstring_params(slize));
	printf("on compare to str = case_compare(str,slize) = %d\n", case_compare_dstring(&str, slize));
	printf("on compare to str = compare(str,slize) = %d\n\n", compare_dstring(&str, slize));

	dstring* slize2 = &((dstring){});
	init_dstring(slize2, get_byte_array_dstring(&str), get_char_count_dstring(&str));
	to_uppercase_dstring(slize2);
	printf("Created an uppercase dstring (slize2) : \"" printf_dstring_format "\"\n", printf_dstring_params(slize2));
	printf("on compare to str = case_compare(str,slize2) = %d\n", case_compare_dstring(&str, slize2));
	printf("on compare to str = compare(str,slize2) = %d\n\n", compare_dstring(&str, slize2));
	deinit_dstring(slize2);

	printf("\n\nCONTAINS_DSTRING algorithms with str\n\n");

	printf("str : \"" printf_dstring_format "\"\n\n", printf_dstring_params(&str));

	{
		char* temp[] = {
			"Hello World",
			"Rohan",
			"said by",
			"said byt",
			"rHello World",
			"sRohan",
			"Hello Worldp",
			"Rohano",
			"Hellyo World",
			"Rohhan",
		};

		int temp_size = sizeof(temp) / sizeof(char*);

		for(int i = 0; i < temp_size; i++)
		{
			printf("contains_dstring : %s\n", temp[i]);

			const dstring* temp_dstr = &get_literal_cstring(temp[i]);

			printf("NAIVE : %u\n", contains_dstring_NAIVE(&str, temp_dstr));

			unsigned int* suffix_prefix_match_length = alloca(sizeof(unsigned int) * (get_char_count_dstring(temp_dstr) + 1));
			get_prefix_suffix_match_lengths(temp_dstr, suffix_prefix_match_length);
			printf("KMP   : %u\n", contains_dstring_KMP(&str, temp_dstr, suffix_prefix_match_length));

			printf("RK    : %u\n", contains_dstring_RK(&str, temp_dstr));

			printf("\n\n");
		}

		printf("\n\n");
	}

	printf("str : " printf_dstring_format "\n\n", printf_dstring_params(&str));

	{
		char* temp[] = {
			"AFFA",
			"BBCBBD",
			"ABCXXXABC",
			"ABCABCABC",
			"AAACAAAAAC",
			"AABAACAABAA",
		};

		int temp_size = sizeof(temp) / sizeof(char*);

		for(int i = 0; i < temp_size; i++)
		{
			printf("contains_dstring : %s\n", temp[i]);

			const dstring* temp_dstr = &get_literal_cstring(temp[i]);

			unsigned int* suffix_prefix_match_length = alloca(sizeof(unsigned int) * (get_char_count_dstring(temp_dstr) + 1));
			get_prefix_suffix_match_lengths(temp_dstr, suffix_prefix_match_length);

			printf("SUFFIX_PREFIX_MATCH_LENGTHS : \n");
			for(unsigned int l = 0; l <= get_char_count_dstring(temp_dstr); l++)
				printf(printf_dstring_format " -> %u\n", printf_dstring_params(&get_literal_dstring(temp[i], l)), suffix_prefix_match_length[l]);
			printf("\n");

			printf("NAIVE : %u\n", contains_dstring_NAIVE(&str, temp_dstr));
			printf("KMP   : %u\n", contains_dstring_KMP(&str, temp_dstr, suffix_prefix_match_length));
			printf("RK    : %u\n", contains_dstring_RK(&str, temp_dstr));

			printf("\n\n");
		}

		printf("\n\n");
	}

	printf("str : \"" printf_dstring_format "\"\n\n", printf_dstring_params(&str));

	to_lowercase_dstring(&str);
	printf("To lowercase str : \"" printf_dstring_format "\"\n\n", printf_dstring_params(&str));

	to_uppercase_dstring(&str);
	printf("To uppercase str : \"" printf_dstring_format "\"\n\n", printf_dstring_params(&str));

	printf("str : \"" printf_dstring_format "\"\n\n", printf_dstring_params(&str));

	discard_chars_dstring(&str, 15, 45);

	printf("str (discard_chars 15 to 45) : \"" printf_dstring_format "\"\n\n", printf_dstring_params(&str));

	deinit_dstring(&str);

	// levenshtein distance formula and length of longest common sub sequence
	{
		char* temp[][2] = {
			{
				"TRACE",
				"CRATE",
			},
			{
				"kitten",
				"sitten",
			},
			{
				"sitten",
				"sittin",
			},
			{
				"sittin",
				"sitting",
			},
			{
				"HONDA",
				"HYUNDAI",
			},
			{
				"rohan",
				"krina",
			},
		};

		int test_count = sizeof(temp) / (sizeof(temp[0]));

		for(int i = 0; i < test_count; i++)
		{
			const dstring* s0 = &get_literal_cstring(temp[i][0]);
			const dstring* s1 = &get_literal_cstring(temp[i][1]);
			printf("leven-dist(\"%s\", \"%s\") = %u\n", temp[i][0], temp[i][1], levenshtein_distance(s0, s1));
			printf("len-of-lcs(\"%s\", \"%s\") = %u\n", temp[i][0], temp[i][1], length_of_longest_common_subsequence(s0, s1));
			printf("\n\n");
		}

		printf("\n\n");
	}

	// tests for ltrim rtrim functions
	dstring strim;

	// ltrim test
	strim = new_copy_dstring(&get_literal_cstring(""));
	printf("<" printf_dstring_format ">\n", printf_dstring_params(&strim));
	ltrim_dstring(&strim);
	printf("after ltrim : <"printf_dstring_format ">\n\n", printf_dstring_params(&strim));
	deinit_dstring(&strim);

	strim = new_copy_dstring(&get_literal_cstring("Rohan"));
	printf("<" printf_dstring_format ">\n", printf_dstring_params(&strim));
	ltrim_dstring(&strim);
	printf("after ltrim : <"printf_dstring_format ">\n\n", printf_dstring_params(&strim));
	deinit_dstring(&strim);

	strim = new_copy_dstring(&get_literal_cstring("   Rohan"));
	printf("<" printf_dstring_format ">\n", printf_dstring_params(&strim));
	ltrim_dstring(&strim);
	printf("after ltrim : <"printf_dstring_format ">\n\n", printf_dstring_params(&strim));
	deinit_dstring(&strim);

	strim = new_copy_dstring(&get_literal_cstring("Rohan   "));
	printf("<" printf_dstring_format ">\n", printf_dstring_params(&strim));
	ltrim_dstring(&strim);
	printf("after ltrim : <"printf_dstring_format ">\n\n", printf_dstring_params(&strim));
	deinit_dstring(&strim);

	strim = new_copy_dstring(&get_literal_cstring("   Rohan   "));
	printf("<" printf_dstring_format ">\n", printf_dstring_params(&strim));
	ltrim_dstring(&strim);
	printf("after ltrim : <"printf_dstring_format ">\n\n", printf_dstring_params(&strim));
	deinit_dstring(&strim);

	// rtrim test
	strim = new_copy_dstring(&get_literal_cstring(""));
	printf("<" printf_dstring_format ">\n", printf_dstring_params(&strim));
	rtrim_dstring(&strim);
	printf("after rtrim : <"printf_dstring_format ">\n\n", printf_dstring_params(&strim));
	deinit_dstring(&strim);

	strim = new_copy_dstring(&get_literal_cstring("Rohan"));
	printf("<" printf_dstring_format ">\n", printf_dstring_params(&strim));
	rtrim_dstring(&strim);
	printf("after rtrim : <"printf_dstring_format ">\n\n", printf_dstring_params(&strim));
	deinit_dstring(&strim);

	strim = new_copy_dstring(&get_literal_cstring("   Rohan"));
	printf("<" printf_dstring_format ">\n", printf_dstring_params(&strim));
	rtrim_dstring(&strim);
	printf("after rtrim : <"printf_dstring_format ">\n\n", printf_dstring_params(&strim));
	deinit_dstring(&strim);

	strim = new_copy_dstring(&get_literal_cstring("Rohan   "));
	printf("<" printf_dstring_format ">\n", printf_dstring_params(&strim));
	rtrim_dstring(&strim);
	printf("after rtrim : <"printf_dstring_format ">\n\n", printf_dstring_params(&strim));
	deinit_dstring(&strim);

	strim = new_copy_dstring(&get_literal_cstring("   Rohan   "));
	printf("<" printf_dstring_format ">\n", printf_dstring_params(&strim));
	rtrim_dstring(&strim);
	printf("after rtrim : <"printf_dstring_format ">\n\n", printf_dstring_params(&strim));
	deinit_dstring(&strim);

	// tests for split_dstring
	{
		dstring str = get_literal_cstring("Vipul, Rupa, Devashree, Rohan, Jumbo");
		dstring delim = get_literal_cstring(", ");

		dstring remaining = str;
		while(!is_empty_dstring(&remaining))
		{
			dstring split;
			remaining = split_dstring(&remaining, &delim, &split);
			printf("<" printf_dstring_format ">\n", printf_dstring_params(&split));
		}
	}
}