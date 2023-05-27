#ifndef DSTRING_DIFF_H
#define DSTRING_DIFF_H

#include<dstring.h>

cy_uint levenshtein_distance(const dstring* str0, const dstring* str1);

cy_uint length_of_longest_common_subsequence(const dstring* str0, const dstring* str1);

#endif