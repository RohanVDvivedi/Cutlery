#ifndef DSTRING_DIFF_H
#define DSTRING_DIFF_H

#include<dstring.h>

unsigned int levenshtein_distance(const dstring* str0, const dstring* str1);

unsigned int length_of_longest_common_subsequence(const dstring* str0, const dstring* str1);

#endif