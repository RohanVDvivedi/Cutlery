#ifndef DSTRING_DIFF_H
#define DSTRING_DIFF_H

#include<dstring.h>

unsigned int levenshtein_distance(dstring* str0, dstring* str1);

unsigned int length_of_longest_common_subsequence(dstring* str0, dstring* str1);

#endif