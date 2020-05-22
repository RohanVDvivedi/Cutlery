#ifndef BUCKET_ARRAY_H
#define BUCKET_ARRAY_H

#include<array.h>

#define BUCKET_SIZE					2
#define KEY_INDEX(BUCKET_INDEX)		(2 * BUCKET_INDEX)
#define VALUE_INDEX(BUCKET_INDEX) 	KEY_INDEX(BUCKET_INDEX) + 1

// returns a new array with total_size as the initial size, with no elements inside,
array* get_bucket_array(unsigned long long int initial_size);

// initializes and gives necessary memory to manage buckets
void initialize_bucket_array(array* array_p, unsigned long long int initial_size);

// returns the maximum number of buckets that can be stored
unsigned long long int get_capacity_bucket_array(const array* array_p);

// returns pointer to the key for a bucket at index = index
const void* get_key_bucket_array(const array* array_p, unsigned long long int index);

// returns pointer to the value for a bucket at index = index
const void* get_value_bucket_array(const array* array_p, unsigned long long int index);

// returns 0 for success, -1 for index out of bounds
int insert_in_bucket_array(array* array_p, const void* key_p, const void* value_p, unsigned long long int index);

// shrink counter part of bucket array
// shrinks the array, if the array is considerably larger than required
// bucket at the start_index comes to 0, and bucket at end_index comes to end_index - start_index
// it returns 1 if the array was shrunk
int shrink_bucket_array(array* array_p, unsigned long long int start_index, unsigned long long int end_index);

void for_each_in_bucket_array(const array* array_p, void (*operation)(const void* key, const void* value, unsigned long long int index, const void* additional_params), const void* additional_params);

void for_each_entry_in_bucket_array(const array* array_p, void (*operation)(const void* key, const void* value, const void* additional_params), const void* additional_params);

void print_bucket_array(const array* array_p, void (*print_key)(const void* key), void (*print_value)(const void* value));

void swap_buckets_bucket_array(array* array_p, unsigned long long int i1, unsigned long long int i2);

#endif