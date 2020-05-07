#ifndef BUCKET_ARRAY_H
#define BUCKET_ARRAY_H

#include<array.h>

#define BUCKET_SIZE					2
#define KEY_INDEX(BUCKET_INDEX)		(2 * BUCKET_INDEX)
#define VALUE_INDEX(BUCKET_INDEX) 	KEY_INDEX(BUCKET_INDEX) + 1

// returns a new array with total_size as the initial size, with no elements inside,
#define get_bucket_array(initial_size) get_array(initial_size * BUCKET_SIZE)

// initializes and gives necessary memory to manage buckets
#define initialize_bucket_array(array_p, initial_size) initialize_array(array_p, initial_size * BUCKET_SIZE)

// returns the maximum number of buckets that can be stored
#define get_capacity_bucket_array(array_p) (array_p->total_size / BUCKET_SIZE)

// returns pointer to the key for a bucket at index = index
#define get_key_bucket_array(array_p, index) get_element(array_p, KEY_INDEX(index))

// returns pointer to the value for a bucket at index = index
#define get_value_bucket_array(array_p, index) get_element(array_p, VALUE_INDEX(index))

// returns 0 for success, -1 for index out of bounds
#define insert_in_bucket_array(array_p, key_p, value_p, index) (set_element(array_p, value_p, VALUE_INDEX(index)) && set_element(array_p, key_p, KEY_INDEX(index)))

// shrink counter part of bucket array
// shrinks the array, if the array is considerably larger than required
// bucket at the start_index comes to 0, and bucket at end_index comes to end_index - start_index
// it returns 1 if the array was shrunk
#define shrink_bucket_array(array_p, start_index, end_index) shrink_array(array_p, KEY_INDEX(start_index), VALUE_INDEX(end_index))

void for_each_in_bucket_array(const array* array_p, void (*operation)(const void* key, const void* value, unsigned long long int index, const void* additional_params), const void* additional_params);

void print_bucket_array(const array* array_p, void (*print_key)(const void* key), void (*print_value)(const void* value));

#endif