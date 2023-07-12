#ifndef CUTLERY_MATH_H
#define CUTLERY_MATH_H

#include<cutlery_stds.h>

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))
#define compare(a,b) (((a)<(b))?(-1):(((a)>(b))?(1):(0)))

// below 2 macros can be used to detect overflow on unsigned numeric data types on sum or multiplication, considering that A and B both are of the same type
#define will_unsigned_sum_overflow(type_of_A_n_B, A, B)                ((B) > ((UNSIGNED_MAX_VALUE_OF(type_of_A_n_B)) - (A)))
#define will_unsigned_mul_overflow(type_of_A_n_B, A, B) (((A) != 0) && ((B) > ((UNSIGNED_MAX_VALUE_OF(type_of_A_n_B)) / (A))))

#define will_signed_sum_overflow(type_of_A_n_B, A, B)		(((A) > 0) && ((B) > (SIGNED_MAX_VALUE_OF(type_of_A_n_B) - (A))))
#define will_signed_sum_underflow(type_of_A_n_B, A, B)		(((A) < 0) && ((B) < (SIGNED_MIN_VALUE_OF(type_of_A_n_B) - (A))))

#define will_signed_sub_overflow(type_of_A_n_B, A, B)		(((A) > 0) && ((B) < ((A) - SIGNED_MAX_VALUE_OF(type_of_A_n_B))))
#define will_signed_sub_underflow(type_of_A_n_B, A, B)		(((B) > 0) && ((A) < ((B) + SIGNED_MIN_VALUE_OF(type_of_A_n_B))))

#endif