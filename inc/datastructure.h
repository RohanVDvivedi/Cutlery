#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

typedef enum datastructuretype dstype;
enum datastructuretype
{
	ARRAY,
	LINKEDLIST,
	HASHMAP,
	BALANCEDBST,
	STACK,
	QUEUE,
	HEAP
};

// this is bucketted datastructures specific enum
// for the put_entry operation
typedef enum put_type put_type;
enum put_type
{
	PUT_IF_EXISTS 		= 0x01,	// update = 01 decimal
	PUT_IF_NOT_EXISTS 	= 0x10	// insert = 16 decimal
};

typedef struct datastructure datastructure;
struct datastructure
{
	dstype type;
};

ds* get_datastructure(dstype type, int sub_type, unsigned long long int size, int (*key_compare)(const void* key1, const void* key2), unsigned long long int (*hash_function)(const void* key));

// returns true for hashmap, bst, linkedlist if it is bucketted and heap
int is_bucketted(datastructure* ds_p);

// returns true for hashmap, bst, linkedlist if it is bucketted
int is_unique_keyed(datastructure* ds_p);

// returns true for stack, queu and heap
int is_push_pop_type(datastructure* ds_p)

// put a new entry
void put_entry(datastructure* ds_p, const void* key, const void* value, put_type ptype);

// get a new value
const void* find_value(const datastructure* ds_p, const void* input);

// returns 1 if the value is found and removed
int remove_value(datastructure* ds_p, const void* key, const void** return_key, const void** return_value);

// push a new element
void push(datastructure* ds_p, const void* data_p);

// pop the element
void pop(datastructure* ds_p);

// returns pointer to the top element data pointer, (i.e. data pointer to the element will be next popped
const void* get_top(stack* stack_p);

void delete_datastructure(datastructure* ds_p);

#endif