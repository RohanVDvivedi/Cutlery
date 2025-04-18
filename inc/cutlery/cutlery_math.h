#ifndef CUTLERY_MATH_H
#define CUTLERY_MATH_H

#include<cutlery/cutlery_stds.h>

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))
#define compare_numbers(a,b) (((a)<(b))?(-1):(((a)>(b))?(1):(0)))

// below 2 macros can be used to detect overflow on unsigned numeric data types on sum or multiplication, considering that A and B both are of the same type
#define will_unsigned_sum_overflow(type_of_A_n_B, A, B)                ((B) > ((UNSIGNED_MAX_VALUE_OF(type_of_A_n_B)) - (A)))
#define will_unsigned_mul_overflow(type_of_A_n_B, A, B) (((A) != 0) && ((B) > ((UNSIGNED_MAX_VALUE_OF(type_of_A_n_B)) / (A))))

#define will_signed_sum_overflow(type_of_A_n_B, A, B)		(((A) > 0) && ((B) > (SIGNED_MAX_VALUE_OF(type_of_A_n_B) - (A))))
#define will_signed_sum_underflow(type_of_A_n_B, A, B)		(((A) < 0) && ((B) < (SIGNED_MIN_VALUE_OF(type_of_A_n_B) - (A))))

#define will_signed_sub_overflow(type_of_A_n_B, A, B)		(((A) > 0) && ((B) < ((A) - SIGNED_MAX_VALUE_OF(type_of_A_n_B))))
#define will_signed_sub_underflow(type_of_A_n_B, A, B)		(((B) > 0) && ((A) < ((B) + SIGNED_MIN_VALUE_OF(type_of_A_n_B))))

#define will_signed_mul_overflow(type_of_A_n_B, A, B)	\
	((((A) > 0) && ((B) > 0) && ((B) > (SIGNED_MAX_VALUE_OF(type_of_A_n_B) / (A)))) || \
	(((A) < 0) && ((B) < 0) && ((B) < (SIGNED_MAX_VALUE_OF(type_of_A_n_B) / (A)))))

#define will_signed_mul_underflow(type_of_A_n_B, A, B)	\
	((((A) < 0) && ((B) > 0) && ((B) > (SIGNED_MIN_VALUE_OF(type_of_A_n_B) / (A)))) || \
	(((A) > 0) && ((B) < 0) && ((B) < (SIGNED_MIN_VALUE_OF(type_of_A_n_B) / (A)))))

#define will_UINT_ALIGN_UP_overflow(type_of_x, x, alignment)   will_unsigned_sum_overflow(type_of_x, UINT_ALIGN_DOWN((x), (alignment)), ( ((x)%(alignment)) ? (alignment) : 0 ))

/*
**	There should be some times when your custom very simple hash function, would not generate a random enough hash value
**	OR there is a linear relation ship between the hash value generated by you, giving you very bad distribution of objects over the buckets of the hash table
**	To mitigate this the below hash_randomizer is provided to be applied to your output hash value to effectively randomize it
*/
/*
** This could be used to hash, the linear relationsships generated for aligned pointers OR page_id for disk accessed that can be multiples of bytes in page OR blocks in page OR some other simple linear increments
*/
static inline cy_uint hash_randomizer(cy_uint input)
{
	unsigned long long int x = input;
	x ^= x >> 33ULL;
	x *= 0x9E3779B97F4A7C15ULL;
	x ^= x >> 29ULL;
	x *= 0xC4CEB9FE1A85EC53ULL;
	x ^= x >> 33ULL;
	return x;
}

#endif