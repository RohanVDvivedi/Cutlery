#include<stdio.h>
#include<stdlib.h>
#include<dstring.h>

int main()
{
	dstring str = new_dstring(NULL, 0);
	printf("str : \"" printf_dstring_format "\"\n\n", printf_dstring_params(&str));

	concatenate_dstring(&str, &get_dstring_pointing_to_literal_cstring("Hello "));
	printf("str : \"" printf_dstring_format "\"\n\n", printf_dstring_params(&str));

	{
		const dstring* tempstr = &get_dstring_pointing_to_literal_cstring("World, as said by Rohan AAACAAAAAC BBCBBD BBCBBD");
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
			printf("prefix \"%s\" => %d\n", temp[i], is_prefix_of_dstring(&str, &get_dstring_pointing_to_cstring(temp[i])));
			printf("\n\n");
		}

		printf("\n\n");
	}

	{
		char* temp[] = {
			"",
			"D",
			"BCBBD",
			"BBCBBD",
			"BBCBBDX",
			"XBBCBBD",
			" BBCBBD",
			"Hello World, as said by Rohan AAACAAAAAC BBCBBD BBCBBD",
		};

		int temp_size = sizeof(temp) / sizeof(char*);

		for(int i = 0; i < temp_size; i++)
		{
			printf("suffix \"%s\" => %d\n", temp[i], is_suffix_of_dstring(&str, &get_dstring_pointing_to_cstring(temp[i])));
			printf("\n\n");
		}

		printf("\n\n");
	}

	snprintf_dstring(&str, "iNt no : %d, strIng : %s, floAT : %f", 336, "Heello Worlds!! =+", 35.49);
	printf("str : \"" printf_dstring_format "\"\n\n\n", printf_dstring_params(&str));

	printf("Created a literal dstring : " printf_dstring_format "\n\n", printf_dstring_params(&get_dstring_pointing_to(get_byte_array_dstring(&str) + 4, 10)));

	const dstring* slize = &get_dstring_pointing_to(get_byte_array_dstring(&str), get_char_count_dstring(&str));
	printf("Created a literal dstring (slize) : \"" printf_dstring_format "\"\n", printf_dstring_params(slize));
	printf("on compare to str = case_compare(str,slize) = %d\n\n", case_compare_dstring(&str, slize));

	slize = &get_dstring_pointing_to(get_byte_array_dstring(&str), get_char_count_dstring(&str));
	printf("Created a literal dstring (slize) : \"" printf_dstring_format "\"\n", printf_dstring_params(slize));
	printf("on compare to str = compare(str,slize) = %d\n\n", compare_dstring(&str, slize));

	slize = &get_dstring_pointing_to(get_byte_array_dstring(&str), 20);
	printf("Created a literal dstring (slize) : \"" printf_dstring_format "\"\n", printf_dstring_params(slize));
	printf("on compare to str = case_compare(str,slize) = %d\n\n", case_compare_dstring(&str, slize));

	slize = &get_dstring_pointing_to(get_byte_array_dstring(&str), 20);
	printf("Created a literal dstring (slize) : \"" printf_dstring_format "\"\n", printf_dstring_params(slize));
	printf("on compare to str = compare(str,slize) = %d\n\n", compare_dstring(&str, slize));

	slize = &get_dstring_pointing_to_dstring(&str);
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

			const dstring* temp_dstr = &get_dstring_pointing_to_literal_cstring(temp[i]);

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

			const dstring* temp_dstr = &get_dstring_pointing_to_cstring(temp[i]);

			unsigned int* suffix_prefix_match_length = alloca(sizeof(unsigned int) * (get_char_count_dstring(temp_dstr) + 1));
			get_prefix_suffix_match_lengths(temp_dstr, suffix_prefix_match_length);

			printf("SUFFIX_PREFIX_MATCH_LENGTHS : \n");
			for(unsigned int l = 0; l <= get_char_count_dstring(temp_dstr); l++)
				printf(printf_dstring_format " -> %u\n", printf_dstring_params(&get_dstring_pointing_to(temp[i], l)), suffix_prefix_match_length[l]);
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
			const dstring* s0 = &get_dstring_pointing_to_cstring(temp[i][0]);
			const dstring* s1 = &get_dstring_pointing_to_cstring(temp[i][1]);
			printf("leven-dist(\"%s\", \"%s\") = %u\n", temp[i][0], temp[i][1], levenshtein_distance(s0, s1));
			printf("len-of-lcs(\"%s\", \"%s\") = %u\n", temp[i][0], temp[i][1], length_of_longest_common_subsequence(s0, s1));
			printf("\n\n");
		}

		printf("\n\n");
	}

	// tests for ltrim rtrim functions
	dstring s1;
	dstring s2;

	// ltrim test
	s1 = get_dstring_pointing_to_literal_cstring("");
	s2 = new_copy_dstring(&s1);
	ltrim_dstring(&s1);
	ltrim_dstring(&s2);
	printf("after ltrim : <"printf_dstring_format "> <"printf_dstring_format ">\n\n", printf_dstring_params(&s1), printf_dstring_params(&s2));
	deinit_dstring(&s2);

	s1 = get_dstring_pointing_to_literal_cstring("Rohan");
	s2 = new_copy_dstring(&s1);
	ltrim_dstring(&s1);
	ltrim_dstring(&s2);
	printf("after ltrim : <"printf_dstring_format "> <"printf_dstring_format ">\n\n", printf_dstring_params(&s1), printf_dstring_params(&s2));
	deinit_dstring(&s2);

	s1 = get_dstring_pointing_to_literal_cstring("   Rohan");
	s2 = new_copy_dstring(&s1);
	ltrim_dstring(&s1);
	ltrim_dstring(&s2);
	printf("after ltrim : <"printf_dstring_format "> <"printf_dstring_format ">\n\n", printf_dstring_params(&s1), printf_dstring_params(&s2));
	deinit_dstring(&s2);

	s1 = get_dstring_pointing_to_literal_cstring("Rohan   ");
	s2 = new_copy_dstring(&s1);
	ltrim_dstring(&s1);
	ltrim_dstring(&s2);
	printf("after ltrim : <"printf_dstring_format "> <"printf_dstring_format ">\n\n", printf_dstring_params(&s1), printf_dstring_params(&s2));
	deinit_dstring(&s2);

	s1 = get_dstring_pointing_to_literal_cstring("   Rohan   ");
	s2 = new_copy_dstring(&s1);
	ltrim_dstring(&s1);
	ltrim_dstring(&s2);
	printf("after ltrim : <"printf_dstring_format "> <"printf_dstring_format ">\n\n", printf_dstring_params(&s1), printf_dstring_params(&s2));
	deinit_dstring(&s2);

	// rtrim test
	s1 = get_dstring_pointing_to_literal_cstring("");
	s2 = new_copy_dstring(&s1);
	rtrim_dstring(&s1);
	rtrim_dstring(&s2);
	printf("after ltrim : <"printf_dstring_format "> <"printf_dstring_format ">\n\n", printf_dstring_params(&s1), printf_dstring_params(&s2));
	deinit_dstring(&s2);

	s1 = get_dstring_pointing_to_literal_cstring("Rohan");
	s2 = new_copy_dstring(&s1);
	rtrim_dstring(&s1);
	rtrim_dstring(&s2);
	printf("after ltrim : <"printf_dstring_format "> <"printf_dstring_format ">\n\n", printf_dstring_params(&s1), printf_dstring_params(&s2));
	deinit_dstring(&s2);

	s1 = get_dstring_pointing_to_literal_cstring("   Rohan");
	s2 = new_copy_dstring(&s1);
	rtrim_dstring(&s1);
	rtrim_dstring(&s2);
	printf("after ltrim : <"printf_dstring_format "> <"printf_dstring_format ">\n\n", printf_dstring_params(&s1), printf_dstring_params(&s2));
	deinit_dstring(&s2);

	s1 = get_dstring_pointing_to_literal_cstring("Rohan   ");
	s2 = new_copy_dstring(&s1);
	rtrim_dstring(&s1);
	rtrim_dstring(&s2);
	printf("after ltrim : <"printf_dstring_format "> <"printf_dstring_format ">\n\n", printf_dstring_params(&s1), printf_dstring_params(&s2));
	deinit_dstring(&s2);

	s1 = get_dstring_pointing_to_literal_cstring("   Rohan   ");
	s2 = new_copy_dstring(&s1);
	rtrim_dstring(&s1);
	rtrim_dstring(&s2);
	printf("after ltrim : <"printf_dstring_format "> <"printf_dstring_format ">\n\n", printf_dstring_params(&s1), printf_dstring_params(&s2));
	deinit_dstring(&s2);

	// tests for split_dstring
	{
		dstring str = get_dstring_pointing_to_literal_cstring("Vipul, Rupa, Devashree, Rohan, , Jumbo");
		dstring delim = get_dstring_pointing_to_literal_cstring(", ");

		dstring remaining = str;
		while(!is_empty_dstring(&remaining))
		{
			dstring split;
			remaining = split_dstring(&remaining, &delim, &split);
			printf("<" printf_dstring_format ">\n", printf_dstring_params(&split));
		}
	}

	printf("\n\n");

	// tests for split_dstring, using an effective for loop
	{
		dstring str = get_dstring_pointing_to_literal_cstring("Vipul, Rupa, Devashree, Rohan, , Jumbo");
		dstring delim = get_dstring_pointing_to_literal_cstring(", ");

		for(dstring split, remaining = split_dstring(&str, &delim, &split); (!is_empty_dstring(&remaining)) || (!is_empty_dstring(&split)); remaining = split_dstring(&remaining, &delim, &split))
			printf("<" printf_dstring_format ">\n", printf_dstring_params(&split));
	}

	printf("\n\n");

	{
		dstring s = get_dstring_pointing_to_literal_cstring("123");
		unsigned int i = 0;
		int r = 0;
		printf("s = <" printf_dstring_format ">\n", printf_dstring_params(&s));
		r = get_unsigned_int_from_dstring(&s, 8, &i);
		printf("unsigned int = %u 0x%x 0%o in base 8 with result %d\n", i, i, i, r);
		r = get_unsigned_int_from_dstring(&s, 10, &i);
		printf("unsigned int = %u 0x%x 0%o in base 10 with result %d\n", i, i, i, r);
		r = get_unsigned_int_from_dstring(&s, 16, &i);
		printf("unsigned int = %u 0x%x 0%o in base 16 with result %d\n", i, i, i, r);
		r = get_unsigned_int_from_dstring(&s, 2, &i);
		printf("unsigned int = %u 0x%x 0%o in base 2 with result %d\n", i, i, i, r);
	}
}